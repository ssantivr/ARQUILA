# PROMPTS

Instrucciones que se dieron al asistente de IA durante el desarrollo de ARQUILA.

## Reglas del proyecto

| Archivo | Contenido |
|---|---|
| `01_PROMPT_MAESTRO_ARQUILA.md` | Reglas generales: alcance académico, organización de carpetas y regla de código sin comentarios. |
| `02_PROMPT_UI_VISUAL.md` | Dirección visual y paleta adoptada. |
| `03_PROMPT_ARQUITECTURA_FRONTEND.md` | Organización del frontend en TypeScript. |
| `04_PROMPT_EJECUCION_EFICIENTE.md` | Forma de trabajar para gastar pocos tokens. |
| `05_PROMPT_IA_BASE_DATOS.md` | Directrices del backend en Python. |

Las reglas del asistente también están en `.agentes/claude_solver.md`.

## Prompts de trabajo

Encargos concretos, copiados tal como se enviaron el 3 de octubre de 2026.

| Archivo | Encargo |
|---|---|
| `06_PROMPT_REVISION_INICIAL.md` | Completar las estructuras de datos en C++, verificar el backend y revisar el esquema de la base. |
| `07_PROMPT_BACKEND_Y_FRONTEND.md` | Alinear el frontend en TypeScript con el backend en Python. |
| `08_PROMPT_STACK_ESTRICTO.md` | Auditar que el backend sea solo Python y el frontend solo TypeScript. |

Varias rutas que mencionan estos tres prompts cambiaron después: `database/schema.sql` pasó a ser `database/migrations/001_initial_schema.sql`, y la regla de no escribir comentarios en el código se aplica aunque el segundo y el tercero hablen de comentarios en inglés.
