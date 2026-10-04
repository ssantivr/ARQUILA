# GUÍA PARA LA DEFENSA

Durante la presentación se debe poder explicar cada punto de esta lista. Debajo de cada uno hay una respuesta corta y el archivo donde se puede mostrar.

La defensa debe priorizar comprensión sobre cantidad de funcionalidades.

## 1. Qué problema resuelve el proyecto

ARQUILA demuestra el uso práctico de las estructuras de datos vistas en clase dentro de una aplicación organizada: una plataforma para gestionar proyectos de arquitectura con sus terrenos, planos, elevaciones y materiales.

Ver `docs/01_PLANTEAMIENTO_PROBLEMA.md`.

## 2. Por qué se separan frontend, backend y estructuras de datos

Cada parte tiene una sola responsabilidad y se puede probar y cambiar sin tocar las otras:

- `frontend/` muestra la información y envía peticiones; no guarda datos ni decide permisos.
- `backend/` valida, aplica las reglas y guarda en la base de datos.
- `backend/app/data_structures/` contiene las estructuras, escritas en Python. No importan nada del resto del backend: los servicios las usan, pero ellas no conocen la API ni la base de datos.

Dentro del backend se repite la misma idea por capas: `api` recibe la petición, `services` aplica la lógica y `repositories` consulta la base. Ver `docs/03_ARQUITECTURA.md` y `docs/12_BACKEND_Y_API.md`.

## 3. Cómo funciona un array

Guarda los elementos en posiciones consecutivas de memoria. Por eso llegar a una posición por su índice es inmediato, O(1), pero insertar o eliminar en medio obliga a desplazar los elementos que siguen, O(n).

Mostrar `insert_at` y `remove_at` de `DynamicArray` en `backend/app/data_structures/arrays.py`.

## 4. Qué significa memoria dinámica

Es memoria que se reserva mientras el programa se ejecuta, y no antes. Permite decidir el tamaño en tiempo de ejecución.

En `arrays.py`, cuando el array dinámico se llena, `_resize` crea un bloque del doble de tamaño y copia los elementos uno por uno. En las listas, cada `push_front` o `push_back` crea un nodo nuevo.

En Python el programa no libera la memoria a mano: el intérprete la recupera cuando ya nada hace referencia al objeto. Por eso, al quitar un nodo, basta con que ningún otro nodo ni `head` ni `tail` lo sigan apuntando.

## 5. Por qué Stack utiliza LIFO

Porque solo se trabaja por un extremo, la cima: el último elemento que entra es el primero que sale. `push` y `pop` solo mueven el contador `_size`, que indica la cima, por eso son O(1).

Mostrar `backend/app/data_structures/stack.py`. `pop` en una pila vacía produce `IndexError`, en lugar de devolver un valor especial como `None`, para que la pila pueda guardar cualquier valor.

## 6. Por qué Queue utiliza FIFO

Porque se inserta por un extremo y se retira por el otro: el primero que entra es el primero que sale.

La cola es circular: el índice avanza con el operador módulo y reutiliza las posiciones que quedan libres al retirar elementos. Sin eso, después de tantas inserciones como su capacidad la cola se reportaría llena aunque estuviera vacía. Mostrar `enqueue` y `dequeue` en `backend/app/data_structures/queue.py`.

## 7. Cómo funciona la referencia `next` de una lista simple

Cada nodo guarda un dato y una referencia al nodo siguiente; en el último, `next` es `None`. Para recorrer la lista se empieza en `head` y se sigue `next` hasta llegar a `None`.

Solo se puede avanzar. Para eliminar un nodo hay que estar parado en el anterior, porque es su `next` el que se debe cambiar. Mostrar `remove` en `backend/app/data_structures/singly_linked_list.py`. El diagrama de nodos está en `docs/04_ESTRUCTURAS_DATOS.md`.

## 8. Cómo funcionan `previous` y `next` en una lista doble

Cada nodo conoce a su vecino anterior y al siguiente, así que la lista se puede recorrer en los dos sentidos y se puede retirar un nodo teniendo solo ese nodo.

El costo es más memoria por nodo y más referencias que mantener en cada operación. Mostrar `__iter__` y `__reversed__` en `backend/app/data_structures/doubly_linked_list.py`.

## 9. Qué ocurre cuando se elimina un nodo

En la lista doble, el método `_unlink` hace tres cosas:

1. El `next` del nodo anterior pasa a apuntar al nodo siguiente. Si no hay anterior, el nodo era la cabeza y `head` pasa al siguiente.
2. El `previous` del nodo siguiente pasa a apuntar al anterior. Si no hay siguiente, el nodo era la cola y `tail` pasa al anterior.
3. Se descuenta el nodo del contador y se devuelve su dato. Como ya nada lo apunta, el intérprete recupera su memoria.

Los casos que hay que saber explicar son: eliminar la cabeza, eliminar la cola, eliminar el único nodo y eliminar uno intermedio.

## 10. Qué significa O(1) y O(n) en los ejemplos del proyecto

- O(1): el trabajo no depende de cuántos elementos haya. Ejemplos: `push` y `pop` de la pila, `enqueue` y `dequeue` de la cola, insertar al inicio de una lista.
- O(n): el trabajo crece con la cantidad de elementos. Ejemplos: buscar un valor en una lista, insertar en medio de un array.
- O(log n): la búsqueda binaria descarta la mitad de los elementos en cada paso, pero exige que el array esté ordenado.

La tabla completa está en `docs/05_COMPLEJIDAD.md`, junto con las mediciones de tiempo que la confirman. El recorrido para mostrar cada estructura en la aplicación está en `docs/14_GUION_DEMO.md`.

## 11. Dónde se usa una estructura de datos dentro de la aplicación

El botón «Deshacer» del proyecto usa una lista doblemente enlazada (`backend/app/services/undo_history.py`). Cada eliminación se agrega al final; deshacer saca la última; y cuando hay más de 20 se descarta la más antigua por el inicio. Las tres operaciones son O(1).

Una pila no alcanzaría, porque no permite quitar el elemento más antiguo.

El límite de intentos de inicio de sesión usa una cola (`backend/app/services/login_limiter.py`). Cada intento fallido se encola con su hora; los que tienen más de un minuto se retiran por el frente, porque el más antiguo es siempre el primero en caducar (FIFO). Si la cola está llena, hay 5 fallos recientes y el inicio de sesión se bloquea.

## 12. Cómo se validó el backend

Con pruebas automatizadas en `backend/tests/`, que se ejecutan con `pytest`. Comprueban las estructuras de datos, el inicio de sesión, los permisos entre usuarios y cada operación de la API. Ver `docs/06_PRUEBAS.md`, que también indica lo que no está cubierto.

Las pruebas de las estructuras están en `backend/tests/test_data_structures.py` y cubren los casos borde: vacía, llena, un solo elemento, y eliminar la cabeza y la cola.

## 13. Qué decisiones se tomaron para mantener el proyecto dentro del alcance de la asignatura

- Solo se usan las estructuras estudiadas: arrays, pila, cola y listas enlazadas. No hay árboles, grafos ni tablas hash propias.
- La pila y la cola tienen capacidad fija y las listas solo guardan las referencias necesarias. No hay optimizaciones avanzadas.
- La aplicación sí incluye partes que van más allá de la asignatura (inicio de sesión, subida de archivos, asistente de IA, modelo 3D). Siguen el plan de `13_EVOLUCION_POR_SEMANAS.md`, pero no son el centro de la defensa: conviene presentarlas como contexto y concentrar la explicación en las estructuras y en el punto 11.

## 14. Qué patrón de diseño se usa y por qué

El patrón Adapter, en los servicios externos del backend. El asistente necesita «dame una respuesta para esta conversación», pero cada proveedor se llama de una forma distinta. Un adaptador es una clase que ofrece la forma que la aplicación espera y la traduce a la del proveedor.

- La interfaz es `Assistant`, con un solo método: `reply(system, messages)`.
- `ClaudeAssistant` la traduce a las llamadas del SDK de Anthropic.
- `OllamaAssistant` la traduce a peticiones HTTP a un modelo local.

`ConversationService` solo conoce la interfaz. Por eso se añadió el modelo local sin tocar ese servicio, y las pruebas usan un asistente simulado en lugar de uno real. El correo sigue la misma idea con `SmtpMailer` y `ConsoleMailer`.

Mostrar `backend/app/ai.py`. Ver la tabla de `docs/12_BACKEND_Y_API.md`.

## 15. Qué pasa si la IA no está disponible o se equivoca

- Si no hay ningún proveedor, o la llamada falla, el asistente contesta con reglas fijas sobre los datos del proyecto (`backend/app/services/assistant_rules.py`). Cada respuesta guarda su origen, `ai` o `rules`, y la pantalla lo indica.
- Un modelo pequeño se equivoca al hacer cuentas. Se comprobó: señaló como más caro un material que no lo era. La corrección fue no dejarle calcular: el backend le entrega los costos y el total ya calculados.

Para demostrarlo en vivo: hacer una pregunta con Ollama encendido, cerrarlo, y repetir la pregunta; la segunda respuesta lleva la etiqueta «Respuesta por reglas».

## 16. Qué algoritmos hay en los esquemas del terreno

Todos recorren los vértices del lote una sola vez, O(n), sobre un array de puntos (`frontend/src/utils/geometry.ts`):

- El área usa la fórmula de Gauss: suma un producto por cada par de vértices consecutivos.
- Las cotas del plano de implantación calculan la longitud de cada lado y hacia dónde queda el exterior.
- La vista frontal convierte la profundidad de cada vértice en altura según la pendiente.

Es un ejemplo de recorrido de array con acceso por índice, incluido el paso del último elemento al primero con el operador módulo, igual que en la cola circular.

## 17. Cómo se arma el modelo 3D del proyecto

El backend no guarda el modelo: lo calcula cada vez que se pide, con los terrenos, planos, cuartos y componentes del proyecto (`backend/app/services/structure_service.py`). Devuelve una lista de cajas en metros y el frontend solo las dibuja.

- Cada terreno es una losa. Como cada terreno guarda sus medidas desde su propio origen, se colocan uno al lado del otro.
- Un plano es un nivel si tiene cuartos o componentes, o si su campo Nivel es un número. Así un plano de implantación no se apila como si fuera un piso.
- Los niveles se apilan con un acumulador: la base de cada nivel es la suma de las alturas de los anteriores. Es un recorrido de la lista de planos, O(n).
- Mientras el proyecto no tiene cuartos, cada nivel se dibuja como un volumen de 3 m dentro del retiro del lote.
- Las ventanas, la puerta, el techo y los árboles no son datos: el visor los añade al dibujar para que el modelo se lea como una edificación.
- Con esos mismos cuartos se generan las plantas con ejes y cotas, las cuatro fachadas y un corte. Los ejes de una planta salen de ordenar los bordes de los cuartos y quitar los repetidos: un recorrido y un ordenamiento de un array.

Lo que no hace: no representa la pendiente del terreno ni comprueba que un cuarto quede dentro del lote. Ver «Modelo 3D» en `docs/12_BACKEND_Y_API.md`.

## 18. Por qué un cuarto, una columna, una viga y un muro son cajas

Porque una caja alineada con los ejes se describe con seis números (posición `x`, `y` y ancho, largo, alto) y alcanza para lo que el proyecto necesita mostrar. Con esa decisión:

- los cuartos y los componentes comparten validaciones, formulario y dibujo;
- una columna, una viga y un muro son la misma tabla con un campo `kind`, en lugar de tres tablas;
- la única diferencia al dibujar es que la viga se cuelga del techo del nivel y los demás se apoyan en el piso.

El costo es que no hay muros en diagonal ni cuartos con forma de L, y que no hay cálculo estructural: los componentes se registran y se dibujan, nada más.

En los datos un cuarto sigue siendo una caja. Solo al dibujarlo, `frontend/src/three/roomGeometry.ts` la convierte en cuatro muros con espesor, con un hueco por cada ventana o puerta, y dos losas. Es un recorrido de los vanos del cuarto, O(v).

## 19. Cómo se evita que el visor 3D gaste memoria

Three.js reserva memoria en la tarjeta gráfica para cada geometría y cada material, y no la libera sola. Es lo contrario de lo que pasa en Python (punto 4): aquí lo que se reserva hay que liberarlo a mano.

- Cada vez que cambia el modelo, y al salir de la pestaña, se llama a `dispose` sobre geometrías, materiales, el mapa de sombras y el contexto WebGL, y se quitan los eventos del ratón y de la ventana.
- Three.js se carga solo al abrir la pestaña Modelo 3D, así que no pesa en el resto de la aplicación.

Mostrar `disposeObject` y `dispose` en `frontend/src/three/structureViewer.ts`. Para demostrarlo en vivo: abrir la pestaña, cambiar a otra y comprobar en las herramientas del navegador que ya no hay ningún lienzo (`canvas`) en la página.

## 20. Cómo se selecciona un cuarto con un clic

Con un rayo (`Raycaster`): se traza una línea desde la cámara que pasa por el punto donde se hizo clic y se toma la primera caja que atraviesa. Si el puntero se movió más de 4 píxeles entre pulsar y soltar, se considera que el usuario estaba girando la cámara y no se selecciona nada.

El elemento seleccionado se guarda en un solo lugar, el estado compartido `frontend/src/state/appState.ts`, y de ahí lo leen el modelo, la lista de niveles, el inspector y el asistente. Por eso todos muestran siempre lo mismo.

## 21. Cómo se colorea el modelo por coste o por alertas

El visor no calcula nada: recibe un diccionario de elemento a color y pinta. Los cálculos están en `frontend/src/utils/elementColors.ts`.

- Coste: se suma el costo de los materiales del proyecto y se reparte entre los elementos según su volumen. Son dos recorridos, O(n + m) con n elementos y m materiales. Es una estimación, porque un material no guarda a qué elemento pertenece.
- Alertas: por cada elemento se buscan las recomendaciones que lo nombran y se toma la prioridad más alta. Es un recorrido dentro de otro, O(n · r) con r recomendaciones; con los tamaños de un proyecto no se nota.
- El diccionario es un `Map`, así que pintar cada elemento es una búsqueda O(1).

Los materiales, las recomendaciones, la selección y el modo de color viven en `appState.ts`: una variable con el estado, una lista de funciones suscritas y un aviso a todas cuando algo cambia. Es el patrón observador, sin librerías.

## 22. Cómo se cambia el material de un elemento en el modelo

Al seleccionar un elemento, el inspector muestra un selector «Material». Al elegir una opción se llama a `appState.setSurface`, que guarda la elección en un diccionario de elemento a material; el visor está suscrito, recibe el diccionario (`setSurfaces`) y cambia el color, la rugosidad, el brillo metálico y la opacidad del material de esa caja. No se reconstruye el modelo, por eso el cambio es inmediato. A la vez se envía al backend (`PATCH /projects/{id}/structure/{kind}/{element_id}/surface`), que lo guarda en la columna `surface` del cuarto, del componente o del plano; si falla, el visor vuelve al material anterior y muestra el error.

- El catálogo está en `frontend/src/utils/surfaceMaterials.ts`. Buscar el material de un elemento es una consulta O(1) en el diccionario; repintar recorre los n elementos, O(n).
- El elemento seleccionado se marca con un contorno cian y no con un tinte, para que el material se vea tal cual.
- El tipo de cubierta (a dos aguas o plana) sigue el mismo camino, pero es un dato del proyecto y no de un elemento: `appState.setRoof`, `PATCH /projects/{id}/structure/roof` y la columna `roof` de `projects`.
- Límite que conviene decir: es un dato visual, distinto de los Materiales del proyecto, que son partidas de presupuesto; no cambia el coste estimado.

## 23. Por qué las estructuras están en Python y no en C++

Porque la aplicación está escrita en Python y esas son las estructuras que usa. Al principio también había una versión en C++ en una carpeta aparte, compilada con CMake, pero la aplicación nunca la llamó: era una segunda implementación que había que mantener y probar por separado. Se retiró para que el proyecto tuviera un solo lenguaje en el backend y una sola versión que explicar.

Lo que se pierde: en Python no se ve la reserva y liberación manual de memoria. El punto 4 explica cómo lo resuelve el intérprete.

## 24. Por qué un array dinámico para ordenar los materiales

Porque el resultado se lee por posición: el asistente recibe los materiales en orden y el más caro es el índice 0, en O(1). Además no se sabe de antemano cuántos materiales tiene un proyecto, así que el array tiene que poder crecer.

El costo: cada material se inserta en su posición desplazando los que siguen, O(n) por inserción y O(n²) en total. Se acepta porque un proyecto tiene pocos materiales. Mostrar `backend/app/services/material_ranking.py`.

## 25. Por qué una lista simple en la ventana de mensajes y no una cola

El comportamiento sí es de cola: entra por el final y sale por el inicio. Pero la cola del proyecto no alcanza por dos razones:

- No se puede recorrer sin vaciarla, y la ventana hay que recorrerla entera para enviar los mensajes a la IA.
- Tiene capacidad fija y falla si se llena; la lista crece nodo a nodo.

La lista simple da `push_back` y `pop_front` en O(1) y un recorrido hacia adelante. No hace falta la lista doble porque nunca se recorre hacia atrás ni se quita por el final. Mostrar `backend/app/services/conversation_context.py`.

## 26. Por qué lista doble más pila en Deshacer y Rehacer, y no dos pilas

Deshacer necesita tres operaciones: agregar al final, quitar del final y descartar la más antigua cuando hay más de 20. La tercera es quitar por el fondo, y una pila no lo permite. La lista doble hace las tres en O(1).

Rehacer solo necesita apilar y desapilar, y nunca supera los 20 elementos, así que le basta una pila. Mostrar `record` y `record_restored` en `backend/app/services/undo_history.py`.

## 27. Qué pasa con 1 millón de elementos

No se midió: las mediciones de `docs/05_COMPLEJIDAD.md` llegan a 100 000 elementos. Lo que sigue sale del código y de esas mediciones, no de una prueba.

- Pila y cola: tienen capacidad fija (100 por defecto). Habría que crearlas con capacidad de un millón, y reservan todo el bloque al crearse. `push`, `pop`, `enqueue` y `dequeue` siguen siendo O(1).
- Array dinámico: agregar al final sigue siendo O(1) amortizado; partiendo de 4 posiciones, duplica su capacidad 18 veces. Insertar al inicio desplaza un millón de elementos cada vez.
- Listas: insertar en los extremos sigue siendo O(1). Buscar un valor es O(n): con 100 000 elementos tardó unos 11 milisegundos, así que con un millón se espera unas diez veces más. Cada elemento ocupa además un nodo propio.
- El orden de materiales por inserción, O(n²), dejaría de servir: habría que cambiarlo por un algoritmo de ordenamiento O(n log n).

En la aplicación ninguna estructura se acerca a ese tamaño: el historial guarda 20 eliminaciones por proyecto, la ventana 20 mensajes y la cola 5 intentos por correo.
