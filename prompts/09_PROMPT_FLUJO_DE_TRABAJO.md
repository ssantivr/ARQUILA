# PROMPT FLUJO DE TRABAJO

## Propósito

Describe cómo trabajar en ARQUILA con Claude Code gastando pocos tokens: qué leer, en qué orden hacer un cambio y qué comandos correr para comprobarlo. Concreta las ideas de `04_PROMPT_EJECUCION_EFICIENTE.md` con las rutas y los comandos reales del repositorio.

## Qué leer antes de tocar nada

Leer solo lo que el cambio necesita. Para orientarse basta con esto:

| Para saber | Leer |
|---|---|
| Qué rutas existen | `backend/app/main.py` (lista de routers) y el archivo de `backend/app/api/` que toque. |
| Cómo se llama y valida un dato | `backend/app/schemas.py` y `frontend/src/types/api.ts`, que deben coincidir. |
| Por qué algo se hizo así | La sección correspondiente de `docs/12_BACKEND_Y_API.md`. El código no lleva comentarios. |
| Qué está hecho y qué falta | La tabla «Estado real» de `docs/13_EVOLUCION_POR_SEMANAS.md`. |

No leer carpetas enteras ni `package-lock.json`, `node_modules/`, `dist/` o `.venv/`. Buscar por nombre antes de abrir archivos.

## Orden de un cambio

Un recurso nuevo recorre siempre las mismas capas. Copiar la forma de uno que ya exista (por ejemplo, cuartos) en vez de diseñar de cero:

1. **Base de datos:** archivo nuevo en `database/migrations/` con el siguiente número, y la misma tabla en `backend/app/models.py`. Nunca modificar una migración ya aplicada.
2. **Backend:** esquemas en `schemas.py`, repositorio en `repositories/`, servicio en `services/` (hereda de `ProjectScopedService` para los permisos), rutas en `api/` y registro del router en `main.py`.
3. **Pruebas:** un archivo `backend/tests/test_<recurso>.py` que cubra crear, listar, validar, permisos entre usuarios y eliminación en cascada.
4. **Frontend:** tipos en `types/api.ts`, llamadas en `services/api.ts`, traducción de los mensajes de error nuevos en `utils/errors.ts`, y el panel en `components/`.
5. **Documentación:** la decisión y sus simplificaciones en `docs/12_BACKEND_Y_API.md`; la migración en `docs/07_BASE_DATOS.md`; lo que cubren las pruebas en `docs/06_PRUEBAS.md`.

El frontend solo habla con el backend por HTTP, a través de `frontend/src/services/http.ts`. Ningún componente llama a `fetch` directamente ni conoce la base de datos.

## Comandos de comprobación

Correr solo los del lado que se tocó, y los dos antes de entregar.

```bash
cd backend
pytest
```

```bash
cd frontend
npm test
npm run build
```

Antes de entregar, también el formato y las reglas de estilo, desde la raíz y con el entorno virtual del backend activado:

```bash
python scripts/quality.py
```

Para ver una pantalla: `python -m app.dev` en `backend` y `npm run dev` en `frontend`, y entrar con el usuario de demostración. El visor 3D necesita un navegador con WebGL.

## Reglas que ahorran tokens

- Pedir y hacer cambios por fragmentos, no reescribir archivos completos.
- Un encargo por mensaje, con el resultado esperado. Un encargo que repite lo ya hecho se contesta diciendo qué existe, no rehaciéndolo.
- No pegar la salida completa de las pruebas: basta la última línea, o el fallo.
- No explicar la historia del proyecto en cada respuesta; está en `docs/13_EVOLUCION_POR_SEMANAS.md`.
- Si un encargo contradice una regla del repositorio (por ejemplo, pedir comentarios en el código), preguntar una vez y seguir la respuesta.

## Entrega

1. Las pruebas y el build pasan.
2. Commit en inglés, estilo convencional (`feat:`, `fix:`, `docs:`).
3. Subir a `santiago` y avanzar `main` al mismo commit, sin forzar.
4. Una versión de entrega se marca con una etiqueta anotada `v1.0.x`.
