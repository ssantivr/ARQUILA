# GUION DE LA DEMOSTRACIÓN

Recorrido paso a paso para mostrar la aplicación en unos diez minutos. Cada paso indica qué hacer, qué estructura de datos se activa y qué decir. El orden está pensado para que aparezcan las seis estructuras del curso.

## Antes de empezar

Hacerlo con tiempo, no delante del público:

1. Cargar los datos de ejemplo: en `BACKEND-ARQUILA/`, `python -m app.migrate --seed`.
2. Arrancar el backend: en `BACKEND-ARQUILA/`, `python -m app.dev`.
3. Arrancar el frontend: en `FRONTEND-ARQUILA/`, `npm run dev`.
4. Abrir dos pestañas del navegador: `http://localhost:5173` (la aplicación) y `http://localhost:8000/docs` (la documentación de la API).
5. Opcional: tener Ollama encendido con `llama3.2`, para que el asistente responda con IA. Si no, responde con reglas y el recorrido sirve igual.
6. Dejar a mano una terminal en `BACKEND-ARQUILA/` con el entorno virtual activado.

Si algo falla durante la demostración, la terminal del backend muestra una línea por cada petición, con su código de respuesta.

## Recorrido

### 1. Inicio de sesión bloqueado — Queue

| | |
|---|---|
| **Qué hacer** | En la pantalla de inicio de sesión, escribir el correo `prueba@example.com` y una contraseña cualquiera. Pulsar «Entrar» seis veces seguidas. |
| **Qué se ve** | Las cinco primeras veces aparece «Correo o contraseña incorrectos.». La sexta, «Demasiados intentos fallidos. Espera un minuto e inténtalo de nuevo.». |
| **Estructura** | Cola (FIFO), en `BACKEND-ARQUILA/app/services/login_limiter.py`. |
| **Qué decir** | «Cada correo tiene una cola con la hora de sus intentos fallidos. La capacidad de la cola es el máximo permitido, cinco. Cuando la cola está llena, el correo queda bloqueado. Los intentos caducan en el mismo orden en que llegaron, así que el más antiguo siempre está al frente: por eso es una cola y no una pila. Encolar, desencolar y consultar el frente son O(1).» |

Se usa un correo que no existe para no bloquear la cuenta de demostración: el límite se cuenta por correo.

### 2. Entrar y ver el resumen

| | |
|---|---|
| **Qué hacer** | Cambiar el correo a `demo@example.com`, contraseña `arquila-demo`, y pulsar «Entrar». |
| **Qué se ve** | La página de Inicio con el resumen: proyectos, terrenos y costo de materiales. |
| **Estructura** | Ninguna del curso; es el contexto. |
| **Qué decir** | «La aplicación reúne proyectos de arquitectura con su terreno, sus planos, sus materiales y un modelo 3D. El frontend está en TypeScript con React, el backend en Python con FastAPI y los datos en PostgreSQL. La contraseña se guarda con Argon2 y la sesión viaja en una cookie que el código de la página no puede leer.» |

### 3. Un lote con forma libre — Array

| | |
|---|---|
| **Qué hacer** | En el menú, «Proyectos». Abrir «Casa Los Arrayanes». En la pestaña «Terreno», ir al formulario del final: Nombre `Lote esquinero`; en «Vértices del lote» escribir, uno por línea, `0 0`, `20 0`, `20 10`, `10 10`, `10 30`, `0 30`. |
| **Qué se ve** | El campo «Área (m²)» sugiere 400, el área del polígono. Escribir `400` y pulsar «Agregar terreno»: el lote aparece dibujado con su forma en L. |
| **Estructura** | Array unidimensional: los vértices, recorridos en orden. `BACKEND-ARQUILA/app/services/geometry.py` y `FRONTEND-ARQUILA/src/utils/geometry.ts`. |
| **Qué decir** | «Los vértices son un array. El área se calcula con la fórmula de Gauss recorriendo el array una sola vez, es decir, en O(n). El backend hace la misma cuenta al recibir el lote y lo rechaza si los puntos no encierran un área.» |

### 4. Eliminar y deshacer — Lista doblemente enlazada

| | |
|---|---|
| **Qué hacer** | En la pestaña «Materiales», pulsar «Eliminar» en la fila de «Cemento». Después eliminar también «Bloque de 15 cm». |
| **Qué se ve** | Las dos filas desaparecen. En la cabecera del proyecto, el botón pasa de «Nada que deshacer» a «Deshacer: material «Bloque de 15 cm»». |
| **Qué hacer** | Pulsar ese botón una vez. |
| **Qué se ve** | Vuelve «Bloque de 15 cm» y el botón ofrece ahora deshacer «Cemento». No pulsarlo todavía: se usa en el paso siguiente. |
| **Estructura** | Lista doblemente enlazada, en `BACKEND-ARQUILA/app/services/undo_history.py`. |
| **Qué decir** | «Cada proyecto guarda sus últimas veinte eliminaciones en una lista doblemente enlazada. Registrar una eliminación es insertar al final; deshacer es quitar del final; y cuando se supera el límite se descarta la más antigua, que está al inicio. Las tres operaciones son O(1) porque la lista tiene punteros a los dos extremos y cada nodo conoce al anterior. Una pila no serviría: no permite quitar el elemento más antiguo.» |

### 5. Rehacer — Stack

La aplicación no tiene botón de rehacer; la operación existe en la API y se muestra desde su documentación.

| | |
|---|---|
| **Qué hacer** | Ir a la pestaña `http://localhost:8000/docs`. Buscar `POST /projects/{project_id}/redo`, pulsar «Try it out», escribir el número del proyecto (se ve en la respuesta de `GET /projects`, o es el 1 con los datos de ejemplo recién cargados) y pulsar «Execute». |
| **Qué se ve** | Respuesta 200 con el material «Bloque de 15 cm». Al volver a la aplicación y abrir de nuevo la pestaña «Materiales», el bloque ha vuelto a desaparecer. |
| **Estructura** | Pila (LIFO), en el mismo archivo `undo_history.py`. |
| **Qué decir** | «Cada vez que se deshace algo, lo restaurado se apila. Rehacer saca lo que está en la cima: lo último que se deshizo es lo primero que se rehace. Eso es exactamente LIFO, y `push` y `pop` son O(1). De paso, esta página es la documentación que FastAPI genera sola a partir del código.» |

Para dejar el proyecto como estaba, volver a la aplicación y pulsar «Deshacer» dos veces.

### 6. Preguntar al asistente — Array dinámico y lista simplemente enlazada

| | |
|---|---|
| **Qué hacer** | En la pestaña «Asistente IA», mirar la línea «Ahora responde» y pulsar la pregunta sugerida «¿Qué materiales pesan más en el costo?». Después escribir una segunda pregunta, por ejemplo «¿Y cuál es el más barato?». |
| **Qué se ve** | La respuesta nombra los materiales más caros con sus cifras; en «Casa Los Arrayanes» el primero es «Hormigón 210 kg/cm²», con 10 856. Si no hay IA disponible, la respuesta lleva la etiqueta de respuesta por reglas. |
| **Estructura** | Array dinámico, en `BACKEND-ARQUILA/app/services/material_ranking.py`, y lista simplemente enlazada, en `BACKEND-ARQUILA/app/services/conversation_context.py`. |
| **Qué decir** | «Antes de preguntar a la IA, el backend ordena los materiales del más caro al más barato insertando cada uno en su posición dentro de un array dinámico, que duplica su capacidad cuando se llena. Así la IA recibe las cifras ya calculadas y no las inventa. Y el historial de la conversación que se le envía es una lista simplemente enlazada que funciona como ventana: se agrega al final y, al pasar de veinte mensajes, se quita el del inicio. Las dos operaciones son O(1) con punteros a la cabeza y a la cola.» |

### 7. El modelo 3D

| | |
|---|---|
| **Qué hacer** | Pestaña «Modelo 3D». Pulsar «Frontal», «Superior» e «Isométrica», y hacer clic en un cuarto. |
| **Qué se ve** | El edificio con sus niveles, cuartos, techo y terreno, y los datos del cuarto seleccionado en el inspector. |
| **Estructura** | Ninguna del curso. |
| **Qué decir** | «Esto va más allá de la asignatura y no es el centro de la defensa: muestra que las estructuras viven dentro de una aplicación completa.» |

### 8. Pruebas y mediciones

| | |
|---|---|
| **Qué hacer** | En la terminal de `BACKEND-ARQUILA/`, ejecutar `pytest -q tests/test_data_structures.py` y después `python -m app.benchmark`. |
| **Qué se ve** | Las pruebas de las estructuras en verde, y una tabla con los tiempos de cada operación para mil, diez mil y cien mil elementos. |
| **Estructura** | Todas. |
| **Qué decir** | «Las estructuras están escritas desde cero en Python, sin librerías que las reemplacen, y cada una tiene pruebas de sus casos borde: vacía, un solo elemento, llena. La tabla confirma la teoría: las operaciones O(1) tardan lo mismo con mil elementos que con cien mil, y las O(n) tardan unas cien veces más cuando hay cien veces más datos.» |

## Si preguntan

Las respuestas a las preguntas habituales, con el archivo que conviene mostrar en cada una, están en `09_GUIA_DEFENSA.md`. La complejidad de cada operación y las mediciones están en `05_COMPLEJIDAD.md`.

## Qué no está comprobado

Los pasos 1 a 5 se ensayaron el 4 de octubre de 2026 contra la API con los datos de ejemplo recién cargados, y dieron exactamente los resultados descritos: cinco rechazos y un bloqueo, el lote de 400 m² aceptado, las dos eliminaciones, el deshacer, el rehacer y la restauración final de los ocho materiales. Las pantallas se revisaron en el navegador, pero el recorrido no se hizo clic a clic de principio a fin, y en el paso 5 no se comprobó en el navegador que la página `/docs` use la misma sesión que la aplicación. Conviene hacer el recorrido completo una vez antes de la defensa.
