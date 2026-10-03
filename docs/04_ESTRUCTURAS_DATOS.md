# ESTRUCTURAS DE DATOS

## Array

Un array almacena elementos en posiciones consecutivas y permite acceder mediante un índice.

## Array dinámico

Un array dinámico utiliza memoria reservada durante la ejecución y debe gestionarse correctamente.

## Stack

Una pila utiliza LIFO: Last In, First Out.

Operaciones principales:

- Push
- Pop
- Peek
- IsEmpty

## Queue

Una cola utiliza FIFO: First In, First Out.

Operaciones principales:

- Enqueue
- Dequeue
- Peek
- IsEmpty

## Lista simplemente enlazada

Cada nodo contiene un dato y una referencia al siguiente nodo.

```text
[data | next] -> [data | next] -> [data | NULL]
```

## Lista doblemente enlazada

Cada nodo contiene un dato, una referencia anterior y una referencia siguiente.

```text
NULL <- [previous | data | next] <-> [previous | data | next] -> NULL
```

La práctica principal consiste en entender cómo cambian las referencias cuando se insertan o eliminan nodos.
