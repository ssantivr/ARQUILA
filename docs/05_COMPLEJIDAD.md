# COMPLEJIDAD BÁSICA

Como el código no lleva comentarios, la complejidad de cada operación se documenta aquí. Las tablas describen las implementaciones de `BACKEND-ARGUILA-/app/data_structures/`.

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

El área de un lote con forma libre se calcula recorriendo una vez el array de sus vértices (fórmula del área de Gauss), es decir, en O(n). Ver `BACKEND-ARGUILA-/app/services/geometry.py`.

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

El historial para deshacer eliminaciones (`BACKEND-ARGUILA-/app/services/undo_history.py`) usa una lista doblemente enlazada por proyecto. Necesita tres operaciones, y las tres son O(1) en esa estructura:

| Acción | Operación de la lista |
|---|---|
| Registrar una eliminación | Insertar al final |
| Deshacer la última eliminación | Eliminar al final |
| Descartar la más antigua al superar el límite | Eliminar al inicio |

Una pila sola no serviría, porque no permite descartar el elemento más antiguo.

El límite de intentos de inicio de sesión (`BACKEND-ARGUILA-/app/services/login_limiter.py`) usa una cola por correo que guarda la hora de cada intento fallido. La capacidad de la cola es el máximo de intentos permitidos.

| Acción | Operación de la cola |
|---|---|
| Registrar un intento fallido | Enqueue |
| Descartar los intentos que ya salieron de la ventana de un minuto | Peek y dequeue mientras el más antiguo haya caducado |
| Saber si el correo está bloqueado | Comprobar si la cola está llena |

La cola sirve porque los intentos caducan en el mismo orden en que ocurrieron: el más antiguo siempre está al frente (FIFO). Cada operación es O(1); descartar caducados cuesta como máximo tantos pasos como la capacidad, que es fija.

Rehacer (`BACKEND-ARGUILA-/app/services/undo_history.py`) usa una pila por proyecto. Cada vez que «Deshacer» restaura un elemento, se apila; rehacer saca el de la cima y lo vuelve a eliminar. Lo último que se deshizo es lo primero que se rehace (LIFO), y `push` y `pop` son O(1). Una eliminación nueva vacía la pila, porque lo deshecho antes ya no se puede rehacer. La pila nunca se llena: entre el historial y la pila no hay más de 20 elementos.

El contexto que se envía a la IA (`BACKEND-ARGUILA-/app/services/conversation_context.py`) usa una lista simplemente enlazada como ventana de los últimos 20 mensajes: cada mensaje se inserta al final y, al superar el límite, se elimina el del inicio. Las dos operaciones son O(1) y la lista se recorre una sola vez hacia adelante, así que no hace falta el puntero al nodo anterior.

El orden de los materiales para el asistente (`BACKEND-ARGUILA-/app/services/material_ranking.py`) usa un array dinámico. Cada material se inserta en la posición que le corresponde por costo, lo que desplaza los siguientes: O(n) por inserción y O(n²) en total. Es aceptable porque un proyecto tiene pocos materiales, y a cambio el más caro se consulta por índice en O(1).

La complejidad depende de la operación y de la implementación utilizada.

## Mediciones

La tabla de arriba es teoría. Para comprobarla, `BACKEND-ARGUILA-/app/benchmark.py` mide esas implementaciones con tres tamaños de entrada. Se ejecuta con:

```bash
cd BACKEND-ARGUILA-
python -m app.benchmark
```

Resultados del 4 de octubre de 2026 en el equipo de desarrollo (Windows 11, Python 3.12). Los tiempos son microsegundos por operación, tomando la mejor de cinco repeticiones. «Crece» es cuántas veces más tarda con 100 000 elementos que con 1 000, es decir, con 100 veces más datos.

| Estructura | Operación | Esperado | n = 1 000 | n = 10 000 | n = 100 000 | Crece |
|---|---|---|---:|---:|---:|---:|
| Array dinámico | Agregar al final | O(1) amortizado | 0,25 | 0,27 | 0,28 | ×1,1 |
| Array dinámico | Acceso por índice | O(1) | 0,08 | 0,08 | 0,08 | ×1,0 |
| Array dinámico | Insertar y eliminar al inicio | O(n) | 44,8 | 441 | 4349 | ×97,1 |
| Array | Búsqueda lineal | O(n) | 82,8 | 578 | 5869 | ×70,9 |
| Array | Búsqueda binaria | O(log n) | 1,38 | 1,97 | 2,40 | ×1,7 |
| Stack | Push y pop | O(1) | 0,21 | 0,22 | 0,23 | ×1,1 |
| Queue | Enqueue y dequeue | O(1) | 0,34 | 0,32 | 0,33 | ×1,0 |
| Lista simple | Insertar al final | O(1) | 0,36 | 0,41 | 0,51 | ×1,4 |
| Lista simple | Insertar y eliminar al inicio | O(1) | 0,24 | 0,24 | 0,24 | ×1,0 |
| Lista simple | Buscar un valor | O(n) | 103 | 1064 | 10980 | ×106,1 |
| Lista simple | Insertar en el medio | O(n) | 19,2 | 221 | 2304 | ×119,8 |
| Lista doble | Insertar al final | O(1) | 0,39 | 0,43 | 0,61 | ×1,6 |
| Lista doble | Insertar y eliminar al final | O(1) | 0,30 | 0,30 | 0,30 | ×1,0 |
| Lista doble | Buscar un valor | O(n) | 105 | 1069 | 10975 | ×104,4 |
| Lista doble | Insertar en el medio | O(n) | 19,2 | 215 | 2201 | ×114,6 |

Comparación con la teoría:

- **O(1).** Con 100 veces más datos, el tiempo por operación apenas cambia: entre ×1,0 y ×1,6. Las dos filas que más suben (insertar al final de una lista, ×1,4 y ×1,6) crean un nodo nuevo por elemento; con cien mil nodos, reservar memoria cuesta algo más, pero sigue muy lejos de crecer con el tamaño.
- **O(1) amortizado.** Agregar al final del array dinámico se mantiene en ×1,1 aunque de vez en cuando duplique su capacidad y copie todo: ese costo, repartido entre todas las inserciones, es constante.
- **O(n).** Con 100 veces más datos tardan entre 71 y 120 veces más, es decir, crecen en proporción al tamaño. La búsqueda lineal queda en ×71 porque con mil elementos el tiempo fijo de cada llamada todavía pesa; entre 10 000 y 100 000 ya crece ×10.
- **O(log n).** La búsqueda binaria solo tarda 1,7 veces más: pasar de 1 000 a 100 000 elementos añade unos 7 pasos a los 10 que ya hacía.

La diferencia práctica se ve en una misma fila: buscar un valor en una lista de 100 000 elementos tarda unos 11 milisegundos, y la búsqueda binaria en un array ordenado del mismo tamaño, 2,4 microsegundos.

Las cifras exactas cambian de un equipo a otro y entre ejecuciones; lo que se mantiene es cómo crecen. No se midió ningún tamaño mayor que 100 000 elementos.
