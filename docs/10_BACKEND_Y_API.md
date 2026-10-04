# BACKEND Y API

Este documento explica las decisiones del backend. El código no lleva comentarios, así que el porqué de cada decisión está aquí.

## Capas

```text
api/           Recibe la petición HTTP y valida los datos (schemas).
services/      Lógica de negocio y control de permisos.
repositories/  Consultas a la base de datos.
models.py      Tablas (SQLAlchemy). Debe coincidir con database/migrations/.
```

Los errores de negocio son excepciones propias (`errors.py`) que `main.py` convierte en respuestas HTTP:

| Código | Significado |
|---|---|
| 401 | No hay sesión o la sesión no es válida. |
| 404 | El recurso no existe o pertenece a otro usuario. |
| 409 | Conflicto: correo o nombre ya usados. |
| 413 | El archivo supera el tamaño máximo. |
| 415 | Tipo de archivo no admitido. |
| 400 | El enlace de recuperación de contraseña no es válido o caducó. |
| 422 | Los datos enviados no son válidos. |
| 429 | Demasiados intentos fallidos de inicio de sesión. |

## Patrón Adapter

El backend habla con dos servicios externos: el de IA y el de correo. Cada uno tiene su propia forma de llamarse, distinta de lo que la aplicación necesita. Para que los servicios de la aplicación no dependan de esas librerías, cada servicio externo se usa a través de un adaptador: una clase que ofrece la interfaz que la aplicación espera y la traduce a las llamadas de la librería.

| Interfaz que usa la aplicación | Adaptador | Qué adapta |
|---|---|---|
| `Assistant.reply(system, messages)` devuelve un texto | `ClaudeAssistant` en `app/ai.py` | El SDK de Anthropic: arma la petición, extrae el texto de la respuesta y convierte los errores del SDK en `AIUnavailableError`. |
| `Assistant.reply(system, messages)` devuelve un texto | `OllamaAssistant` en `app/ai.py` | Un modelo local servido por Ollama, por HTTP: elige el modelo, envía la conversación, limpia la respuesta y convierte los fallos en `AIUnavailableError`. |
| `Mailer.send(recipient, subject, body)` | `SmtpMailer` en `app/mailer.py` | La librería `smtplib`: construye el mensaje, abre la conexión cifrada, se identifica y envía. |
| `Mailer.send(recipient, subject, body)` | `ConsoleMailer` en `app/mailer.py` | La consola del servidor, para desarrollo sin correo configurado. |

`AuthService` y `ConversationService` solo conocen las interfaces `Mailer` y `Assistant`. Gracias a eso:

- cambiar de proveedor de IA o de correo significa escribir otro adaptador, sin tocar los servicios;
- las pruebas sustituyen el adaptador por uno simulado y no llaman a ningún servicio real.

Las interfaces se declaran con `typing.Protocol`: cualquier clase que tenga el método con esa forma sirve, sin necesidad de heredar. Los adaptadores tienen sus pruebas en `backend/tests/test_adapters.py`.

## Autenticación

- El registro y el inicio de sesión crean una sesión y la entregan en una cookie `HttpOnly` con `SameSite=Lax`. Al ser `HttpOnly`, el código de la página no puede leerla; al ser `SameSite=Lax`, otros sitios no pueden usarla para enviar peticiones de escritura.
- En la base solo se guarda el hash SHA-256 del identificador de sesión, de modo que una copia de la base no permite suplantar sesiones.
- Las contraseñas se guardan con `scrypt` y una sal aleatoria por contraseña, usando la librería estándar de Python.
- Un inicio de sesión fallido devuelve el mismo mensaje exista o no el correo y realiza la misma verificación de contraseña en ambos casos, para no revelar qué cuentas existen.
- La sesión dura 7 días y se elimina del servidor al cerrar sesión.
- En producción hay que definir `COOKIE_SECURE=1` para que la cookie solo viaje por HTTPS.

- Tras 5 intentos fallidos con el mismo correo en un minuto, el inicio de sesión responde 429 hasta que los intentos salen de esa ventana de tiempo. Un inicio de sesión correcto borra la cuenta de fallos. El control usa la cola de `app/data_structures` (ver `05_COMPLEJIDAD.md`) y vive en memoria, igual que el historial de deshacer.
- El control recuerda como máximo 10 000 correos a la vez; al llegar a ese número descarta los que ya caducaron, para que no pueda crecer sin límite.
- El límite se cuenta por correo, no por dirección IP: alguien que conozca un correo puede bloquear su inicio de sesión durante un minuto escribiendo contraseñas falsas.

### Recuperación de contraseña

- «Olvidé mi contraseña» pide un correo. Si existe una cuenta, se genera un enlace válido 30 minutos y de un solo uso; la respuesta es la misma exista o no la cuenta, para no revelar qué correos están registrados.
- En la base solo se guarda el hash del identificador del enlace. Pedir un enlace nuevo invalida el anterior.
- Al cambiar la contraseña se cierran todas las sesiones abiertas de esa cuenta.
- El correo se envía por SMTP si están definidas `SMTP_HOST`, `SMTP_PORT`, `SMTP_USER`, `SMTP_PASSWORD` y `SMTP_FROM`. Si `SMTP_HOST` no está definida, el mensaje con el enlace se escribe en la consola del servidor: sirve para desarrollo, pero en producción hay que configurar el correo.
- `APP_URL` indica la dirección de la aplicación que se pone en el enlace.
- Se atienden como máximo 3 solicitudes por correo cada 15 minutos; las demás se ignoran sin avisar, con la misma respuesta. Usa el mismo control basado en la cola que el inicio de sesión.
- El envío real por SMTP no está probado: las pruebas usan un envío simulado.

## Permisos

Todo lo que pertenece a un proyecto exige sesión y que el proyecto sea del usuario. Un proyecto ajeno responde 404, igual que uno inexistente, para no revelar qué identificadores existen. La comprobación está centralizada en `services/base.py`.

## Archivos

- Se guardan en disco, en la carpeta indicada por `UPLOAD_DIR`, con un nombre aleatorio. El nombre original solo se guarda como texto para mostrarlo; nunca decide dónde se escribe el archivo.
- El tipo se detecta por los primeros bytes del contenido, no por la extensión ni por lo que declare el navegador. Se admiten PDF, PNG, JPEG y WebP, hasta 20 MB.
- Al eliminar un archivo se desvincula de forma explícita de los planos y elevaciones que lo usaban. No se depende de `ON DELETE SET NULL` porque SQLite, que se usa en las pruebas, solo lo aplica si se activan las claves foráneas.
- Al eliminar un proyecto se borran también sus archivos del disco.
- El botón «Ver», en la pestaña Archivos y junto al archivo adjunto de un plano o una elevación, abre el archivo dentro de la aplicación en una ventana superpuesta (`frontend/src/components/FileViewer.tsx` y `Modal.tsx`): las imágenes se muestran ajustadas a la ventana y los PDF con el lector del navegador. La ventana usa el elemento `<dialog>` del navegador, así que se cierra con Escape, con el botón «Cerrar» o pulsando fuera, y tiene un enlace para abrir el archivo en otra pestaña.

## Deshacer eliminaciones

- Se guarda un historial por proyecto de los terrenos, planos, elevaciones y materiales eliminados, con un máximo de 20 entradas.
- El historial usa la lista doblemente enlazada de `app/data_structures` (ver `05_COMPLEJIDAD.md`).
- Vive en memoria: se pierde al reiniciar el servidor y no se comparte entre varios procesos.
- Al restaurar se inserta una fila nueva con los mismos datos; el identificador cambia porque el anterior pudo haberse reutilizado.
- Si al restaurar hay un conflicto (por ejemplo, ya existe un material con ese nombre), se responde 409 y la entrada se conserva.
- Si el archivo adjunto de un plano ya no existe, el plano se restaura sin archivo.

## Recomendaciones automáticas

`POST /projects/{id}/recommendations/generate` revisa los terrenos y materiales con reglas fijas (`services/recommendation_rules.py`) y reemplaza las recomendaciones automáticas anteriores, sin tocar las manuales. No usa IA; se guardan con origen `system`.

Las reglas son orientativas (por ejemplo, avisar cuando la pendiente es de 15 % o más) y no sustituyen un estudio técnico. Los textos están en español porque se muestran al usuario.

## Asistente de IA

- Hay dos proveedores, cada uno con su adaptador. El backend elige uno en cada pregunta:
  - Si está definida `ANTHROPIC_API_KEY`, usa Claude mediante el SDK oficial de Anthropic. Es de pago.
  - Si no, usa un modelo local servido por [Ollama](https://ollama.com), que es gratuito y no necesita clave.
- En cada pregunta se envían los datos del proyecto y el historial completo de la conversación.
- La llamada real a la IA no está cubierta por pruebas automáticas: las pruebas usan un asistente simulado y un servidor de Ollama simulado.

### Modelo local con Ollama

No hay que configurar nada en `backend/.env`. Pasos, una sola vez:

1. Instalar Ollama desde https://ollama.com.
2. Descargar un modelo, por ejemplo `ollama pull llama3.2` (unos 2 GB).

Con Ollama encendido, el asistente lo encuentra en `http://127.0.0.1:11434` y usa el primer modelo instalado. Dos variables opcionales cambian eso: `OLLAMA_MODEL` fija el modelo y `OLLAMA_URL` la dirección.

- La velocidad y la calidad de las respuestas dependen del equipo y del modelo; un modelo pequeño responde peor que Claude.
- Si Ollama no está encendido o no tiene modelos, el backend lo detecta en un segundo como máximo y contesta con las reglas.
- Algunos modelos escriben su razonamiento entre etiquetas `<think>`; el adaptador lo quita de la respuesta.

### Respaldo por reglas

Si no hay ningún proveedor disponible (ni clave de Anthropic ni Ollama encendido), o la llamada a la IA falla (clave rechazada, límite de uso, error del servicio o de red), el asistente no devuelve un error: contesta con reglas fijas (`services/assistant_rules.py`) sobre los datos del proyecto.

- Las reglas buscan palabras clave en la pregunta y reconocen tres temas: terreno (área, pendiente, desnivel, suelo), materiales y costos, y planos y elevaciones. Una pregunta puede tocar varios temas. Si no reconoce ninguno, resume el proyecto y dice sobre qué puede responder.
- Reutilizan los avisos de `services/recommendation_rules.py`, para que el asistente y las recomendaciones automáticas digan lo mismo.
- Cada mensaje del asistente guarda su origen en el campo `source`: `ai` si lo escribió la IA, `rules` si salió de las reglas. Los mensajes del usuario lo tienen vacío. La interfaz marca las respuestas por reglas con la etiqueta «Respuesta por reglas».
- No entienden el lenguaje: solo comparan palabras. Sirven para que el asistente sea útil sin IA, no para sustituirla.

## Esquemas del terreno

La pestaña Terreno dibuja cuatro esquemas en SVG por cada terreno (`frontend/src/components/TerrainDiagrams.tsx` y `Terrain3D.tsx`), sin librerías adicionales:

- **Vista superior**: el contorno del lote a escala.
- **Vista frontal**: el lote visto desde el frente, que es su lado más bajo. Conserva el ancho y convierte la profundidad de cada vértice en altura (`profundidad × pendiente / 100`), así que un lote rectangular se ve como una franja de ancho por desnivel.
- **Vista lateral**: el perfil del terreno, una línea con la pendiente real. El desnivel se calcula como `largo × pendiente / 100`.
- **Vista 3D**: el lote como una superficie inclinada según la pendiente, que se puede girar con un control. Es una proyección calculada a mano: cada vértice se rota alrededor del eje vertical y se proyecta con una inclinación fija de 30°.

La vista 3D tiene cuatro capas que se pueden mostrar u ocultar: superficie, plano base (el nivel de referencia), límites (contorno y aristas verticales) y medidas. No hay capas de construcción, vegetación ni vías, porque el proyecto no guarda esos datos.

### Lotes no rectangulares

Un terreno puede tener una lista de vértices `x y` en metros, guardados en la tabla `terrain_points` con su posición en el contorno. Si los tiene, los esquemas dibujan ese polígono; si no, usan el rectángulo de ancho por largo.

- Se aceptan entre 3 y 50 vértices, y deben encerrar un área mayor que cero.
- El área se calcula con la fórmula del área de Gauss (o «del cordón»), recorriendo los vértices una vez: O(n). Está en `backend/app/services/geometry.py` y en `frontend/src/utils/geometry.ts`.
- No se comprueba que el contorno no se cruce a sí mismo.
- Al deshacer la eliminación de un terreno se recuperan sus datos, pero no sus vértices.

Simplificaciones: la superficie es un plano inclinado y se asume que la pendiente va en el sentido del largo (el eje `y`). El ancho y el largo son opcionales; si faltan, el esquema muestra qué dato falta. El área se guarda aparte porque un lote real puede no ser rectangular; el formulario la propone como ancho por largo si se deja vacía.

## Plano de implantación

La pestaña Planos dibuja, debajo de la lista de planos, un plano de implantación en SVG por cada terreno (`frontend/src/components/SitePlan.tsx`). Usa solo los datos del terreno:

- **Límite del terreno**: el contorno del lote a escala, con una cota en cada lado. La longitud de cada lado y hacia dónde queda el exterior se calculan recorriendo los vértices una vez: O(n) (`edges` en `frontend/src/utils/geometry.ts`).
- **Área edificable**: el rectángulo que queda al descontar un retiro igual en los cuatro lados. El retiro se elige con un control (de 0 a 10 m; al abrir vale 3 m, o menos si el lote es estrecho, para que siempre quede área edificable) y el plano muestra el área resultante.
- **Acceso**: una marca en el frente del lote.
- **Norte**: una flecha cuya dirección se elige con un control.
- **Leyenda** y botón **Descargar plano (SVG)**. El archivo descargado lleva sus colores dentro, así que se ve igual fuera de la aplicación.
- Botón **Imprimir o guardar como PDF**: abre la impresión del navegador con el plano solo, en una hoja A4 horizontal. Para obtener el PDF se elige «Guardar como PDF» como impresora. No usa ninguna librería.

Simplificaciones:

- El retiro y el norte no se guardan: el proyecto no tiene esos datos y vuelven a su valor inicial al recargar.
- El acceso se asume por el frente, el lado más bajo, igual que en la vista frontal del terreno.
- El área edificable solo se calcula en lotes rectangulares. En un lote con vértices se dibujan el contorno y las cotas, sin área edificable.
- No se dibujan vivienda, áreas verdes, andenes ni parqueadero, porque el proyecto no guarda esos datos.

## Datos numéricos

La API recibe y devuelve áreas, cantidades y costos como números JSON. En la base de datos son `NUMERIC`.

## Variables de entorno

Se pueden definir en la terminal o en el archivo `backend/.env`, que `python -m app.dev` lee al arrancar. Una variable ya definida en la terminal tiene prioridad sobre el archivo. `backend/.env` está excluido del repositorio porque contiene contraseñas.

| Variable | Uso |
|---|---|
| `DATABASE_URL` | Conexión a la base de datos (obligatoria). |
| `UPLOAD_DIR` | Carpeta de archivos subidos (por defecto `uploads`). |
| `COOKIE_SECURE` | `1` para exigir HTTPS en la cookie de sesión. |
| `ANTHROPIC_API_KEY` | Usa Claude como asistente de IA. Sin ella se usa el modelo local de Ollama. |
| `OLLAMA_MODEL` | Modelo local que usa el asistente (por defecto, el primero instalado). |
| `OLLAMA_URL` | Dirección de Ollama (por defecto `http://127.0.0.1:11434`). |
| `APP_URL` | Dirección de la aplicación, para el enlace del correo de recuperación. |
| `SMTP_HOST`, `SMTP_PORT`, `SMTP_USER`, `SMTP_PASSWORD`, `SMTP_FROM` | Servidor de correo para la recuperación de contraseña. |

### Comprobar los servicios externos

Después de escribir la clave de IA o los datos del correo en `backend/.env`, este comando comprueba que funcionan de verdad, sin arrancar el servidor:

```bash
cd backend
python -m app.check correo@ejemplo.com
```

Hace una pregunta corta al asistente y envía un mensaje de prueba a la dirección indicada. Por cada servicio escribe `OK` o `FAILED` con el motivo (falta la variable, la clave fue rechazada, no se pudo conectar). Sin dirección, solo comprueba el asistente. La pregunta al asistente es una llamada real al proveedor que esté activo: con Claude consume una cantidad pequeña de crédito; con Ollama no cuesta nada.

## Frontend

Los mensajes de error del backend están en inglés. El frontend los traduce al español en `frontend/src/utils/errors.ts`; un mensaje que no esté en esa lista se muestra tal cual. Si cualquier petición responde 401, la aplicación vuelve a la pantalla de inicio de sesión.

En desarrollo, Vite reenvía las peticiones `/api/*` al backend en `localhost:8000`, por lo que el backend no necesita configurar CORS y la cookie de sesión funciona en el mismo origen. Los tipos de `frontend/src/types/api.ts` reflejan los de `backend/app/schemas.py` y deben mantenerse sincronizados.
