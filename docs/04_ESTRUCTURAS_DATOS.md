# ESTRUCTURAS DE DATOS

Este proyecto se enfoca únicamente en los conceptos trabajados hasta este punto:

1. Arrays unidimensionales.
2. Arrays dinámicos.
3. Pilas (Stack) con LIFO.
4. Colas (Queue) con FIFO.
5. Listas simplemente enlazadas.
6. Listas doblemente enlazadas.

No se usan árboles, grafos ni tablas hash propias.

## Array

Un array almacena elementos en posiciones consecutivas y permite acceder a cada uno mediante un índice. El acceso por índice es directo.

## Array dinámico

Un array dinámico utiliza memoria reservada durante la ejecución, que el programa debe liberar. Cuando se llena, reserva un bloque más grande y copia los elementos.

## Stack

Una pila utiliza el principio LIFO: Last In, First Out.

Operaciones principales:

- `push` agrega un elemento en la cima.
- `pop` retira el elemento de la cima.
- `peek` consulta la cima sin retirarla.
- `isEmpty` indica si está vacía.

## Queue

Una cola utiliza el principio FIFO: First In, First Out.

Operaciones principales:

- `enqueue` agrega un elemento al final.
- `dequeue` retira el elemento del frente.
- `peek` consulta el frente sin retirarlo.
- `isEmpty` indica si está vacía.

La cola del proyecto es circular: reutiliza las posiciones que quedan libres al retirar elementos.

## Lista simplemente enlazada

Cada nodo contiene un dato y una referencia al siguiente nodo.

```text
[data | next] -> [data | next] -> [data | NULL]
```

## Lista doblemente enlazada

Cada nodo contiene un dato, una referencia al nodo anterior y una referencia al siguiente.

```text
NULL <- [previous | data | next] <-> [previous | data | next] -> NULL
```

La práctica principal consiste en entender cómo cambian las referencias cuando se insertan o eliminan nodos.

## Dónde están

| Estructura | C++ | Python |
|---|---|---|
| Array y array dinámico | `data_structures/arrays/ArrayExamples.cpp` | `backend/app/data_structures/arrays.py` |
| Stack | `data_structures/stack/Stack.cpp` | `backend/app/data_structures/stack.py` |
| Queue | `data_structures/queue/Queue.cpp` | `backend/app/data_structures/queue.py` |
| Lista simple | `data_structures/singly_linked_list/SinglyLinkedList.cpp` | `backend/app/data_structures/singly_linked_list.py` |
| Lista doble | `data_structures/doubly_linked_list/DoublyLinkedList.cpp` | `backend/app/data_structures/doubly_linked_list.py` |

La complejidad de cada operación y el uso de estas estructuras dentro de la aplicación están en `05_COMPLEJIDAD.md`.

## Objetivo académico

El objetivo es comprender cómo funcionan estas estructuras y poder explicar sus operaciones, referencias y costos antes de avanzar a estructuras más complejas.
