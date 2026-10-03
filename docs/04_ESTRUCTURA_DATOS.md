# ESTRUCTURAS DE DATOS

Este proyecto se enfoca únicamente en los conceptos trabajados hasta este punto:

1. Arrays unidimensionales.
2. Arrays dinámicos.
3. Pilas (Stack) con LIFO.
4. Colas (Queue) con FIFO.
5. Listas simplemente enlazadas.
6. Listas doblemente enlazadas.

## Arrays

Permiten almacenar elementos en posiciones identificadas por índices.

El acceso mediante índice es directo.

## Stack

Una pila utiliza el principio LIFO:

Last In, First Out.

La operación `push` agrega un elemento y `pop` retira el elemento superior.

## Queue

Una cola utiliza el principio FIFO:

First In, First Out.

La operación `enqueue` agrega un elemento al final y `dequeue` retira el elemento del frente.

## Lista simple

Cada nodo contiene un dato y una referencia al siguiente nodo.

```text
[data | next] -> [data | next] -> [data | NULL]
```

## Lista doble

Cada nodo contiene un dato, una referencia al nodo anterior y una referencia al siguiente.

```text
NULL <- [previous | data | next] <-> [previous | data | next] -> NULL
```

## Objetivo académico

El objetivo es comprender cómo funcionan estas estructuras y poder explicar sus operaciones, referencias y costos antes de avanzar a estructuras más complejas.
