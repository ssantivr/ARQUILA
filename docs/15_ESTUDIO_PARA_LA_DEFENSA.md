# ESTUDIO PARA LA DEFENSA

Resumen para repasar antes de la presentación. Cada estructura tiene cinco líneas: qué es, cómo está implementada, un caso borde, su complejidad y dónde se usa en la aplicación. Las preguntas con sus respuestas están en `09_GUIA_DEFENSA.md` y los diagramas de nodos en `04_ESTRUCTURAS_DATOS.md`.

Todas las estructuras están en `backend/app/data_structures/` y sus pruebas en `backend/tests/test_data_structures.py`.

## Array unidimensional

- **Qué es.** Elementos en posiciones consecutivas, a los que se llega por su índice.
- **Cómo está implementado.** `linear_search` y `binary_search` en `arrays.py` reciben una secuencia y devuelven el índice del valor.
- **Caso borde.** Si el valor no está, o el array está vacío, las dos búsquedas devuelven `-1`.
- **Complejidad.** Acceso por índice O(1), búsqueda lineal O(n), búsqueda binaria O(log n) sobre un array ordenado.
- **Dónde se usa.** `polygon_area` en `backend/app/services/geometry.py` recorre los vértices de un lote para calcular su área. Las dos búsquedas no se usan en la aplicación, solo en las pruebas.

## Array dinámico

- **Qué es.** Un array que crece cuando se llena.
- **Cómo está implementado.** `DynamicArray` en `arrays.py`: un bloque de 4 posiciones y un contador; `_resize` crea un bloque del doble y copia los elementos.
- **Caso borde.** Insertar con el bloque lleno obliga a redimensionar antes de desplazar. Un índice fuera de rango produce `IndexError`.
- **Complejidad.** Acceso por índice O(1), agregar al final O(1) amortizado, insertar o eliminar en una posición O(n).
- **Dónde se usa.** `rank_by_cost` en `backend/app/services/material_ranking.py` ordena los materiales del más caro al más barato para el asistente.

## Stack

- **Qué es.** Una pila: el último que entra es el primero que sale (LIFO).
- **Cómo está implementada.** `Stack` en `stack.py`: un array de capacidad fija (100 por defecto) y el contador `_size`, que indica la cima.
- **Caso borde.** `pop` y `peek` en una pila vacía producen `IndexError`; `push` en una llena produce `OverflowError`.
- **Complejidad.** `push`, `pop` y `peek` son O(1).
- **Dónde se usa.** «Rehacer», en `backend/app/services/undo_history.py`: cada elemento restaurado con «Deshacer» se apila.

## Queue

- **Qué es.** Una cola: el primero que entra es el primero que sale (FIFO).
- **Cómo está implementada.** `Queue` en `queue.py`: un array circular de capacidad fija, el índice del frente y un contador. Los índices avanzan con el operador módulo.
- **Caso borde.** `dequeue` en una cola vacía produce `IndexError`; `enqueue` en una llena produce `OverflowError`. Después de dar la vuelta al array, el orden se mantiene.
- **Complejidad.** `enqueue`, `dequeue` y `peek` son O(1).
- **Dónde se usa.** El límite de intentos de inicio de sesión, en `backend/app/services/login_limiter.py`: una cola de 5 posiciones por correo con la hora de cada fallo.

## Lista simplemente enlazada

- **Qué es.** Nodos con un dato y una referencia al siguiente (`next`). Solo se recorre hacia adelante.
- **Cómo está implementada.** `SinglyLinkedList` en `singly_linked_list.py`, con `head`, `tail` y un contador. `tail` permite insertar al final en O(1).
- **Caso borde.** Al eliminar la cola, `tail` pasa al nodo anterior; al quitar el único nodo, `head` y `tail` vuelven a `None`.
- **Complejidad.** Insertar al inicio o al final y eliminar al inicio O(1); buscar, eliminar un valor, insertar en una posición e invertir O(n).
- **Dónde se usa.** La ventana de los últimos 20 mensajes que se envían a la IA, en `backend/app/services/conversation_context.py`.

## Lista doblemente enlazada

- **Qué es.** Nodos con un dato y dos referencias, al anterior (`previous`) y al siguiente (`next`). Se recorre en los dos sentidos.
- **Cómo está implementada.** `DoublyLinkedList` en `doubly_linked_list.py`, con `head`, `tail` y un contador. Todas las eliminaciones pasan por `_unlink`.
- **Caso borde.** Si el nodo eliminado no tiene anterior, era la cabeza y `head` pasa al siguiente; si no tiene siguiente, era la cola y `tail` pasa al anterior.
- **Complejidad.** Insertar y eliminar en los dos extremos O(1); buscar, eliminar un valor e insertar en una posición O(n).
- **Dónde se usa.** El historial de «Deshacer», en `backend/app/services/undo_history.py`: se agrega al final, se deshace desde el final y se descarta la más antigua por el inicio.

## Archivos que cada integrante debe poder explicar sin mirarlos

Integrantes: [INTEGRANTES]

Las estructuras:

- `backend/app/data_structures/arrays.py`
- `backend/app/data_structures/stack.py`
- `backend/app/data_structures/queue.py`
- `backend/app/data_structures/singly_linked_list.py`
- `backend/app/data_structures/doubly_linked_list.py`

Dónde se usan:

- `backend/app/services/geometry.py`
- `backend/app/services/material_ranking.py`
- `backend/app/services/undo_history.py`
- `backend/app/services/login_limiter.py`
- `backend/app/services/conversation_context.py`

Cómo se prueban y se miden:

- `backend/tests/test_data_structures.py`
- `backend/tests/test_structure_usage.py`
- `backend/app/benchmark.py`

De cada archivo de estructuras hay que poder decir, sin leerlo: qué atributos guarda, qué hace cada método paso a paso, qué ocurre en los casos borde y cuánto cuesta cada operación. De cada archivo de uso: qué operaciones de la estructura llama y por qué esa estructura y no otra.
