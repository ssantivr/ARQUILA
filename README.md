# ARQUILA PROJECT

Proyecto académico de cuarto semestre de Ingeniería de Software.

## Objetivo

Aplicar los conceptos de estructuras de datos vistos en clase dentro de un proyecto de software organizado.

## Alcance académico

- Arrays unidimensionales.
- Arrays dinámicos.
- Stack con LIFO.
- Queue con FIFO.
- Lista simplemente enlazada.
- Lista doblemente enlazada.

## Tecnologías

- C++ para las implementaciones académicas de estructuras de datos.
- Python y FastAPI para el backend, con SQLAlchemy y PostgreSQL.
- TypeScript, React y Vite para el frontend.
- Git para control de versiones.

## Regla de código

Los identificadores del código están en inglés y el código no contiene comentarios ni documentación interna.

La explicación del proyecto está separada del código y está escrita en español.

## Organización

```text
ARQUILA/
├── data_structures/   Estructuras de datos en C++
├── backend/           API en Python (FastAPI)
├── frontend/          Interfaz en TypeScript (React)
├── database/          Esquema y datos de ejemplo
├── docs/              Documentación en español
└── .agentes/          Reglas del asistente
```

## Principio de diseño

El proyecto mantiene una complejidad adecuada para cuarto semestre. Se prioriza comprender y defender correctamente las estructuras estudiadas antes de incorporar estructuras más avanzadas.

## Ejecución de una estructura

```bash
g++ -std=c++17 data_structures/stack/Stack.cpp -o stack
./stack
```

## Ejecución del backend

```bash
cd backend
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements-dev.txt
```

La configuración se guarda en `backend/.env`, que no se sube al repositorio. La primera vez:

1. Copiar `backend/.env.example` a `backend/.env`.
2. Poner en `DATABASE_URL` la conexión a PostgreSQL, por ejemplo `postgresql+psycopg://postgres:CLAVE@localhost:5432/arquila`. La base `arquila` debe existir.
3. Cargar los datos de ejemplo, si se quieren: `python -m app.migrate --seed`, con `DATABASE_URL` definida en la terminal.

Después, para arrancar el backend basta con:

```bash
python -m app.dev
```

Ese comando lee `backend/.env`, crea o actualiza las tablas con las migraciones pendientes y arranca el servidor en el puerto 8000. Ver `docs/07_BASE_DATOS.md`.

Las demás variables están en `backend/.env.example` y se explican en `docs/10_BACKEND_Y_API.md`.

## Ejecución del frontend

```bash
cd frontend
npm install
npm run dev
```

La interfaz queda en `http://localhost:5173` y se comunica con el backend en el puerto 8000.

## Pruebas

```bash
cd backend
pytest
```

```bash
cd frontend
npm run build
```

## Documentación

- `docs/05_COMPLEJIDAD.md`: complejidad de cada operación de las estructuras.
- `docs/10_BACKEND_Y_API.md`: decisiones del backend, autenticación, archivos e IA.
- `06_EVOLUCION_POR_SEMANAS.md`: evolución del proyecto y estado real de implementación.
