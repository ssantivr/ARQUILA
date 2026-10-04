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
- `GET /assistant/status` dice quién responderá ahora: `claude`, `ollama` (con el nombre del modelo) o `rules` si no hay IA disponible. La pestaña Asistente lo muestra en la línea «Ahora responde». Se comprueba al abrir la pestaña; si Ollama se enciende o se apaga después, la línea no cambia hasta volver a entrar, aunque cada respuesta sigue llevando su origen real.
- En cada pregunta se envían los datos del proyecto y el historial completo de la conversación. Los totales de costo van ya calculados, para que el modelo los cite en lugar de calcularlos.
- La llamada real a la IA no está cubierta por pruebas automáticas: las pruebas usan un asistente simulado y un servidor de Ollama simulado.

### Modelo local con Ollama

No hay que configurar nada en `backend/.env`. Pasos, una sola vez:

1. Instalar Ollama desde https://ollama.com.
2. Descargar un modelo: `ollama pull llama3.2` (unos 2 GB). Los modelos más pequeños, como `llama3.2:1b`, se inventan cifras del proyecto.

Con Ollama encendido, el asistente lo encuentra en `http://127.0.0.1:11434` y usa el primer modelo instalado. Dos variables opcionales cambian eso: `OLLAMA_MODEL` fija el modelo y `OLLAMA_URL` la dirección.

- La velocidad y la calidad de las respuestas dependen del equipo y del modelo; un modelo pequeño responde peor que Claude.
- Probado el 3 de octubre de 2026 con Ollama 0.35.1 y `llama3.2:1b` (1,3 GB): la conexión funciona y las respuestas quedan guardadas con origen `ai`. La primera respuesta tardó cerca de un minuto, mientras se cargaba el modelo; las siguientes, unos dos segundos. Pero ese modelo no es fiable con los datos: acertó la pendiente del terreno y se inventó el costo total de los materiales y el nombre de un plano. Con un modelo tan pequeño, las reglas dan cifras más fiables.
- Probado el mismo día con `llama3.2` (2 GB) y las mismas tres preguntas: acertó la pendiente, el costo total y el detalle de los materiales, y el plano registrado. Es el modelo recomendado; se fija con `OLLAMA_MODEL=llama3.2`. Sigue siendo un modelo pequeño y redacta con alguna imprecisión.
- Los modelos pequeños fallan al hacer cuentas: en un proyecto con ocho materiales, `llama3.2` dio bien el costo total pero señaló como más caro un material que no lo era, con una cifra mal multiplicada. Por eso el backend no le deja calcular: los datos que recibe llevan ya el costo de cada material, el total y el nombre del más caro, con los materiales ordenados de mayor a menor costo, y las instrucciones le piden citar esas cifras tal cual. Con ese cambio, la misma pregunta se respondió bien.
- Si Ollama no está encendido o no tiene modelos, el backend lo detecta en un segundo como máximo y contesta con las reglas. Después no vuelve a intentar la conexión durante 30 segundos.
- Algunos modelos escriben su razonamiento entre etiquetas `<think>`; el adaptador lo quita de la respuesta.

### Respaldo por reglas

Si no hay ningún proveedor disponible (ni clave de Anthropic ni Ollama encendido), o la llamada a la IA falla (clave rechazada, límite de uso, error del servicio o de red), el asistente no devuelve un error: contesta con reglas fijas (`services/assistant_rules.py`) sobre los datos del proyecto.

- Las reglas buscan palabras clave en la pregunta y reconocen tres temas: terreno (área, pendiente, desnivel, suelo), materiales y costos, y planos y elevaciones. Una pregunta puede tocar varios temas. Si no reconoce ninguno, resume el proyecto y dice sobre qué puede responder.
- Reutilizan los avisos de `services/recommendation_rules.py`, para que el asistente y las recomendaciones automáticas digan lo mismo.
- Cada mensaje del asistente guarda su origen en el campo `source`: `ai` si lo escribió la IA, `rules` si salió de las reglas. Los mensajes del usuario lo tienen vacío. La interfaz marca las respuestas por reglas con la etiqueta «Respuesta por reglas».
- No entienden el lenguaje: solo comparan palabras. Sirven para que el asistente sea útil sin IA, no para sustituirla.

## Esquemas del terreno

La pestaña Terreno dibuja cinco esquemas en SVG por cada terreno (`frontend/src/components/TerrainDiagrams.tsx` y `Terrain3D.tsx`), sin librerías adicionales:

- **Vista superior**: el contorno del lote a escala.
- **Curvas de nivel**: el lote visto desde arriba con una línea cada cierto desnivel y franjas más oscuras cuanto más alto está el terreno. El intervalo entre curvas se elige de una lista de valores redondos (0,1 m, 0,25 m, 0,5 m, 1 m, 2 m…) para que salgan como mucho seis. Como el terreno se modela como un plano inclinado, las curvas son rectas paralelas al frente.
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

## Cuartos

Un cuarto pertenece a un plano y se gestiona en la pestaña Modelo 3D (`frontend/src/components/RoomsPanel.tsx`). Las rutas siguen el mismo patrón que los planos: `POST` y `GET /projects/{id}/rooms`, y `GET`, `PATCH` y `DELETE /rooms/{id}`.

- Un cuarto es una caja: nombre, posición (`x_m`, `y_m`), ancho, largo y alto, en metros. La posición es la esquina del cuarto medida desde la esquina de origen del primer terreno del modelo.
- El plano de un cuarto debe ser del mismo proyecto; si no, se responde 404, igual que con el archivo de un plano.
- Las medidas deben ser mayores que cero. Lo validan el esquema (422) y la base de datos (`CHECK`).
- Al eliminar un plano o un proyecto se eliminan sus cuartos.

Simplificaciones:

- Los cuartos no entran en el historial de deshacer; por eso la interfaz pide confirmación antes de eliminar uno. Al deshacer la eliminación de un plano se recupera el plano, pero no sus cuartos.
- No se comprueba que un cuarto quede dentro del terreno ni que dos cuartos no se solapen.
- No hay puertas, ventanas ni componentes estructurales (columnas, vigas, muros).

## Modelo 3D

La pestaña Modelo 3D muestra el proyecto en tres dimensiones con Three.js. Los datos salen de `GET /projects/{id}/structure` (`backend/app/services/structure_service.py`), que no guarda nada: arma la respuesta con los terrenos, los planos y los cuartos del proyecto, en metros.

- **Terrenos**: cada terreno con contorno (vértices, o ancho y largo) es una losa. Los terrenos se colocan uno al lado del otro sobre el eje `x`, separados 5 m, porque cada uno guarda sus coordenadas desde su propio origen.
- **Niveles**: un plano es un nivel si tiene cuartos o si su campo Nivel es un número (`0`, `1`, `-1`, `0.5`). Así un plano de implantación o de detalles, con el nivel vacío o con texto, no se apila como si fuera un piso. Los niveles se apilan en el orden en que se crearon los planos, no por el número. Un plano con cuartos muestra sus cuartos (`kind: "room"`) y el nivel mide lo que su cuarto más alto. Un plano sin cuartos se dibuja como un volumen de 3 m (`kind: "volume"`) sobre el área edificable del primer terreno rectangular, con el mismo retiro inicial del plano de implantación (3 m, o menos si el lote es estrecho).

En el frontend, `frontend/src/three/structureViewer.ts` contiene toda la escena y no depende de React; `StructureViewer.tsx` la crea al montar, la destruye al desmontar y dibuja encima los paneles flotantes. Decisiones del visor:

- El eje `y` del plano pasa a ser `-z` en la escena. Así la altura queda en `y` y el modelo no sale reflejado.
- Las losas se generan por extrusión del contorno, que corrige el sentido de los vértices; por eso las caras quedan hacia afuera aunque el lote se haya escrito en sentido horario.
- Las sombras usan `PCFShadowMap`, que en la versión instalada de Three.js ya filtra los bordes; `PCFSoftShadowMap` se retiró de la librería y solo producía un aviso en la consola.
- Los planos `near` y `far` de la cámara y la cámara de sombras de la luz se ajustan al tamaño del modelo. Junto con `polygonOffset` en los materiales evita el parpadeo entre caras que coinciden (z-fighting) y las sombras recortadas.
- La cámara se encuadra solo la primera vez y con el botón «Restablecer vista». Al agregar o editar un cuarto el modelo se reconstruye, pero la cámara se queda donde el usuario la dejó.
- **Selección**: un clic sobre un cuarto lo selecciona con un rayo desde la cámara (`Raycaster`). Si el puntero se movió más de 4 px entre pulsar y soltar se considera un giro de cámara y no una selección. El cuarto seleccionado lo guarda React, no la escena, así que la lista de niveles y el inspector muestran siempre lo mismo que el modelo; la lista permite además seleccionar con el teclado.
- Al cambiar de modelo o salir de la pestaña se liberan geometrías, materiales, el mapa de sombras, los eventos y el contexto WebGL (`dispose`).
- Three.js se carga solo al abrir la pestaña (`lazy` en `ProjectDetailPage.tsx`), para no aumentar la carga inicial.
- Los paneles flotantes (niveles, inspector y barra inferior) usan `backdrop-filter: blur` y acentos cian `#00F0FF` y magenta `#FF007F`. Ese estilo se limita al visor (clases `.structure-stage` y `.hud` en `styles.css`); el resto de la aplicación conserva su paleta. En pantallas estrechas los paneles pasan debajo del modelo.

Simplificaciones: la pendiente del terreno no se representa. En un lote con vértices no se dibuja el volumen de un plano sin cuartos, igual que en el plano de implantación; los cuartos sí se dibujan siempre.

## Datos numéricos

La API recibe y devuelve áreas, cantidades y costos como números JSON. En la base de datos son `NUMERIC`.

## Rendimiento

Revisión hecha el 3 de octubre de 2026: se contó cuántas consultas a la base de datos hace cada endpoint con pocos datos (2 proyectos de 2 elementos) y con más (20 proyectos de 20 elementos). Si el número crece con los datos, hay una consulta repetida por cada elemento.

- Todas las listas hacen un número fijo de consultas, entre 3 y 5, sea cual sea la cantidad de datos.
- Se corrigió la lista de terrenos, que hacía una consulta más por cada terreno para traer sus vértices: con 20 terrenos pasaba de 6 a 24 consultas. Ahora trae los vértices de todos en una sola consulta y se queda en 5. Una prueba comprueba que el número no cambia al añadir terrenos.
- Se corrigió una espera en el asistente: sin Ollama encendido, cada pregunta perdía un segundo intentando conectar. Ahora, tras un intento fallido, el backend no vuelve a intentarlo durante 30 segundos y responde con las reglas de inmediato. La contrapartida es que, al encender Ollama, el asistente puede tardar hasta 30 segundos en notarlo.
- Generar recomendaciones y eliminar un proyecto sí hacen más consultas cuantos más elementos hay, porque escriben o borran una fila por elemento. Son operaciones poco frecuentes y no se cambiaron.
- El frontend compilado pesa unos 200 kB (62 kB comprimido), sin librerías de gráficos: los esquemas son SVG hechos a mano.

Las medidas se hicieron con una base SQLite en memoria; cuentan consultas, no tiempos reales contra PostgreSQL.

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
| `APP_URL` | Dirección de la aplicación, para el enlace del correo de recuperación. También es el origen que CORS admite si no se define `CORS_ORIGINS`. |
| `CORS_ORIGINS` | Orígenes que pueden llamar a la API desde el navegador, separados por comas (por defecto, `APP_URL`). |
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

En desarrollo, Vite reenvía las peticiones `/api/*` al backend en `localhost:8000`, por lo que la cookie de sesión funciona en el mismo origen. Si el frontend se sirve desde otro origen (`VITE_API_URL` con la dirección completa de la API), el backend lo admite por CORS solo si está en `CORS_ORIGINS`; no se usa `*` porque las peticiones llevan la cookie de sesión. Los tipos de `frontend/src/types/api.ts` reflejan los de `backend/app/schemas.py` y deben mantenerse sincronizados.
