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

## Arquitectura

El proyecto está dividido en tres repositorios independientes. Este repositorio los coordina: guarda la documentación general, las reglas de trabajo y el historial del proyecto, y no contiene código de la aplicación.

```text
ARQUILA                     Documentación y coordinación (este repositorio)
├── FRONTEND-ARQUILA        Interfaz en TypeScript (React, Vite, Three.js)
├── BACKEND-ARQUILA         API en Python (FastAPI) y estructuras de datos
└── BASE-DE-DATOS-ARQUILA   Migraciones y datos de ejemplo (PostgreSQL)
```

| Repositorio | Contenido | Enlace |
|---|---|---|
| `FRONTEND-ARQUILA` | Pantallas, componentes, escenas 3D y cliente de la API. | <https://github.com/ssantivr/FRONTEND-ARQUILA> |
| `BACKEND-ARQUILA` | Rutas, servicios, modelos, autenticación y las estructuras de datos. | <https://github.com/ssantivr/BACKEND-ARQUILA> |
| `BASE-DE-DATOS-ARQUILA` | Esquema en migraciones SQL numeradas, datos de ejemplo y script de creación. | <https://github.com/ssantivr/BASE-DE-DATOS-ARQUILA> |

Cómo se relacionan:

- El **frontend** llama a la API del backend por HTTP. La dirección sale de la variable `VITE_API_URL` y la sesión viaja en una cookie `HttpOnly`.
- El **backend** se conecta a PostgreSQL con `DATABASE_URL` y crea las tablas con las migraciones del repositorio de **base de datos**, que busca en la carpeta hermana o en la que indique `DATABASE_DIR`.
- La **base de datos** no depende de los otros dos: su esquema se puede crear solo con `psql`.

El detalle está en `docs/03_ARQUITECTURA.md`. En la documentación, una ruta como `BACKEND-ARQUILA/app/main.py` nombra un archivo del repositorio `BACKEND-ARQUILA`.

## Objetivo

Aplicar los conceptos de estructuras de datos vistos en clase dentro de un proyecto de software organizado.

## Alcance académico

Las estructuras estudiadas están implementadas desde cero en Python, en `BACKEND-ARQUILA/app/data_structures/`, sin librerías que las reemplacen.

| Estructura | Uso en la aplicación | Dónde está el uso |
|---|---|---|
| Array unidimensional | Cálculo del área de un lote recorriendo sus vértices. | `BACKEND-ARQUILA/app/services/geometry.py` |
| Array dinámico | Orden de los materiales por costo para el asistente. | `BACKEND-ARQUILA/app/services/material_ranking.py` |
| Stack (LIFO) | «Rehacer» una eliminación deshecha. | `BACKEND-ARQUILA/app/services/undo_history.py` |
| Queue (FIFO) | Límite de intentos fallidos de inicio de sesión. | `BACKEND-ARQUILA/app/services/login_limiter.py` |
| Lista simplemente enlazada | Ventana con los últimos 20 mensajes enviados a la IA. | `BACKEND-ARQUILA/app/services/conversation_context.py` |
| Lista doblemente enlazada | Historial de eliminaciones para «Deshacer». | `BACKEND-ARQUILA/app/services/undo_history.py` |

El detalle está en `docs/04_ESTRUCTURAS_DATOS.md`, la complejidad de cada operación y las mediciones de tiempo en `docs/05_COMPLEJIDAD.md`, y el recorrido para mostrar cada estructura en la aplicación en `docs/14_GUION_DEMO.md`.

## Criterios de diseño

- **Qué quedó fuera.** No se implementaron árboles, grafos ni tablas hash: no se vieron en clase. El proyecto se limita a las seis estructuras de la tabla anterior.
- **Qué se hizo a mano.** Las seis estructuras y sus operaciones (`BACKEND-ARQUILA/app/data_structures/`), la búsqueda lineal y la binaria, el orden de los materiales por inserción en un array dinámico y el cálculo del área de un lote. No se usa `collections.deque` ni otra librería que las reemplace.
- **Qué viene de librerías.** El servidor web (FastAPI), el acceso a la base de datos (SQLAlchemy y psycopg), el hash de contraseñas (argon2-cffi), la interfaz (React), el modelo 3D (Three.js) y las herramientas de pruebas y de formato.

## Tecnologías

- Python y FastAPI para el backend, con SQLAlchemy y PostgreSQL, Argon2 para las contraseñas y pytest para las pruebas.
- TypeScript, React y Vite para el frontend, con Three.js para el modelo 3D, y Vitest con Testing Library para las pruebas.
- Ruff, ESLint y Prettier para el formato y las reglas de estilo.
- Git para control de versiones y GitHub Actions para ejecutar las comprobaciones.

## Regla de código

Los identificadores del código están en inglés y el código no contiene comentarios ni documentación interna. La regla alcanza a todo lo que forma parte del código: variables, funciones, clases, tablas, rutas de la API, archivos de código y carpetas.

La razón de no escribir comentarios: los nombres deben bastar para entender el código, y la explicación vive en la documentación, para que no haya dos versiones que mantener.

La explicación del proyecto está escrita en español. Por eso los documentos sí tienen nombre en español (por ejemplo `docs/01_PLANTEAMIENTO_PROBLEMA.md`): son documentación, no código. Los textos que ve el usuario en la interfaz también están en español.

## Organización de este repositorio

```text
ARQUILA/
├── docs/                Documentación general en español
├── prompts/             Instrucciones dadas al asistente de IA
├── .agents/             Reglas del asistente
├── .github/             Plantilla de pull request
├── scripts/             Comando de calidad de los tres repositorios
└── CHANGELOG.md         Cambios agrupados por semana
```

## Inicio rápido

Requisitos: Python 3.12, Node 24 y PostgreSQL 16 o superior.

1. Clonar los tres repositorios de código en la misma carpeta:

   ```bash
   git clone https://github.com/ssantivr/BASE-DE-DATOS-ARQUILA.git
   git clone https://github.com/ssantivr/BACKEND-ARQUILA.git
   git clone https://github.com/ssantivr/FRONTEND-ARQUILA.git
   ```

2. Crear una base vacía:

   ```bash
   psql -U postgres -c "CREATE DATABASE arquila;"
   ```

3. Backend. Instalar, copiar `.env.example` a `.env`, poner la contraseña de PostgreSQL en `DATABASE_URL` y arrancar:

   ```bash
   cd BACKEND-ARQUILA
   python -m venv .venv
   .venv\Scripts\activate
   pip install -r requirements-dev.txt
   python -m app.migrate --seed
   python -m app.dev
   ```

   `--seed` carga los datos de ejemplo y es opcional.

4. Frontend, en otra terminal:

   ```bash
   cd FRONTEND-ARQUILA
   npm install
   npm run dev
   ```

La interfaz queda en `http://localhost:5173` y la API en `http://localhost:8000`. Si se cargaron los datos de ejemplo, se puede entrar con `demo@example.com`, contraseña `arquila-demo`: es una credencial pública, solo para desarrollo.

Las variables de entorno, los comandos y las pruebas de cada parte están en el `README.md` de su repositorio.

## Calidad del código

Con los tres repositorios clonados en la misma carpeta que este, el entorno virtual del backend activado y las dependencias del frontend instaladas, un solo comando revisa el formato y los errores comunes del backend y del frontend:

```bash
python scripts/quality.py
```

Con `--fix` corrige lo que se puede corregir solo. Usa Ruff para Python, y Prettier y ESLint para TypeScript, con la configuración de cada repositorio.

GitHub ejecuta las comprobaciones de cada repositorio de código en cada subida a `main` y en cada pull request. Lo que cubren las pruebas y lo que no está en `docs/06_PRUEBAS.md`.

## Uso de herramientas de IA

Durante el desarrollo se usó un asistente de IA. Las instrucciones que se le dieron están en `prompts/` (índice en `prompts/README.md`) y sus reglas de trabajo en `.agents/solver.md`.

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
| `docs/08_GIT_Y_TRABAJO_EN_EQUIPO.md` | Flujo de trabajo con Git, convención de commits, hooks y pull requests. |
| `docs/09_GUIA_DEFENSA.md` | Preguntas de la defensa con sus respuestas. |
| `docs/10_CONCLUSIONES.md` | Conclusiones. |
| `docs/11_PLAN_TRABAJO.md` | Fases del trabajo y su estado. |
| `docs/13_EVOLUCION_POR_SEMANAS.md` | Evolución de la aplicación y estado real de cada semana. |
| `docs/14_GUION_DEMO.md` | Recorrido paso a paso para la demostración: qué hacer, qué estructura se activa y qué decir. |
| `docs/15_ESTUDIO_PARA_LA_DEFENSA.md` | Resumen de cada estructura para estudiar y archivos que cada integrante debe poder explicar. |
| `docs/16_MEJORAS.md` | Qué se puede mejorar, ordenado por urgencia. |
| `CHANGELOG.md` | Cambios agrupados por semana y etiquetas de Git propuestas. |
| `prompts/README.md` | Índice de los prompts usados con el asistente de IA. |

La documentación específica de cada capa está en su repositorio, por eso faltan los números 07 y 12:

| Documento | Contenido |
|---|---|
| `BASE-DE-DATOS-ARQUILA/docs/BASE_DATOS.md` | Base de datos, diagrama entidad-relación, migraciones y datos de ejemplo. |
| `BACKEND-ARQUILA/docs/BACKEND_Y_API.md` | Decisiones del backend: autenticación, seguridad, registro de eventos, archivos e IA. |
| `FRONTEND-ARQUILA/docs/FRONTEND.md` | Decisiones de la interfaz: esquemas del terreno, planos, modelo 3D, estados de carga y accesibilidad. |
