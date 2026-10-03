# COMPLEJIDAD BÁSICA

Como el código no lleva comentarios, la complejidad de cada operación se documenta aquí. Las tablas aplican tanto a las implementaciones en C++ (`data_structures/`) como a las de Python (`backend/app/data_structures/`), salvo donde se indica.

`n` es el número de elementos almacenados.

## Array

| Operación | Tiempo | Notas |
|---|---|---|
| Acceso por índice | O(1) | |
| Recorrido | O(n) | |
| Búsqueda lineal | O(n) | No requiere orden. |
| Búsqueda binaria | O(log n) | Requiere el array ordenado de forma ascendente. |
| Insertar en una posición | O(n) | Desplaza los elementos hacia la derecha. |
| Eliminar en una posición | O(n) | Desplaza los elementos hacia la izquierda. |

El área de un lote con forma libre se calcula recorriendo una vez el array de sus vértices (fórmula del área de Gauss), es decir, en O(n). Ver `backend/app/services/geometry.py`.

## Array dinámico

| Operación | Tiempo | Notas |
|---|---|---|
| Agregar al final | O(1) amortizado | Cuando se llena, duplica su capacidad. |
| Redimensionar | O(n) | Reserva un bloque mayor y copia los elementos. |
| Insertar o eliminar en una posición | O(n) | |

## Stack (LIFO)

Implementada sobre un array de capacidad fija.

| Operación | Tiempo |
|---|---|
| Push | O(1) |
| Pop | O(1) |
| Peek | O(1) |
| Tamaño, vacía, llena | O(1) |

## Queue (FIFO)

Implementada como cola circular sobre un array de capacidad fija: las posiciones que libera `dequeue` se reutilizan, así que la cola solo se reporta llena cuando realmente contiene tantos elementos como su capacidad.

| Operación | Tiempo |
|---|---|
| Enqueue | O(1) |
| Dequeue | O(1) |
| Peek | O(1) |
| Tamaño, vacía, llena | O(1) |

## Lista simplemente enlazada

Mantiene puntero a la cabeza y a la cola.

| Operación | Tiempo | Notas |
|---|---|---|
| Insertar al inicio | O(1) | |
| Insertar al final | O(1) | Gracias al puntero a la cola. |
| Insertar en una posición | O(n) | |
| Eliminar al inicio | O(1) | |
| Eliminar un valor | O(n) | Elimina la primera aparición. |
| Buscar | O(n) | |
| Invertir | O(n) | Usa O(1) de memoria adicional. |
| Tamaño | O(1) | Se mantiene un contador. |

## Lista doblemente enlazada

| Operación | Tiempo | Notas |
|---|---|---|
| Insertar al inicio o al final | O(1) | |
| Insertar en una posición | O(n) | |
| Eliminar al inicio o al final | O(1) | |
| Eliminar un valor | O(n) | Encontrado el nodo, desenlazarlo es O(1). |
| Buscar | O(n) | |
| Recorrer hacia adelante o hacia atrás | O(n) | |
| Tamaño | O(1) | Se mantiene un contador. |

## Uso dentro del backend

El historial para deshacer eliminaciones (`backend/app/services/undo_history.py`) usa una lista doblemente enlazada por proyecto. Necesita tres operaciones, y las tres son O(1) en esa estructura:

| Acción | Operación de la lista |
|---|---|
| Registrar una eliminación | Insertar al final |
| Deshacer la última eliminación | Eliminar al final |
| Descartar la más antigua al superar el límite | Eliminar al inicio |

Una pila sola no serviría, porque no permite descartar el elemento más antiguo.

El límite de intentos de inicio de sesión (`backend/app/services/login_limiter.py`) usa una cola por correo que guarda la hora de cada intento fallido. La capacidad de la cola es el máximo de intentos permitidos.

| Acción | Operación de la cola |
|---|---|
| Registrar un intento fallido | Enqueue |
| Descartar los intentos que ya salieron de la ventana de un minuto | Peek y dequeue mientras el más antiguo haya caducado |
| Saber si el correo está bloqueado | Comprobar si la cola está llena |

La cola sirve porque los intentos caducan en el mismo orden en que ocurrieron: el más antiguo siempre está al frente (FIFO). Cada operación es O(1); descartar caducados cuesta como máximo tantos pasos como la capacidad, que es fija.

La complejidad depende de la operación y de la implementación utilizada.
