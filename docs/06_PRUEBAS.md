# PRUEBAS

El proyecto utiliza pruebas sencillas porque el objetivo es validar los conceptos aprendidos y no construir una plataforma de testing avanzada.

## Casos

### Stack

- Insertar varios valores.
- Consultar el elemento superior.
- Retirar valores respetando LIFO.
- Comprobar estado vacío.

### Queue

- Insertar varios valores.
- Consultar el frente.
- Retirar valores respetando FIFO.
- Comprobar estado vacío.

### Lista simple

- Insertar al inicio.
- Insertar al final.
- Eliminar un valor.
- Recorrer la estructura.

### Lista doble

- Insertar al inicio.
- Insertar al final.
- Eliminar un valor.
- Recorrer hacia adelante.
- Recorrer hacia atrás.

### Casos borde de las estructuras

Los casos anteriores, y los de los arrays, están automatizados en `BACKEND-ARQUILA/tests/test_data_structures.py`. Además comprueban:

- `pop`, `peek` y `dequeue` en una estructura vacía, y `push` y `enqueue` en una llena.
- Que la cola mantiene el orden después de dar la vuelta al array.
- Las listas vacías, las de un solo elemento, y eliminar la cabeza, la cola y un nodo intermedio.
- Que en la lista doble los enlaces hacia atrás siguen siendo coherentes después de insertar y eliminar.
- Una lista de 1000 nodos después de eliminar la mitad.
- Que las estructuras guardan tipos distintos de `int` y que las búsquedas funcionan con textos.
- Que el array dinámico crece, desplaza los elementos y rechaza los índices fuera de rango.

`BACKEND-ARQUILA/tests/test_structure_usage.py` comprueba dos de sus usos en los servicios: la ventana de mensajes del asistente y el orden de los materiales por costo.

### Backend

Las pruebas automatizadas están en `BACKEND-ARQUILA/tests/` y se ejecutan con `pytest`. Por defecto usan una base SQLite en memoria, así que no necesitan PostgreSQL.

### Ejecución contra PostgreSQL

Las mismas pruebas se pueden ejecutar contra un PostgreSQL real definiendo `TEST_DATABASE_URL`:

```bash
set TEST_DATABASE_URL=postgresql+psycopg://usuario:clave@localhost:5432/arquila_test
pytest
```

En ese modo las tablas se crean aplicando las migraciones de `BASE-DE-DATOS-ARQUILA/migrations/`, de modo que también se comprueba que el esquema coincide con los modelos del backend.

Antes de cada prueba se vacían todas las tablas de esa base. Por seguridad, las pruebas se niegan a ejecutarse si el nombre de la base no termina en `test`. Nunca se debe apuntar a una base con datos reales.

Ejecutado el 4 de octubre de 2026 con PostgreSQL 18.6: las 285 pruebas pasan (ver «Resultados de la última ejecución»).

Cubren:

- El endpoint `/health`.
- Las estructuras de datos en Python: orden LIFO y FIFO, estructura vacía y llena, reutilización de posiciones en la cola circular, inserción y eliminación en las listas, y búsquedas.
- Registro, inicio y cierre de sesión, y que cada endpoint exija sesión.
- El resumen de las contraseñas (`test_security.py`): que se calcule con Argon2id, que solo verifique la contraseña correcta, que cambie en cada cálculo, que un valor guardado con otro formato nunca se dé por válido y que una contraseña antigua guardada con `scrypt` siga sirviendo y se actualice al iniciar sesión.
- El registro de eventos (`test_logs.py`): una línea JSON por petición, los avisos de inicio de sesión fallido y bloqueado, y que no aparezcan contraseñas, correos ni parámetros de la dirección.
- El comando de migraciones (`test_migrate.py`): que lea la conexión de `BACKEND-ARQUILA/.env`, cargue los datos de ejemplo y se detenga con un mensaje claro si falta `DATABASE_URL`.
- Que un usuario no pueda ver ni modificar los datos de otro.
- Crear, listar, actualizar y eliminar proyectos, terrenos, materiales, planos y elevaciones, con sus validaciones.
- Subida de archivos: tipos admitidos, tamaño máximo y adjuntarlos a planos y elevaciones.
- Recomendaciones automáticas, con la prioridad que asigna cada regla, y deshacer eliminaciones.
- Cuartos: crear, listar, actualizar y eliminar, que las medidas sean mayores que cero, que el plano sea del mismo proyecto y que se eliminen con su plano o su proyecto.
- Componentes estructurales: crear, listar y filtrar por tipo, actualizar y eliminar, que el tipo sea columna, viga o muro, que las medidas sean mayores que cero, que el plano sea del mismo proyecto y que se eliminen con su plano o su proyecto.
- El modelo 3D (`/projects/{id}/structure`): colocación de los terrenos, qué planos cuentan como nivel, cuartos frente a volumen del nivel, altura de cada nivel, y posición de los componentes, con las vigas colgadas del techo del nivel. También el material de superficie de cada elemento: que se guarde en un cuarto, en un componente y en el plano de un volumen, que se rechace un material o un tipo desconocido, que el elemento sea de ese tipo y de ese proyecto, y que otro usuario no pueda cambiarlo. Y el tipo de cubierta del proyecto: a dos aguas por defecto, que se guarde la plana, que se rechace otro valor y que otro usuario no pueda cambiarla.
- Proyectos de ejemplo: la lista, la creación de un proyecto completo desde un ejemplo, los materiales del ejemplo que los trae, el nombre libre al repetirlo, los permisos, y que los cuartos de cada ejemplo caben en su lote sin solaparse.
- CORS: que se admita el origen de la aplicación con credenciales, que se responda la petición previa y que se ignoren otros orígenes. También que todas las respuestas, incluidas las de error, lleven las cabeceras de seguridad (ver «Seguridad» en `BACKEND-ARQUILA/docs/BACKEND_Y_API.md`).
- Conversaciones con el asistente, usando un asistente simulado, y la respuesta por reglas cuando la IA falla o no está configurada.
- Los adaptadores de correo y de IA (`test_adapters.py`), sustituyendo `smtplib` por un objeto simulado: qué adaptador de correo se elige según la configuración y las llamadas SMTP que hace. El adaptador del modelo local se prueba contra un servidor de Ollama simulado: qué modelo elige, qué envía, y qué hace si Ollama no responde, no tiene modelos o contesta algo inesperado.

### Frontend

Las funciones de cálculo y de texto del frontend tienen pruebas en `FRONTEND-ARQUILA/src/utils/*.test.ts`, `FRONTEND-ARQUILA/src/state/*.test.ts`, `FRONTEND-ARQUILA/src/three/*.test.ts` y `FRONTEND-ARQUILA/src/services/*.test.ts`, que se ejecutan con `npm test`: área, vista frontal, curvas de nivel, cotas de cada lado, área edificable y lectura de los vértices de un lote, agrupación de cuartos por nivel e indicadores de ocupación (huella, área construida, COS y CUS), colocación de ventanas y puerta, forma del techo (a dos aguas o plano), fachadas y corte generados con cada cubierta, geometría de un cuarto en el modelo 3D (muros dentro de su medida, vidrio, hoja de puerta, líneas de cada vano y coordenadas de textura en metros), colores del modelo 3D (por tipo, por coste estimado y por alertas), materiales de superficie (el elegido o el predeterminado de cada tipo, y la lectura de los guardados), el estado compartido `appState` (avisos a los suscriptores, cambio de proyecto, material de superficie por elemento y recarga con peticiones simuladas), traducción de los mensajes de error y formato de números.

También están probados la colocación de puertas y ventanas y la forma del techo del modelo 3D (`openings.test.ts`), y el cliente HTTP (`http.test.ts`) con un `fetch` simulado: la dirección y el cuerpo de cada petición, los mensajes de error que devuelve el backend y el aviso de sesión terminada.

Los componentes principales tienen pruebas con Testing Library (`*.test.tsx`), que los dibujan en un navegador simulado (jsdom) con la API sustituida por funciones simuladas:

- `AsyncStatus.test.tsx`: los estados de carga, lista vacía y error, y el botón «Reintentar».
- `Sidebar.test.tsx`: todos los módulos del menú, el módulo actual y la navegación con ratón y solo con teclado.
- `LoginPage.test.tsx`: las etiquetas de los campos, el inicio de sesión (también solo con teclado), el botón bloqueado mientras se envía, el error traducido, el registro y la recuperación de contraseña.
- `ProjectsPage.test.tsx`: la carga y la lista de proyectos, abrir un proyecto, el error de carga con su reintento, y crear un proyecto con y sin error.

`npm test` ejecuta estas pruebas junto con las de cálculo. Las demás pantallas se revisaron a mano en un navegador automatizado (diseño en pantallas pequeñas, etiquetas y contraste; ver «Frontend» en `BACKEND-ARQUILA/docs/BACKEND_Y_API.md`), pero no tienen pruebas automáticas guardadas en el repositorio.

### Enlace de recuperación en el navegador

Comprobado a mano el 4 de octubre de 2026 con Chrome sin ventana, manejado por su protocolo de depuración, sobre la versión `v1.0.10`. Se usó una copia temporal de la aplicación contra la base de pruebas, con un usuario creado para la ocasión, y se repitió con el frontend en modo desarrollo (`npm run dev`) y compilado (`npm run build`, servido con `vite preview`). En los dos casos:

- El enlace abre el formulario «Elegir contraseña nueva».
- Nada más cargar, la barra de direcciones queda sin `reset_token` y no se añade ninguna entrada al historial.
- Al enviar la contraseña nueva se vuelve al inicio de sesión con el aviso «Contraseña cambiada». Después, la contraseña anterior responde 401 y la nueva 200.
- Usar el mismo enlace por segunda vez responde 400.
- Las peticiones a la API salen sin la cabecera `Referer`.

En la versión compilada ninguna petición lleva el identificador del enlace en `Referer`. En modo desarrollo lo lleva una sola: la del script que Vite inyecta al principio de la página (`/@vite/client`), que va al propio servidor de desarrollo, el mismo que ya recibió el enlace completo. Ese script no existe en la versión compilada.

No se guardó como prueba automática: el repositorio no incluye ninguna herramienta para manejar un navegador. Tampoco se comprobó en un despliegue real, con HTTPS y detrás de otro servidor, ni con el envío real del correo: el enlace se tomó de la consola del backend (ver «Seguridad» en `BACKEND-ARQUILA/docs/BACKEND_Y_API.md`).

### Instalación desde cero

Comprobada el 4 de octubre de 2026 con un clon nuevo del repositorio, siguiendo los pasos del `README.md` en Windows 11 con Python 3.12.10, Node 24.21.0 y PostgreSQL 18.6, sobre una base de datos vacía creada para la prueba:

| Paso | Resultado |
|---|---|
| `python -m venv .venv` y `pip install -r requirements-dev.txt` | instala sin errores en una ruta corta |
| `python -m app.migrate` y `python -m app.dev` sin `BACKEND-ARQUILA/.env` | terminan con el mensaje «DATABASE_URL is not set. Copy BACKEND-ARQUILA/.env.example to BACKEND-ARQUILA/.env and fill it in.» |
| `python -m app.migrate` (con `BACKEND-ARQUILA/.env`, sin `DATABASE_URL` en la terminal) | aplica las 9 migraciones; quedan 0 usuarios y 0 proyectos |
| `python -m app.migrate --seed` | carga 1 usuario y 3 proyectos; una segunda ejecución no duplica nada |
| `python -m app.check` | lee `BACKEND-ARQUILA/.env`; el asistente respondió con Ollama |
| `python -m app.dev` | `/health` responde `{"status":"ok"}` y `/docs` responde 200 |
| `npm install` y `npm run dev` | la interfaz responde 200; el usuario de demostración inicia sesión a través de `/api` y ve sus 3 proyectos |

`npm run dev` se probó en el puerto 5183 porque el 5173 estaba ocupado en el equipo. La interfaz no se abrió en un navegador en esa comprobación: se usaron peticiones HTTP.

### Resultados de la última ejecución

Ejecutados el 5 de octubre de 2026 en el equipo de desarrollo (Windows 11, Python 3.12, Node 24.21.0), después de la auditoría y de las mejoras del modelo 3D. Ese día no se repitió la ejecución local contra PostgreSQL 18.6 (la última, del 4 de octubre, pasó con 285 pruebas); la integración continua sí la corre, con PostgreSQL 16:

| Comando | Resultado |
|---|---|
| `python scripts/quality.py` | Ruff, Prettier y ESLint pasan |
| `pytest` (en `BACKEND-ARQUILA/`, con SQLite) | 286 pruebas pasan |
| `pytest` con PostgreSQL 16 (integración continua, commit `710ba88`) | pasa |
| `npm test` (en `FRONTEND-ARQUILA/`) | 124 pruebas pasan en 17 archivos |
| `npm run build` (en `FRONTEND-ARQUILA/`) | compila sin errores; Vite avisa de que un archivo generado supera los 500 kB |

La integración continua de GitHub ejecutó el commit `87e13d8` en `main` ese mismo día y sus tres trabajos terminaron bien: formato y reglas, backend (`pytest` con SQLite y con PostgreSQL 16) y frontend. Se consultó el estado de cada trabajo, no sus registros, así que el número de pruebas que corrió allí no se confirmó.

No se repitieron las comprobaciones manuales con fecha de este documento (el enlace de recuperación en el navegador y la cobertura), que son anteriores.

### Después de la separación en tres repositorios

Ejecutados el 6 de octubre de 2026 en el mismo equipo, ya con cada parte en su repositorio.

| Comando | Resultado |
|---|---|
| `python scripts/quality.py` | Ruff, Prettier y ESLint pasan |
| `pytest` (en `BACKEND-ARQUILA/`, con SQLite) | 288 pruebas pasan |
| `pytest` con PostgreSQL 18.6 local | 286 pruebas pasan (ejecutado antes de añadir las dos pruebas de `COOKIE_SAMESITE`) |
| `npm test` (en `FRONTEND-ARQUILA/`) | 124 pruebas pasan en 17 archivos |
| `npm run build` (en `FRONTEND-ARQUILA/`) | compila sin errores, con el mismo aviso de Vite |
| `python -m app.migrate --seed` y `bash scripts/init.sh --seed` sobre una base vacía | crean las 9 migraciones y los datos de ejemplo; las dos vías son compatibles |

Con el backend y el frontend arrancados desde sus repositorios, `/health`, el inicio de sesión del usuario de demostración y el listado de proyectos responden a través de `/api`.

Además se recorrió la interfaz en Chrome sin ventana (modo *headless*), con la sesión del usuario de demostración: las ocho secciones del menú y las ocho pestañas de «Casa Los Arrayanes», incluidos el modelo 3D, el recorrido interior y la vista de un archivo adjunto. Todas cargaron sin errores en la consola ni respuestas fallidas de la API. Fue un recorrido de lectura: no se crearon, editaron ni borraron datos, y no sustituye a probar a mano el guion de `docs/14_GUION_DEMO.md`.

### Versión publicada en Vercel

Ejecutados el 6 de octubre de 2026, al final del día, con el backend ya ampliado para guardar en la base los archivos subidos, el historial de «Deshacer» y los límites de intentos.

| Comando | Resultado |
|---|---|
| `pytest` (en `BACKEND-ARQUILA/`, con SQLite) | 304 pruebas pasan |
| `pytest` con PostgreSQL 18.6 local | 304 pruebas pasan |
| `npm test` (en `FRONTEND-ARQUILA/`) | 135 pruebas pasan en 19 archivos |
| `python scripts/quality.py` | Ruff, Prettier y ESLint pasan |

Sobre <https://arquila-frontend.vercel.app>, con peticiones HTTP y el usuario de demostración:

| Prueba | Resultado |
|---|---|
| Registro, inicio y cierre de sesión | La sesión se crea con cookie `HttpOnly` y deja de valer al cerrarla. |
| Subir, descargar y borrar un archivo | El contenido descargado tres veces a lo largo de minuto y medio es idéntico al subido; después de borrarlo responde 404. |
| Límite de intentos | Cinco inicios de sesión fallidos responden 401 y los dos siguientes 429. |
| Deshacer y rehacer | Con esperas de 15 a 20 segundos entre peticiones, el historial lista lo borrado, lo restaura con sus valores exactos, lo rehace y lo vuelve a deshacer. |

Además se abrieron en Chrome sin ventana la pantalla de inicio, la visualización 3D y el recorrido interior de la versión publicada, sin errores en la consola. Los datos creados en estas pruebas se borraron al terminar.

### Prueba de uso en el navegador

Hecha el 6 de octubre de 2026 en local, con Chrome sin ventana manejado por un guion que hace clic y escribe en los formularios como lo haría una persona, con una cuenta temporal que se borró al terminar:

| Paso | Resultado |
|---|---|
| Iniciar sesión desde el formulario | Aparece la página de inicio con el nombre del usuario. |
| Crear un proyecto y abrirlo | El proyecto aparece en la lista y se abre su página. |
| Agregar un material, editar su cantidad y borrarlo | La tabla refleja cada cambio y, al quedar vacía, lo dice. |
| Deshacer el borrado del material | El botón dice «Deshacer: material «Cemento»» y el material vuelve con la cantidad editada. |
| Borrar un cuarto en la pestaña Modelo 3D y deshacerlo | El botón dice «Deshacer: cuarto «Cocina»» y el cuarto vuelve; el modelo 3D se dibuja. |
| Cerrar sesión | Vuelve el formulario de inicio de sesión. |

Los catorce puntos comprobados pasaron, sin errores en la consola ni respuestas fallidas de la API. Es una prueba automatizada: no sustituye a que cada integrante recorra a mano `14_GUION_DEMO.md` antes de la defensa, pero cubre que los formularios y los botones funcionan de verdad.

Los cinco diagramas Mermaid de la documentación (el del `README.md`, los de `03_ARQUITECTURA.md` y `04_ESTRUCTURAS_DATOS.md` y el entidad-relación) se dibujaron sin errores con la versión 11 de Mermaid. No se comprobó cómo los muestra GitHub, que usa su propia versión.

### Cobertura del backend

Medida el 4 de octubre de 2026 con `coverage` y no repetida desde entonces: las pruebas ejecutaban el 98 % de las líneas de `BACKEND-ARQUILA/app/`, y el 100 % de `app/data_structures/`. Lo que quedaba sin ejecutar eran sobre todo los puntos de entrada de los comandos (`app.dev`, `app.migrate`, `app.check`) y la conexión real a la base. `coverage` no está entre las dependencias del proyecto; para repetir la medición:

```bash
pip install coverage
coverage run --source=app -m pytest
coverage report -m
```

### Integración continua

Desde la separación del proyecto en tres repositorios (6 de octubre de 2026), cada uno de los tres tiene su propio `.github/workflows/ci.yml`, que GitHub ejecuta en cada subida a `main` y en cada pull request:

- **`BACKEND-ARQUILA`:** comprueba el formato y las reglas con Ruff, instala `requirements-dev.txt` con Python 3.12 y ejecuta `pytest` dos veces, con SQLite y contra un PostgreSQL 16 que se levanta solo para esa ejecución. Para ello clona también `BASE-DE-DATOS-ARQUILA`, de donde salen las migraciones.
- **`FRONTEND-ARQUILA`:** `npm ci`, Prettier, ESLint, `npm test` y `npm run build` con Node 24.
- **`BASE-DE-DATOS-ARQUILA`:** crea el esquema y carga los datos de ejemplo en un PostgreSQL 16 vacío con `scripts/init.sh --seed`, dos veces, y comprueba que quedan anotadas todas las migraciones.

Este repositorio ya no contiene código de la aplicación, así que no tiene flujo.

`BASE-DE-DATOS-ARQUILA` tiene además `.github/workflows/reset-demo.yml`, que una vez al día restaura los datos del usuario de demostración en la base en línea, y `.github/workflows/deploy.yml`: al fusionar en `main` un cambio en `migrations/`, aplica las migraciones pendientes a la base de datos en línea. La rama `main` de los tres repositorios de código está protegida y solo acepta cambios cuyo flujo haya pasado.

El resultado se ve en la pestaña «Actions» de cada repositorio en GitHub. No despliega nada: solo avisa si un cambio rompe algo.

### Sin cobertura automática

La llamada real al servicio de IA, el envío real de correo y la mayoría de las pantallas del frontend. Las dos primeras se comprueban a mano con `python -m app.check`, que necesita credenciales reales (ver `BACKEND-ARQUILA/docs/BACKEND_Y_API.md`).
