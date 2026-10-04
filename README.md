# ARQUILA

| | |
|---|---|
| Universidad | [UNIVERSIDAD] |
| Materia | [MATERIA] |
| Docente | [DOCENTE] |
| Integrantes | [INTEGRANTES] |
| Fecha | [FECHA] |

Proyecto académico de cuarto semestre de Ingeniería de Software.

Licencia: uso académico. El código y los documentos se entregan para su evaluación en la materia; no se concede permiso para uso comercial.

ARQUILA es una aplicación web para organizar proyectos de arquitectura: reúne en un mismo lugar el terreno, los cuartos, los materiales, los componentes estructurales y un modelo 3D de cada proyecto, con un asistente de IA que responde sobre esos datos.

## Objetivo

Aplicar los conceptos de estructuras de datos vistos en clase dentro de un proyecto de software organizado.

## Alcance académico

Las estructuras estudiadas están implementadas desde cero en Python, en `backend/app/data_structures/`, sin librerías que las reemplacen.

| Estructura | Uso en la aplicación | Dónde está el uso |
|---|---|---|
| Array unidimensional | Cálculo del área de un lote recorriendo sus vértices. | `backend/app/services/geometry.py` |
| Array dinámico | Orden de los materiales por costo para el asistente. | `backend/app/services/material_ranking.py` |
| Stack (LIFO) | «Rehacer» una eliminación deshecha. | `backend/app/services/undo_history.py` |
| Queue (FIFO) | Límite de intentos fallidos de inicio de sesión. | `backend/app/services/login_limiter.py` |
| Lista simplemente enlazada | Ventana con los últimos 20 mensajes enviados a la IA. | `backend/app/services/conversation_context.py` |
| Lista doblemente enlazada | Historial de eliminaciones para «Deshacer». | `backend/app/services/undo_history.py` |

El detalle está en `docs/04_ESTRUCTURAS_DATOS.md`, la complejidad de cada operación y las mediciones de tiempo en `docs/05_COMPLEJIDAD.md`, y el recorrido para mostrar cada estructura en la aplicación en `docs/14_GUION_DEMO.md`.

## Criterios de diseño

- **Qué quedó fuera.** No se implementaron árboles, grafos ni tablas hash: no se vieron en clase. El proyecto se limita a las seis estructuras de la tabla anterior.
- **Qué se hizo a mano.** Las seis estructuras y sus operaciones (`backend/app/data_structures/`), la búsqueda lineal y la binaria, el orden de los materiales por inserción en un array dinámico y el cálculo del área de un lote. No se usa `collections.deque` ni otra librería que las reemplace.
- **Qué viene de librerías.** El servidor web (FastAPI), el acceso a la base de datos (SQLAlchemy y psycopg), el hash de contraseñas (argon2-cffi), la interfaz (React), el modelo 3D (Three.js) y las herramientas de pruebas y de formato.

## Tecnologías

- Python y FastAPI para el backend, con SQLAlchemy y PostgreSQL, Argon2 para las contraseñas y pytest para las pruebas.
- TypeScript, React y Vite para el frontend, con Three.js para el modelo 3D, y Vitest con Testing Library para las pruebas.
- Ruff, ESLint y Prettier para el formato y las reglas de estilo.
- Git para control de versiones y GitHub Actions para ejecutar las comprobaciones.

## Regla de código

Los identificadores del código están en inglés y el código no contiene comentarios ni documentación interna. La regla alcanza a todo lo que forma parte del código: variables, funciones, clases, tablas, rutas de la API, archivos de código y carpetas del repositorio.

La razón de no escribir comentarios: los nombres deben bastar para entender el código, y la explicación vive en un solo lugar, `docs/`, para que no haya dos versiones que mantener.

La explicación del proyecto está escrita en español. Por eso los documentos sí tienen nombre en español (por ejemplo `docs/01_PLANTEAMIENTO_PROBLEMA.md`): son documentación, no código. Los textos que ve el usuario en la interfaz también están en español.

## Organización

```text
ARQUILA/
├── backend/             API en Python (FastAPI) y estructuras de datos
├── frontend/            Interfaz en TypeScript (React)
├── database/            Migraciones y datos de ejemplo
├── docs/                Documentación en español
├── prompts/             Instrucciones dadas al asistente de IA
├── .agents/             Reglas del asistente
├── .github/             Comprobaciones automáticas y plantilla de pull request
├── scripts/             Comando de calidad del código
└── CHANGELOG.md         Cambios agrupados por semana
```

## Requisitos

- Python 3.12 con `pip`.
- Node 24 con `npm`.
- PostgreSQL 16 o superior.

## Inicio rápido

Los pasos van en este orden: base de datos, backend, datos de ejemplo y frontend. Las secciones siguientes explican cada uno.

1. Base de datos. Crear una base vacía llamada `arquila`:

   ```bash
   psql -U postgres -c "CREATE DATABASE arquila;"
   ```

2. Backend. En Windows:

   ```bash
   cd backend
   python -m venv .venv
   .venv\Scripts\activate
   pip install -r requirements-dev.txt
   ```

   Copiar `backend/.env.example` a `backend/.env` y poner en `DATABASE_URL` la contraseña de PostgreSQL en lugar de `CHANGE_ME`.

3. Datos de ejemplo (opcional), con el entorno virtual activado y desde `backend/`:

   ```bash
   python -m app.migrate --seed
   ```

   Después, arrancar el backend:

   ```bash
   python -m app.dev
   ```

4. Frontend, en otra terminal:

   ```bash
   cd frontend
   npm install
   npm run dev
   ```

La interfaz queda en `http://localhost:5173`.

## Ejecución del backend

En Windows:

```bash
cd backend
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements-dev.txt
```

Si PowerShell no deja activar el entorno virtual porque la ejecución de scripts está deshabilitada, permitirla solo para esa terminal y activar de nuevo:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy RemoteSigned
.venv\Scripts\Activate.ps1
```

En Linux o macOS solo cambia la activación del entorno virtual:

```bash
cd backend
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements-dev.txt
```

En Windows conviene clonar el proyecto en una ruta corta: en una carpeta con una ruta muy larga, `pip install` puede fallar.

La configuración se guarda en `backend/.env`, que no se sube al repositorio. La primera vez:

1. Copiar `backend/.env.example` a `backend/.env`.
2. Poner en `DATABASE_URL` la conexión a PostgreSQL, por ejemplo `postgresql+psycopg://postgres:CLAVE@localhost:5432/arquila`. La base `arquila` debe existir.
3. Cargar los datos de ejemplo, si se quieren: `python -m app.migrate --seed`. Sin `--seed`, el comando solo crea las tablas y no crea ningún usuario.

Después, para arrancar el backend basta con:

```bash
python -m app.dev
```

Ese comando lee `backend/.env`, crea o actualiza las tablas con las migraciones pendientes y arranca el servidor en el puerto 8000. Ver `docs/07_BASE_DATOS.md`.

`DATABASE_URL` se define en un solo lugar, `backend/.env`, y la usan el arranque (`app.dev`), las migraciones y los datos de ejemplo (`app.migrate`) y la comprobación (`app.check`). Las demás variables están en `backend/.env.example` y se explican en `docs/12_BACKEND_Y_API.md`.

Con el backend arrancado:

- `http://localhost:8000/docs` muestra la documentación interactiva de la API, que FastAPI genera a partir del código. Desde ahí se puede probar cada operación.
- `http://localhost:8000/health` responde `{"status": "ok"}` si el servidor está en marcha.
- La terminal muestra una línea en formato JSON por cada petición (ver «Registro de eventos» en `docs/12_BACKEND_Y_API.md`).

## Ejecución del frontend

```bash
cd frontend
npm install
npm run dev
```

La interfaz queda en `http://localhost:5173` y se comunica con el backend en el puerto 8000.

La dirección de la API viene de la variable `VITE_API_URL`. Por defecto vale `/api`, que Vite reenvía al backend, así que en desarrollo no hay que definirla. Para apuntar a otro backend, copiar `frontend/.env.example` a `frontend/.env` y cambiar su valor.

Si se cargaron los datos de ejemplo, se puede entrar con el usuario de demostración: `demo@example.com`, contraseña `arquila-demo`. Es una credencial pública, solo para desarrollo.

## Asistente de IA

El asistente funciona sin configurar nada: si no hay ninguna IA disponible, responde con reglas fijas sobre los datos del proyecto. Para que responda una IA hay dos opciones:

- **Modelo local, gratuito y sin clave.** Instalar [Ollama](https://ollama.com) y ejecutar `ollama pull llama3.2`. El backend lo detecta solo.
- **Claude, de pago.** Escribir `ANTHROPIC_API_KEY` en `backend/.env`. En ese caso los datos del proyecto y las preguntas se envían a Anthropic, un tercero (ver «Privacidad» en `docs/12_BACKEND_Y_API.md`).

Para comprobar qué responde y si funciona:

```bash
cd backend
python -m app.check
```

## Seguridad

- Las contraseñas se guardan con Argon2id (`backend/app/security.py`).
- La sesión dura 7 días y viaja en una cookie `HttpOnly`; en la base solo se guarda su hash (`backend/app/services/auth_service.py`).
- Cinco intentos fallidos de inicio de sesión por correo en un minuto bloquean el acceso (`backend/app/services/login_limiter.py`).
- La API solo admite peticiones del origen del frontend, definido con `APP_URL` o `CORS_ORIGINS`, y añade cabeceras de seguridad a todas las respuestas (`backend/app/main.py`).
- El registro de eventos no incluye contraseñas ni correos (`backend/app/logs.py`).
- `backend/.env` no se sube al repositorio y `backend/.env.example` no contiene valores reales.

El detalle y las limitaciones conocidas están en la sección «Seguridad» de `docs/12_BACKEND_Y_API.md`.

## Pruebas

```bash
cd backend
pytest
```

```bash
cd frontend
npm test
npm run build
```

`npm run build` también comprueba los tipos de TypeScript. El formato y las reglas de estilo se revisan aparte, con `python scripts/quality.py` (ver «Calidad del código»).

Para medir los tiempos de las estructuras:

```bash
cd backend
python -m app.benchmark
```

GitHub ejecuta estas mismas comprobaciones en cada subida a `main` y en cada pull request (`.github/workflows/ci.yml`): formato, backend con SQLite y con PostgreSQL, y frontend. Lo que cubren las pruebas y lo que no está en `docs/06_PRUEBAS.md`.

## Calidad del código

Un solo comando revisa el formato y los errores comunes de todo el repositorio. Se ejecuta desde la raíz, con el entorno virtual del backend activado y las dependencias del frontend instaladas:

```bash
python scripts/quality.py
```

Con `--fix` corrige lo que se puede corregir solo:

```bash
python scripts/quality.py --fix
```

| Lenguaje | Herramientas | Configuración |
|---|---|---|
| Python | Ruff (formato y reglas) | `ruff.toml` |
| TypeScript | Prettier (formato) y ESLint (reglas) | `frontend/.prettierrc.json`, `frontend/eslint.config.js` |

Todas se instalan con `pip install -r requirements-dev.txt` y `npm install`; no hay que instalar nada aparte.

Para que el formato se aplique solo antes de cada commit, activar los hooks una vez:

```bash
pre-commit install
```

Las versiones de las dependencias están fijadas: exactas en `backend/requirements.txt`, `backend/requirements-dev.txt` y `frontend/package.json`, y con todo el árbol en `frontend/package-lock.json`.

## Uso de herramientas de IA

Durante el desarrollo se usó un asistente de IA. Las instrucciones que se le dieron están en `prompts/` (índice en `prompts/README.md`) y sus reglas de trabajo en `.agents/claude_solver.md`.

[Completar según la política del curso]

## Documentación

| Documento | Contenido |
|---|---|
| `docs/01_PLANTEAMIENTO_PROBLEMA.md` | Qué problema resuelve el proyecto. |
| `docs/02_REQUERIMIENTOS.md` | Requerimientos y dónde se cumple cada uno. |
| `docs/03_ARQUITECTURA.md` | Partes del sistema y cómo se comunican, con su diagrama. |
| `docs/04_ESTRUCTURAS_DATOS.md` | Las estructuras estudiadas y dónde están implementadas. |
| `docs/05_COMPLEJIDAD.md` | Complejidad de cada operación, uso de las estructuras en la aplicación y mediciones de tiempo. |
| `docs/06_PRUEBAS.md` | Qué cubren las pruebas, cómo ejecutarlas y qué hace la integración continua. |
| `docs/07_BASE_DATOS.md` | Base de datos, diagrama entidad-relación, migraciones y datos de ejemplo. |
| `docs/08_GIT_Y_TRABAJO_EN_EQUIPO.md` | Flujo de trabajo con Git, convención de commits, hooks y pull requests. |
| `docs/09_GUIA_DEFENSA.md` | Preguntas de la defensa con sus respuestas. |
| `docs/10_CONCLUSIONES.md` | Conclusiones. |
| `docs/11_PLAN_TRABAJO.md` | Fases del trabajo y su estado. |
| `docs/12_BACKEND_Y_API.md` | Decisiones del backend y del frontend: autenticación, seguridad, registro de eventos, archivos, terreno, IA y accesibilidad. |
| `docs/13_EVOLUCION_POR_SEMANAS.md` | Evolución de la aplicación y estado real de cada semana. |
| `docs/14_GUION_DEMO.md` | Recorrido paso a paso para la demostración: qué hacer, qué estructura se activa y qué decir. |
| `CHANGELOG.md` | Cambios agrupados por semana y etiquetas de Git propuestas. |
| `prompts/README.md` | Índice de los prompts usados con el asistente de IA. |
