# BACKEND Y API

Este documento explica las decisiones del backend. El código no lleva comentarios, así que el porqué de cada decisión está aquí.

## Capas

```text
api/           Recibe la petición HTTP y valida los datos (schemas).
services/      Lógica de negocio y control de permisos.
repositories/  Consultas a la base de datos.
models.py      Tablas (SQLAlchemy). Debe coincidir con database/schema.sql.
```

Los errores de negocio son excepciones propias (`errors.py`) que `main.py` convierte en respuestas HTTP:

| Código | Significado |
|---|---|
| 401 | No hay sesión o la sesión no es válida. |
| 404 | El recurso no existe o pertenece a otro usuario. |
| 409 | Conflicto: correo o nombre ya usados. |
| 413 | El archivo supera el tamaño máximo. |
| 415 | Tipo de archivo no admitido. |
| 422 | Los datos enviados no son válidos. |
| 429 | Demasiados intentos fallidos de inicio de sesión. |
| 503 | El asistente de IA no está disponible. |

## Autenticación

- El registro y el inicio de sesión crean una sesión y la entregan en una cookie `HttpOnly` con `SameSite=Lax`. Al ser `HttpOnly`, el código de la página no puede leerla; al ser `SameSite=Lax`, otros sitios no pueden usarla para enviar peticiones de escritura.
- En la base solo se guarda el hash SHA-256 del identificador de sesión, de modo que una copia de la base no permite suplantar sesiones.
- Las contraseñas se guardan con `scrypt` y una sal aleatoria por contraseña, usando la librería estándar de Python.
- Un inicio de sesión fallido devuelve el mismo mensaje exista o no el correo y realiza la misma verificación de contraseña en ambos casos, para no revelar qué cuentas existen.
- La sesión dura 7 días y se elimina del servidor al cerrar sesión.
- En producción hay que definir `COOKIE_SECURE=1` para que la cookie solo viaje por HTTPS.

- Tras 5 intentos fallidos con el mismo correo en un minuto, el inicio de sesión responde 429 hasta que los intentos salen de esa ventana de tiempo. Un inicio de sesión correcto borra la cuenta de fallos. El control usa la cola de `app/data_structures` (ver `05_COMPLEJIDAD.md`) y vive en memoria, igual que el historial de deshacer.
- El límite se cuenta por correo, no por dirección IP: alguien que conozca un correo puede bloquear su inicio de sesión durante un minuto escribiendo contraseñas falsas.

Pendiente: recuperación de contraseña.

## Permisos

Todo lo que pertenece a un proyecto exige sesión y que el proyecto sea del usuario. Un proyecto ajeno responde 404, igual que uno inexistente, para no revelar qué identificadores existen. La comprobación está centralizada en `services/base.py`.

## Archivos

- Se guardan en disco, en la carpeta indicada por `UPLOAD_DIR`, con un nombre aleatorio. El nombre original solo se guarda como texto para mostrarlo; nunca decide dónde se escribe el archivo.
- El tipo se detecta por los primeros bytes del contenido, no por la extensión ni por lo que declare el navegador. Se admiten PDF, PNG, JPEG y WebP, hasta 20 MB.
- Al eliminar un archivo se desvincula de forma explícita de los planos y elevaciones que lo usaban. No se depende de `ON DELETE SET NULL` porque SQLite, que se usa en las pruebas, solo lo aplica si se activan las claves foráneas.
- Al eliminar un proyecto se borran también sus archivos del disco.

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

- Usa Claude mediante el SDK oficial de Anthropic. La clave se toma de la variable de entorno `ANTHROPIC_API_KEY`; sin ella, los endpoints responden 503.
- En cada pregunta se envían los datos del proyecto y el historial completo de la conversación.
- Si la IA falla no se guarda nada, para que el historial no quede con preguntas sin respuesta.
- La llamada real a la IA no está cubierta por pruebas automáticas: las pruebas usan un asistente simulado.

## Esquemas del terreno

La pestaña Terreno dibuja dos esquemas en SVG por cada terreno (`frontend/src/components/TerrainDiagrams.tsx`), sin librerías adicionales:

- **Vista superior**: un rectángulo con el ancho y el largo, a escala entre sí.
- **Perfil**: una línea con la pendiente real. El desnivel se calcula como `largo × pendiente / 100`.

Son simplificaciones: el lote se trata como un rectángulo y se asume que la pendiente va en el sentido del largo. El ancho y el largo son opcionales; si faltan, el esquema muestra qué dato falta. El área se guarda aparte porque un lote real puede no ser rectangular; el formulario la propone como ancho por largo si se deja vacía.

## Datos numéricos

La API recibe y devuelve áreas, cantidades y costos como números JSON. En la base de datos son `NUMERIC`.

## Variables de entorno

| Variable | Uso |
|---|---|
| `DATABASE_URL` | Conexión a la base de datos (obligatoria). |
| `UPLOAD_DIR` | Carpeta de archivos subidos (por defecto `uploads`). |
| `COOKIE_SECURE` | `1` para exigir HTTPS en la cookie de sesión. |
| `ANTHROPIC_API_KEY` | Activa el asistente de IA. |

## Frontend

En desarrollo, Vite reenvía las peticiones `/api/*` al backend en `localhost:8000`, por lo que el backend no necesita configurar CORS y la cookie de sesión funciona en el mismo origen. Los tipos de `frontend/src/types/api.ts` reflejan los de `backend/app/schemas.py` y deben mantenerse sincronizados.
