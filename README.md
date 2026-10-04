# ARQUILA

Proyecto académico de cuarto semestre de Ingeniería de Software.

ARQUILA es una aplicación web para organizar proyectos de arquitectura: reúne en un mismo lugar el terreno, los cuartos, los materiales, los componentes estructurales y un modelo 3D de cada proyecto, con un asistente de IA que responde sobre esos datos.

## Objetivo

Aplicar los conceptos de estructuras de datos vistos en clase dentro de un proyecto de software organizado.

## Alcance académico

Las estructuras estudiadas están implementadas dos veces: en C++, como plantillas independientes con sus propias pruebas, y en Python, dentro del backend, donde la aplicación las usa.

| Estructura | Uso en la aplicación | Dónde está el uso |
|---|---|---|
| Array unidimensional | Cálculo del área de un lote recorriendo sus vértices. | `backend/app/services/geometry.py` |
| Array dinámico | Orden de los materiales por costo para el asistente. | `backend/app/services/material_ranking.py` |
| Stack (LIFO) | «Rehacer» una eliminación deshecha. | `backend/app/services/undo_history.py` |
| Queue (FIFO) | Límite de intentos fallidos de inicio de sesión. | `backend/app/services/login_limiter.py` |
| Lista simplemente enlazada | Ventana con los últimos 20 mensajes enviados a la IA. | `backend/app/services/conversation_context.py` |
| Lista doblemente enlazada | Historial de eliminaciones para «Deshacer». | `backend/app/services/undo_history.py` |

Las implementaciones en Python están en `backend/app/data_structures/` y las de C++ en `data_structures/`. Están escritas a mano, sin `collections.deque` ni otra librería que las reemplace.

El detalle está en `docs/04_ESTRUCTURAS_DATOS.md`, la complejidad de cada operación y las mediciones de tiempo en `docs/05_COMPLEJIDAD.md`, y el recorrido para mostrar cada estructura en la aplicación en `docs/14_GUION_DEMO.md`.

## Tecnologías

- C++17 para las implementaciones académicas de estructuras de datos, con CMake para compilar y Catch2 para las pruebas.
- Python y FastAPI para el backend, con SQLAlchemy y PostgreSQL, Argon2 para las contraseñas y pytest para las pruebas.
- TypeScript, React y Vite para el frontend, con Three.js para el modelo 3D, y Vitest con Testing Library para las pruebas.
- Ruff, ESLint, Prettier y clang-format para el formato y las reglas de estilo.
- Git para control de versiones y GitHub Actions para ejecutar las comprobaciones en cada subida.

## Regla de código

Los identificadores del código están en inglés y el código no contiene comentarios ni documentación interna. La regla alcanza a todo lo que forma parte del código: variables, funciones, clases, tablas, rutas de la API, archivos de código y carpetas del repositorio.

La explicación del proyecto está separada del código y está escrita en español. Por eso los documentos sí tienen nombre en español (`docs/01_PLANTEAMIENTO_PROBLEMA.md`, `PLAN_MEJORAS.md`): son documentación, no código. Los textos que ve el usuario en la interfaz también están en español.

## Organización

```text
ARQUILA/
├── data_structures/     Estructuras de datos en C++
├── backend/             API en Python (FastAPI)
├── frontend/            Interfaz en TypeScript (React)
├── database/            Migraciones y datos de ejemplo
├── docs/                Documentación en español
├── prompts/             Instrucciones dadas al asistente de IA
├── .agents/             Reglas del asistente
├── .github/             Comprobaciones automáticas y plantilla de pull request
├── scripts/             Comando de calidad del código
└── CHANGELOG.md         Cambios agrupados por semana
```

## Qué se añadió y cómo está implementado

Además de la aplicación en sí, el proyecto pasó por dos rondas de mejoras (`PLAN_MEJORAS.md` y `PLAN_MEJORAS_2.md`). Esta tabla resume cada una, dónde está en el repositorio y cómo funciona. El historial completo está en `CHANGELOG.md`.

### Estructuras de datos

| Qué | Cómo está implementado |
|---|---|
| Las seis estructuras se usan en la aplicación | Cada una resuelve una necesidad concreta de un servicio del backend (tabla de «Alcance académico»). La pila guarda lo que se deshizo para poder rehacerlo; la lista simple mantiene la ventana de mensajes agregando al final y quitando del inicio; el array dinámico ordena los materiales insertando cada uno en su posición. |
| Estructuras de C++ como plantillas | Cada estructura es un `template <typename T>` en un `.hpp` (`data_structures/stack/Stack.hpp`, etc.), con un programa de ejemplo en el `.cpp` del mismo nombre. Sirven para `int`, `std::string` o cualquier tipo que se pueda copiar y comparar. |
| Compilación con CMake | `data_structures/CMakeLists.txt` compila los cinco programas y las pruebas; `CMakePresets.json` define el comando único `cmake --workflow --preset default`. En Windows enlaza con `-static`. |
| Pruebas de C++ con Catch2 | Están en `data_structures/tests/`. CMake descarga Catch2 con `FetchContent`, sin instalar nada a mano. |
| Memoria sin fugas | Las listas liberan sus nodos en el destructor y no se pueden copiar. Una prueba cuenta los valores vivos y verifica que queda en cero, y GitHub ejecuta todo con AddressSanitizer, que falla ante una fuga o una doble liberación. |
| Mediciones de tiempo | `backend/app/benchmark.py` mide cada operación con 1 000, 10 000 y 100 000 elementos (`python -m app.benchmark`). Los resultados, comparados con la complejidad teórica, están en `docs/05_COMPLEJIDAD.md`. |

### Backend y seguridad

| Qué | Cómo está implementado |
|---|---|
| Contraseñas con Argon2id | `backend/app/security.py` usa la librería `argon2-cffi`. Las cuentas antiguas, guardadas con `scrypt`, siguen funcionando y se actualizan solas en el primer inicio de sesión correcto. |
| Sesiones que caducan | La sesión dura 7 días y viaja en una cookie `HttpOnly`; en la base solo se guarda su hash (`backend/app/services/auth_service.py`). |
| Límite de intentos de inicio de sesión | Cinco fallos por correo en un minuto bloquean el acceso; lo controla la cola en `login_limiter.py`. |
| CORS restringido | Solo se admite el origen del frontend, definido con `APP_URL` o `CORS_ORIGINS` (`backend/app/main.py`). |
| Cabeceras de seguridad | Un middleware en `main.py` añade `nosniff`, `Referrer-Policy`, `Cache-Control: no-store` y `frame-ancestors` a todas las respuestas. |
| Registro de eventos en JSON | `backend/app/logs.py` escribe una línea JSON por petición, con método, ruta, código y duración, y avisos de inicio de sesión fallido o de respaldo de la IA. Nunca registra contraseñas ni correos. |
| Una sola configuración | `backend/app/env.py` carga `backend/.env`, y lo usan el arranque (`app.dev`), las migraciones y los datos de ejemplo (`app.migrate`) y la comprobación (`app.check`). |
| Asistente de IA robusto | Los adaptadores de `backend/app/ai.py` tienen tiempo máximo de espera y convierten cualquier fallo en un error propio; si la IA no responde, contestan las reglas. Las pruebas usan clientes simulados, sin red ni gasto de tokens. |

### Frontend

| Qué | Cómo está implementado |
|---|---|
| Estados de carga y de error | Cada lectura pasa por el hook `useAsync` y se muestra con `AsyncStatus`: «Cargando…», lista vacía o el error con un botón «Reintentar». Cada escritura muestra «Guardando…» y no se envía dos veces. |
| Diseño para pantallas pequeñas | Por debajo de 720 px el menú pasa a ser una fila desplazable y las tablas se muestran como fichas (`frontend/src/styles.css`). Comprobado a 390, 820 y 1280 px. |
| Accesibilidad básica | Todos los campos tienen etiqueta, el contraste cumple WCAG AA en los dos temas, hay un enlace «Saltar al contenido» y toda la aplicación se puede usar con el teclado. |
| Dirección de la API configurable | Viene de `VITE_API_URL`, con valor de ejemplo en `frontend/.env.example`. |
| Pruebas de componentes | Con Testing Library, en archivos `*.test.tsx` junto a cada componente: estados de carga, menú, inicio de sesión y lista de proyectos. |

### Calidad y flujo de trabajo

| Qué | Cómo está implementado |
|---|---|
| Un comando para el formato y las reglas | `scripts/quality.py` ejecuta Ruff, Prettier, ESLint y clang-format; con `--fix` corrige. |
| Hooks antes de cada commit | `.pre-commit-config.yaml` aplica los formateadores a los archivos modificados; se activa con `pre-commit install`. |
| Integración continua | `.github/workflows/ci.yml` ejecuta cuatro trabajos en cada subida: formato, estructuras en C++ con AddressSanitizer, backend con SQLite y con PostgreSQL, y frontend. |
| Convención de commits y pull requests | Conventional Commits, documentada en `docs/08_GIT_Y_TRABAJO_EN_EQUIPO.md`, y plantilla en `.github/pull_request_template.md`. |
| Versiones fijadas | Versiones exactas en `backend/requirements*.txt` y `frontend/package.json`, más `frontend/package-lock.json`. |

### Documentación y defensa

| Qué | Cómo está implementado |
|---|---|
| Diagramas | Arquitectura en `docs/03_ARQUITECTURA.md` y entidad-relación en `docs/07_BASE_DATOS.md`, escritos en Mermaid: GitHub los dibuja al abrir el documento. |
| Guion de la demostración | `docs/14_GUION_DEMO.md`: ocho pasos con qué hacer, qué estructura se activa y qué decir. |
| Historial de cambios | `CHANGELOG.md`, agrupado por las semanas del plan, con las etiquetas de Git propuestas. |

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

En Windows:

```bash
cd backend
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements-dev.txt
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

```bash
cd data_structures
cmake --workflow --preset default
```

`npm run build` también comprueba los tipos de TypeScript. El formato y las reglas de estilo se revisan aparte, con `python scripts/quality.py` (ver «Calidad del código»).

Para medir los tiempos de las estructuras:

```bash
cd backend
python -m app.benchmark
```

GitHub ejecuta estas mismas comprobaciones, y además las pruebas de las estructuras en C++, en cada subida a `santiago` o a `main` (`.github/workflows/ci.yml`). Lo que cubren las pruebas y lo que no está en `docs/06_PRUEBAS.md`.

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
| C++ | clang-format (formato) | `data_structures/.clang-format` |

Todas se instalan con `pip install -r requirements-dev.txt` y `npm install`; no hay que instalar nada aparte.

Para que el formato se aplique solo antes de cada commit, activar los hooks una vez:

```bash
pre-commit install
```

Las versiones de las dependencias están fijadas: exactas en `backend/requirements.txt`, `backend/requirements-dev.txt` y `frontend/package.json`, y con todo el árbol en `frontend/package-lock.json`.

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
