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

### Estructuras en C++

Los casos anteriores, y los de los arrays, están automatizados con Catch2 en `data_structures/tests/`. Se ejecutan con un solo comando, que también compila:

```bash
cd data_structures
cmake --workflow --preset default
```

Además de los casos de cada estructura, las pruebas comprueban:

- La pila y la cola llenas (100 elementos) y vacías, y que la cola mantiene el orden después de dar la vuelta al array.
- Que cada estructura funciona con un tipo distinto de `int` (`std::string`).
- La memoria de las listas: con un tipo que cuenta cuántos valores existen, se verifica que insertar, eliminar, vaciar y destruir la lista deja el contador en cero, es decir, que cada nodo se libera una sola vez.

En GitHub las mismas pruebas y los cinco programas se ejecutan compilados con AddressSanitizer y UndefinedBehaviorSanitizer, que fallan ante una fuga, una doble liberación o un acceso fuera de rango. En Windows no se pueden usar porque GCC para Windows no los incluye.

### Backend

Las pruebas automatizadas están en `backend/tests/` y se ejecutan con `pytest`. Por defecto usan una base SQLite en memoria, así que no necesitan PostgreSQL.

### Ejecución contra PostgreSQL

Las mismas pruebas se pueden ejecutar contra un PostgreSQL real definiendo `TEST_DATABASE_URL`:

```bash
set TEST_DATABASE_URL=postgresql+psycopg://usuario:clave@localhost:5432/arquila_test
pytest
```

En ese modo las tablas se crean aplicando las migraciones de `database/migrations/`, de modo que también se comprueba que el esquema coincide con los modelos del backend.

Antes de cada prueba se vacían todas las tablas de esa base. Por seguridad, las pruebas se niegan a ejecutarse si el nombre de la base no termina en `test`. Nunca se debe apuntar a una base con datos reales.

Verificado el 3 de octubre de 2026 con PostgreSQL 16.2 y 18.6: todas las pruebas pasan.

Cubren:

- El endpoint `/health`.
- Las estructuras de datos en Python: orden LIFO y FIFO, estructura vacía y llena, reutilización de posiciones en la cola circular, inserción y eliminación en las listas, y búsquedas.
- Registro, inicio y cierre de sesión, y que cada endpoint exija sesión.
- El resumen de las contraseñas (`test_security.py`): que se calcule con Argon2id, que solo verifique la contraseña correcta, que cambie en cada cálculo, que un valor guardado con otro formato nunca se dé por válido y que una contraseña antigua guardada con `scrypt` siga sirviendo y se actualice al iniciar sesión.
- El registro de eventos (`test_logs.py`): una línea JSON por petición, los avisos de inicio de sesión fallido y bloqueado, y que no aparezcan contraseñas, correos ni parámetros de la dirección.
- El comando de migraciones (`test_migrate.py`): que lea la conexión de `backend/.env`, cargue los datos de ejemplo y se detenga con un mensaje claro si falta `DATABASE_URL`.
- Que un usuario no pueda ver ni modificar los datos de otro.
- Crear, listar, actualizar y eliminar proyectos, terrenos, materiales, planos y elevaciones, con sus validaciones.
- Subida de archivos: tipos admitidos, tamaño máximo y adjuntarlos a planos y elevaciones.
- Recomendaciones automáticas, con la prioridad que asigna cada regla, y deshacer eliminaciones.
- Cuartos: crear, listar, actualizar y eliminar, que las medidas sean mayores que cero, que el plano sea del mismo proyecto y que se eliminen con su plano o su proyecto.
- Componentes estructurales: crear, listar y filtrar por tipo, actualizar y eliminar, que el tipo sea columna, viga o muro, que las medidas sean mayores que cero, que el plano sea del mismo proyecto y que se eliminen con su plano o su proyecto.
- El modelo 3D (`/projects/{id}/structure`): colocación de los terrenos, qué planos cuentan como nivel, cuartos frente a volumen del nivel, altura de cada nivel, y posición de los componentes, con las vigas colgadas del techo del nivel. También el material de superficie de cada elemento: que se guarde en un cuarto, en un componente y en el plano de un volumen, que se rechace un material o un tipo desconocido, que el elemento sea de ese tipo y de ese proyecto, y que otro usuario no pueda cambiarlo. Y el tipo de cubierta del proyecto: a dos aguas por defecto, que se guarde la plana, que se rechace otro valor y que otro usuario no pueda cambiarla.
- Proyectos de ejemplo: la lista, la creación de un proyecto completo desde un ejemplo, el nombre libre al repetirlo, los permisos, y que los cuartos de cada ejemplo caben en su lote sin solaparse.
- CORS: que se admita el origen de la aplicación con credenciales, que se responda la petición previa y que se ignoren otros orígenes. También que todas las respuestas, incluidas las de error, lleven las cabeceras de seguridad (ver «Seguridad» en `12_BACKEND_Y_API.md`).
- Conversaciones con el asistente, usando un asistente simulado, y la respuesta por reglas cuando la IA falla o no está configurada.
- Los adaptadores de correo y de IA (`test_adapters.py`), sustituyendo `smtplib` y el SDK de Anthropic por objetos simulados: qué adaptador de correo se elige según la configuración, las llamadas SMTP que hace, y cómo el adaptador de IA extrae el texto, trata las negativas y las respuestas vacías y convierte los errores del SDK en el error de la aplicación. El adaptador del modelo local se prueba contra un servidor de Ollama simulado: qué modelo elige, qué envía, y qué hace si Ollama no responde, no tiene modelos o contesta algo inesperado.

### Frontend

Las funciones de cálculo y de texto del frontend tienen pruebas en `frontend/src/utils/*.test.ts`, `frontend/src/state/*.test.ts`, `frontend/src/three/*.test.ts` y `frontend/src/services/*.test.ts`, que se ejecutan con `npm test`: área, vista frontal, curvas de nivel, cotas de cada lado, área edificable y lectura de los vértices de un lote, agrupación de cuartos por nivel e indicadores de ocupación (huella, área construida, COS y CUS), colocación de ventanas y puerta, forma del techo (a dos aguas o plano), fachadas y corte generados con cada cubierta, geometría de un cuarto en el modelo 3D (muros dentro de su medida, vidrio, hoja de puerta, líneas de cada vano y coordenadas de textura en metros), colores del modelo 3D (por tipo, por coste estimado y por alertas), materiales de superficie (el elegido o el predeterminado de cada tipo, y la lectura de los guardados), el estado compartido `appState` (avisos a los suscriptores, cambio de proyecto, material de superficie por elemento y recarga con peticiones simuladas), traducción de los mensajes de error y formato de números.

También están probados la colocación de puertas y ventanas y la forma del techo del modelo 3D (`openings.test.ts`), y el cliente HTTP (`http.test.ts`) con un `fetch` simulado: la dirección y el cuerpo de cada petición, los mensajes de error que devuelve el backend y el aviso de sesión terminada.

Los componentes principales tienen pruebas con Testing Library (`*.test.tsx`), que los dibujan en un navegador simulado (jsdom) con la API sustituida por funciones simuladas:

- `AsyncStatus.test.tsx`: los estados de carga, lista vacía y error, y el botón «Reintentar».
- `Sidebar.test.tsx`: los siete módulos, el módulo actual y la navegación con ratón y solo con teclado.
- `LoginPage.test.tsx`: las etiquetas de los campos, el inicio de sesión (también solo con teclado), el botón bloqueado mientras se envía, el error traducido, el registro y la recuperación de contraseña.
- `ProjectsPage.test.tsx`: la carga y la lista de proyectos, abrir un proyecto, el error de carga con su reintento, y crear un proyecto con y sin error.

`npm test` ejecuta estas pruebas junto con las de cálculo. Las demás pantallas se revisaron a mano en un navegador automatizado (diseño en pantallas pequeñas, etiquetas y contraste; ver «Frontend» en `12_BACKEND_Y_API.md`), pero no tienen pruebas automáticas guardadas en el repositorio.

### Enlace de recuperación en el navegador

Comprobado a mano el 4 de octubre de 2026 con Chrome sin ventana, manejado por su protocolo de depuración, sobre la versión `v1.0.10`. Se usó una copia temporal de la aplicación contra la base de pruebas, con un usuario creado para la ocasión, y se repitió con el frontend en modo desarrollo (`npm run dev`) y compilado (`npm run build`, servido con `vite preview`). En los dos casos:

- El enlace abre el formulario «Elegir contraseña nueva».
- Nada más cargar, la barra de direcciones queda sin `reset_token` y no se añade ninguna entrada al historial.
- Al enviar la contraseña nueva se vuelve al inicio de sesión con el aviso «Contraseña cambiada». Después, la contraseña anterior responde 401 y la nueva 200.
- Usar el mismo enlace por segunda vez responde 400.
- Las peticiones a la API salen sin la cabecera `Referer`.

En la versión compilada ninguna petición lleva el identificador del enlace en `Referer`. En modo desarrollo lo lleva una sola: la del script que Vite inyecta al principio de la página (`/@vite/client`), que va al propio servidor de desarrollo, el mismo que ya recibió el enlace completo. Ese script no existe en la versión compilada.

No se guardó como prueba automática: el repositorio no incluye ninguna herramienta para manejar un navegador. Tampoco se comprobó en un despliegue real, con HTTPS y detrás de otro servidor, ni con el envío real del correo: el enlace se tomó de la consola del backend (ver «Seguridad» en `12_BACKEND_Y_API.md`).

### Instalación desde cero

Comprobada el 3 de octubre de 2026 con un clon nuevo de la rama `main`, siguiendo los pasos del `README.md` en Windows 11 con Python 3.12, Node 24 y PostgreSQL:

- Backend: entorno virtual, `pip install -r requirements-dev.txt` y `pytest`. Las 216 pruebas pasan.
- Frontend: `npm install`, `npm test` (29 pruebas) y `npm run build`.
  - Desde entonces se añadieron pruebas: al 4 de octubre de 2026 `npm test` ejecuta 118 y `pytest` 269, y todas pasan (el backend, tanto con SQLite como con PostgreSQL). Ese dato es del equipo de desarrollo; la instalación desde un clon nuevo no se repitió.
- Base de datos: sobre una base vacía, `python -m app.migrate --seed` aplicó las seis migraciones y cargó los datos de ejemplo. Una segunda ejecución no aplicó ni duplicó nada.
- Aplicación: con esa base, el usuario de demostración inicia sesión y ve sus tres proyectos con sus datos; generar recomendaciones y preguntar al asistente funcionan.

- Estructuras en C++: los cinco programas de `data_structures/` compilan con GCC 16.2 (`g++ -std=c++17 -static -Wall -Wextra -Wpedantic`) sin ningún aviso, y al ejecutarlos su salida coincide con los casos de este documento: la pila devuelve 30, 20, 10; la cola 10, 20, 30 y sigue funcionando después de dar la vuelta; las listas insertan, eliminan y se recorren en los dos sentidos. No se analizó el uso de memoria con una herramienta, porque GCC para Windows no incluye ese análisis.

No se comprobó el arranque con `docker-compose.yml`, porque el equipo no tiene Docker.

En Windows, si la carpeta del proyecto está en una ruta muy larga, `pip install` puede fallar con el error «No such file or directory» en un archivo del paquete `anthropic`. Se resuelve clonando el proyecto en una ruta corta.

### Cobertura del backend

Medida el 4 de octubre de 2026 con `coverage`: las pruebas ejecutan el 98 % de las líneas de `backend/app/`, y el 100 % de `app/data_structures/`. Lo que queda sin ejecutar son sobre todo los puntos de entrada de los comandos (`app.dev`, `app.migrate`, `app.check`) y la conexión real a la base. `coverage` no está entre las dependencias del proyecto; para repetir la medición:

```bash
pip install coverage
coverage run --source=app -m pytest
coverage report -m
```

### Integración continua

El archivo `.github/workflows/ci.yml` hace que GitHub ejecute las comprobaciones en cada subida a `santiago` o a `main` y en cada pull request. Son cuatro trabajos independientes:

- **Formato y reglas:** ejecuta `python scripts/quality.py` (Ruff, Prettier, ESLint y clang-format) y falla si algún archivo no cumple.
- **Estructuras en C++:** compila `data_structures/` con CMake, con AddressSanitizer y UndefinedBehaviorSanitizer activados, ejecuta las pruebas de Catch2 y después cada programa de ejemplo.
- **Backend:** instala `requirements-dev.txt` con Python 3.12 y ejecuta `pytest` dos veces, con SQLite y contra un PostgreSQL 16 que se levanta solo para esa ejecución.
- **Frontend:** `npm ci`, `npm test` y `npm run build` con Node 24.

El resultado se ve en la pestaña «Actions» del repositorio en GitHub. No despliega nada: solo avisa si un cambio rompe algo.

### Sin cobertura automática

La llamada real al servicio de IA, el envío real de correo y la mayoría de las pantallas del frontend. Las dos primeras se comprueban a mano con `python -m app.check`, que necesita credenciales reales (ver `12_BACKEND_Y_API.md`).
