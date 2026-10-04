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
- `data_structures/` contiene las estructuras en C++ como material de estudio, independientes de la aplicación.

Dentro del backend se repite la misma idea por capas: `api` recibe la petición, `services` aplica la lógica y `repositories` consulta la base. Ver `docs/03_ARQUITECTURA.md` y `docs/10_BACKEND_Y_API.md`.

## 3. Cómo funciona un array

Guarda los elementos en posiciones consecutivas de memoria. Por eso llegar a una posición por su índice es inmediato, O(1), pero insertar o eliminar en medio obliga a desplazar los elementos que siguen, O(n).

Mostrar `insertAt` y `removeAt` en `data_structures/arrays/ArrayExamples.cpp`.

## 4. Qué significa memoria dinámica

Es memoria que se reserva mientras el programa se ejecuta, con `new`, y que el programa debe liberar con `delete`. Permite decidir el tamaño en tiempo de ejecución.

En `ArrayExamples.cpp`, cuando el array dinámico se llena, `resize` reserva un bloque más grande, copia los elementos y libera el bloque anterior. En las listas, cada nodo se reserva con `new` y el destructor los libera todos con `clear`.

## 5. Por qué Stack utiliza LIFO

Porque solo se trabaja por un extremo, la cima: el último elemento que entra es el primero que sale. `push` y `pop` solo mueven el índice `top`, por eso son O(1).

Mostrar `data_structures/stack/Stack.cpp`. `pop` devuelve `false` si la pila está vacía, en lugar de un valor especial, para poder guardar cualquier entero.

## 6. Por qué Queue utiliza FIFO

Porque se inserta por un extremo y se retira por el otro: el primero que entra es el primero que sale.

La cola es circular: el índice avanza con el operador módulo y reutiliza las posiciones que quedan libres al retirar elementos. Sin eso, después de 100 inserciones la cola se reportaría llena aunque estuviera vacía. Mostrar `data_structures/queue/Queue.cpp`.

## 7. Cómo funciona el puntero `next` de una lista simple

Cada nodo guarda un dato y la dirección del nodo siguiente; el último apunta a `nullptr`. Para recorrer la lista se empieza en `head` y se sigue `next` hasta llegar a `nullptr`.

Solo se puede avanzar. Para eliminar un nodo hay que estar parado en el anterior, porque es su `next` el que se debe cambiar. Mostrar `remove` en `data_structures/singly_linked_list/SinglyLinkedList.cpp`.

## 8. Cómo funcionan `previous` y `next` en una lista doble

Cada nodo conoce a su vecino anterior y al siguiente, así que la lista se puede recorrer en los dos sentidos y se puede retirar un nodo teniendo solo ese nodo.

El costo es más memoria por nodo y más punteros que mantener en cada operación. Mostrar `printForward` y `printBackward` en `data_structures/doubly_linked_list/DoublyLinkedList.cpp`.

## 9. Qué ocurre cuando se elimina un nodo

En la lista doble, la función `unlink` hace tres cosas:

1. El `next` del nodo anterior pasa a apuntar al nodo siguiente. Si no hay anterior, el nodo era la cabeza y `head` pasa al siguiente.
2. El `previous` del nodo siguiente pasa a apuntar al anterior. Si no hay siguiente, el nodo era la cola y `tail` pasa al anterior.
3. Se libera la memoria del nodo con `delete` y se descuenta del contador.

Los casos que hay que saber explicar son: eliminar la cabeza, eliminar la cola, eliminar el único nodo y eliminar uno intermedio.

## 10. Qué significa O(1) y O(n) en los ejemplos del proyecto

- O(1): el trabajo no depende de cuántos elementos haya. Ejemplos: `push` y `pop` de la pila, `enqueue` y `dequeue` de la cola, insertar al inicio de una lista.
- O(n): el trabajo crece con la cantidad de elementos. Ejemplos: buscar un valor en una lista, insertar en medio de un array.
- O(log n): la búsqueda binaria descarta la mitad de los elementos en cada paso, pero exige que el array esté ordenado.

La tabla completa está en `docs/05_COMPLEJIDAD.md`.

## 11. Dónde se usa una estructura de datos dentro de la aplicación

El botón «Deshacer» del proyecto usa una lista doblemente enlazada (`backend/app/services/undo_history.py`). Cada eliminación se agrega al final; deshacer saca la última; y cuando hay más de 20 se descarta la más antigua por el inicio. Las tres operaciones son O(1).

Una pila no alcanzaría, porque no permite quitar el elemento más antiguo.

El límite de intentos de inicio de sesión usa una cola (`backend/app/services/login_limiter.py`). Cada intento fallido se encola con su hora; los que tienen más de un minuto se retiran por el frente, porque el más antiguo es siempre el primero en caducar (FIFO). Si la cola está llena, hay 5 fallos recientes y el inicio de sesión se bloquea.

## 12. Cómo se validó el backend

Con pruebas automatizadas en `backend/tests/`, que se ejecutan con `pytest`. Comprueban las estructuras de datos, el inicio de sesión, los permisos entre usuarios y cada operación de la API. Ver `docs/06_PRUEBAS.md`, que también indica lo que no está cubierto.

Las estructuras en C++ se validan ejecutando cada programa y comparando su salida con la esperada.

## 13. Qué decisiones se tomaron para mantener el proyecto dentro del alcance de la asignatura

- Solo se usan las estructuras estudiadas: arrays, pila, cola y listas enlazadas. No hay árboles, grafos ni tablas hash propias.
- Las estructuras tienen capacidad fija o enlaces simples, sin plantillas ni optimizaciones avanzadas en C++.
- La aplicación sí incluye partes que van más allá de la asignatura (inicio de sesión, subida de archivos, asistente de IA). Siguen el plan de `06_EVOLUCION_POR_SEMANAS.md`, pero no son el centro de la defensa: conviene presentarlas como contexto y concentrar la explicación en las estructuras y en el punto 11.

## 14. Qué patrón de diseño se usa y por qué

El patrón Adapter, en los servicios externos del backend. El asistente necesita «dame una respuesta para esta conversación», pero cada proveedor se llama de una forma distinta. Un adaptador es una clase que ofrece la forma que la aplicación espera y la traduce a la del proveedor.

- La interfaz es `Assistant`, con un solo método: `reply(system, messages)`.
- `ClaudeAssistant` la traduce a las llamadas del SDK de Anthropic.
- `OllamaAssistant` la traduce a peticiones HTTP a un modelo local.

`ConversationService` solo conoce la interfaz. Por eso se añadió el modelo local sin tocar ese servicio, y las pruebas usan un asistente simulado en lugar de uno real. El correo sigue la misma idea con `SmtpMailer` y `ConsoleMailer`.

Mostrar `backend/app/ai.py`. Ver la tabla de `docs/10_BACKEND_Y_API.md`.

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
