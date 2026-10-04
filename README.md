# ARQUILA

Proyecto académico de cuarto semestre de Ingeniería de Software.

ARQUILA es una aplicación web para organizar proyectos de arquitectura: reúne en un mismo lugar el terreno, los cuartos, los materiales, los componentes estructurales y un modelo 3D de cada proyecto, con un asistente de IA que responde sobre esos datos.

## Objetivo

Aplicar los conceptos de estructuras de datos vistos en clase dentro de un proyecto de software organizado.

## Alcance académico

Las estructuras estudiadas están implementadas dos veces: en C++, como ejercicio académico independiente, y en Python, dentro del backend, donde la aplicación las usa.

| Estructura | Uso en la aplicación |
|---|---|
| Array unidimensional | Cálculo del área de un lote recorriendo sus vértices. |
| Array dinámico | Orden de los materiales por precio para el asistente. |
| Stack (LIFO) | «Rehacer» una eliminación deshecha. |
| Queue (FIFO) | Límite de intentos fallidos de inicio de sesión. |
| Lista simplemente enlazada | Ventana con los últimos mensajes enviados a la IA. |
| Lista doblemente enlazada | Historial de eliminaciones para «Deshacer». |

El detalle está en `docs/04_ESTRUCTURAS_DATOS.md` y la complejidad de cada operación en `docs/05_COMPLEJIDAD.md`.

## Tecnologías

- C++ para las implementaciones académicas de estructuras de datos.
- Python y FastAPI para el backend, con SQLAlchemy y PostgreSQL.
- TypeScript, React y Vite para el frontend, con Three.js para el modelo 3D.
- Git para control de versiones.

## Regla de código

Los identificadores del código están en inglés y el código no contiene comentarios ni documentación interna.

La explicación del proyecto está separada del código y está escrita en español.

## Organización

```text
ARQUILA/
├── data_structures/     Estructuras de datos en C++
├── backend/             API en Python (FastAPI)
├── frontend/            Interfaz en TypeScript (React)
├── database/            Migraciones y datos de ejemplo
├── docs/                Documentación en español
├── prompts/             Instrucciones dadas al asistente de IA
├── .agentes/            Reglas del asistente
├── .github/             Comprobaciones automáticas en GitHub
└── docker-compose.yml   PostgreSQL en un contenedor (opcional)
```

## Principio de diseño

El proyecto mantiene una complejidad adecuada para cuarto semestre. Se prioriza comprender y defender correctamente las estructuras estudiadas antes de incorporar estructuras más avanzadas.

## Requisitos

- Python 3 con `pip`.
- Node.js con `npm`.
- PostgreSQL, con una base de datos vacía llamada `arquila`.
- Un compilador de C++17 (`g++`), CMake 3.25 o posterior y Ninja, solo para las estructuras en C++.

La instalación desde cero está comprobada en Windows 11 con Python 3.12 y Node 24, y las pruebas pasan con PostgreSQL 16 y 18 (ver `docs/06_PRUEBAS.md`).

## Estructuras en C++

Se compilan con CMake (3.25 o posterior) y Ninja. Un solo comando configura, compila los cinco programas y ejecuta las pruebas:

```bash
cd data_structures
cmake --workflow --preset default
```

Los ejecutables quedan en `data_structures/build/`, por ejemplo `build/Stack`. La primera vez CMake descarga Catch2, la librería de pruebas, así que necesita conexión a internet.

En Linux o macOS, `cmake --workflow --preset sanitize` hace lo mismo con AddressSanitizer, que detecta fugas de memoria y accesos inválidos. GCC para Windows no lo incluye.

En Windows, CMake enlaza con `-static`. Sin esa opción, un programa puede cerrarse sin escribir nada si otro programa instalado (por ejemplo PostgreSQL) tiene en el `PATH` una versión distinta de las bibliotecas de GCC. Con `-static` el ejecutable no depende de ellas.

Un programa suelto también se puede compilar sin CMake:

```bash
g++ -std=c++17 -static data_structures/stack/Stack.cpp -o stack
./stack
```

## Ejecución del backend

```bash
cd backend
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements-dev.txt
```

En Linux o macOS el entorno se activa con `source .venv/bin/activate`.

En Windows conviene clonar el proyecto en una ruta corta: en una carpeta con una ruta muy larga, `pip install` puede fallar.

La configuración se guarda en `backend/.env`, que no se sube al repositorio. La primera vez:

1. Copiar `backend/.env.example` a `backend/.env`.
2. Poner en `DATABASE_URL` la conexión a PostgreSQL, por ejemplo `postgresql+psycopg://postgres:CLAVE@localhost:5432/arquila`. La base `arquila` debe existir.
3. Cargar los datos de ejemplo, si se quieren: `python -m app.migrate --seed`. Lee la conexión de `backend/.env`, igual que el arranque.

Después, para arrancar el backend basta con:

```bash
python -m app.dev
```

Ese comando lee `backend/.env`, crea o actualiza las tablas con las migraciones pendientes y arranca el servidor en el puerto 8000. Ver `docs/07_BASE_DATOS.md`.

`DATABASE_URL` se define en un solo lugar, `backend/.env`, y la usan tanto el arranque como las migraciones y los datos de ejemplo. Las demás variables están en `backend/.env.example` y se explican en `docs/12_BACKEND_Y_API.md`.

Con el backend arrancado:

- `http://localhost:8000/docs` muestra la documentación interactiva de la API, que FastAPI genera a partir del código. Desde ahí se puede probar cada operación.
- `http://localhost:8000/health` responde `{"status": "ok"}` si el servidor está en marcha.
- La terminal muestra una línea en formato JSON por cada petición (ver «Registro de eventos» en `docs/12_BACKEND_Y_API.md`).

### PostgreSQL con Docker (opcional)

Quien no tenga PostgreSQL instalado puede levantarlo con Docker desde la raíz del repositorio:

```bash
docker compose up -d
```

El contenedor crea la base `arquila` con usuario y contraseña `arquila`, así que el valor de `DATABASE_URL` en `backend/.env` es `postgresql+psycopg://arquila:arquila@localhost:5432/arquila`, el mismo que trae `backend/.env.example`. Este arranque no está comprobado, porque el equipo de desarrollo no tiene Docker.

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

Ver `docs/12_BACKEND_Y_API.md`.

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

`npm run build` también comprueba los tipos de TypeScript.

GitHub ejecuta estas mismas comprobaciones, y además las pruebas de las estructuras en C++, en cada subida a `santiago` o a `main` (`.github/workflows/ci.yml`). Lo que cubren las pruebas y lo que no está en `docs/06_PRUEBAS.md`.

## Documentación

| Documento | Contenido |
|---|---|
| `docs/01_PLANTEAMIENTO_PROBLEMA.md` | Qué problema resuelve el proyecto. |
| `docs/02_REQUERIMIENTOS.md` | Requerimientos y dónde se cumple cada uno. |
| `docs/03_ARQUITECTURA.md` | Partes del sistema y cómo se comunican. |
| `docs/04_ESTRUCTURAS_DATOS.md` | Las estructuras estudiadas y dónde están implementadas. |
| `docs/05_COMPLEJIDAD.md` | Complejidad de cada operación y uso de las estructuras en la aplicación. |
| `docs/06_PRUEBAS.md` | Qué cubren las pruebas y cómo ejecutarlas. |
| `docs/07_BASE_DATOS.md` | Base de datos, migraciones y datos de ejemplo. |
| `docs/08_GIT_Y_TRABAJO_EN_EQUIPO.md` | Flujo de trabajo con Git. |
| `docs/09_GUIA_DEFENSA.md` | Preguntas de la defensa con sus respuestas. |
| `docs/10_CONCLUSIONES.md` | Conclusiones. |
| `docs/11_PLAN_TRABAJO.md` | Fases del trabajo y su estado. |
| `docs/12_BACKEND_Y_API.md` | Decisiones del backend: autenticación, seguridad, archivos, terreno e IA. |
| `docs/13_EVOLUCION_POR_SEMANAS.md` | Evolución de la aplicación y estado real de cada semana. |
| `prompts/README.md` | Índice de los prompts usados con el asistente de IA. |
