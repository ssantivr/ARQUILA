# CONCLUSIONES

ARQUILA se concentra en las estructuras de datos estudiadas actualmente: arrays, pilas, colas, listas simples y listas dobles. El proyecto prioriza comprenderlas correctamente.

Las estructuras están implementadas desde cero en Python, en `BACKEND-ARQUILA/app/data_structures/`, sin librerías que las reemplacen. Es la única versión: la que usa la aplicación es la misma que se prueba y se explica.

Una estructura no se elige solo por saber implementarla, sino por lo que el sistema necesita. La aplicación lo muestra en cinco casos:

- El historial de «Deshacer» usa una lista doblemente enlazada porque necesita agregar y quitar por el final, y descartar por el inicio.
- «Rehacer» usa una pila porque lo último que se deshizo es lo primero que se rehace.
- El límite de intentos de inicio de sesión usa una cola porque los intentos caducan en el mismo orden en que ocurrieron.
- El historial que se envía al asistente usa una lista simplemente enlazada como ventana: se agrega al final y se descarta por el inicio.
- El orden de los materiales por costo usa un array dinámico, que crece cuando se llena.

Las mediciones de `05_COMPLEJIDAD.md` confirman con tiempos reales lo que predice la teoría: las operaciones O(1) no dependen del tamaño y las O(n) crecen en proporción a él.

La arquitectura separa frontend, API, lógica de negocio, estructuras de datos, persistencia y pruebas, de modo que cada parte se puede explicar y comprobar por separado.

La aplicación incluye partes que van más allá del contenido de la asignatura, como el inicio de sesión, la subida de archivos y el asistente de IA. Son el contexto en el que se usan las estructuras, no el objeto de estudio.

El asistente dejó dos lecciones. La primera, sobre diseño: como los servicios solo conocen una interfaz (patrón Adapter), se pudo añadir un modelo local gratuito sin tocar la lógica de las conversaciones. La segunda, sobre confiar en una IA: un modelo pequeño se equivoca al hacer cuentas, así que los cálculos los hace el backend y el modelo solo los explica; y cuando no hay IA, unas reglas fijas dan una respuesta peor redactada pero exacta.

## Limitaciones y trabajo futuro

Lo que no se hizo, sin orden de importancia:

- **Estructuras.** La pila y la cola tienen capacidad fija y fallan si se llenan. No hay árboles, grafos ni tablas hash propias; donde hace falta buscar por clave (un historial por proyecto, una cola por correo) se usa el diccionario de Python. La búsqueda lineal y la binaria no se usan en la aplicación, solo en las pruebas y en las mediciones.
- **Orden de materiales.** Es por inserción, O(n²). Sirve para los pocos materiales de un proyecto, no para miles.
- **Mediciones.** Llegan a 100 000 elementos y se hicieron en un solo equipo. No se midió el uso de memoria.
- **Memoria manual.** Al retirar la versión en C++, el proyecto ya no muestra la reserva y liberación manual de memoria.
- **Deshacer.** Solo cubre terrenos, planos, elevaciones y materiales; los cuartos y los componentes estructurales no. El historial vive en la memoria del servidor: se pierde al reiniciarlo. Lo mismo ocurre con el límite de intentos de inicio de sesión.
- **Asistente.** Solo se probó con el modelo local de Ollama y con las reglas fijas; no hay un proveedor de IA en la nube.
- **Correo.** No hay un servicio de correo configurado: el enlace de recuperación de contraseña se escribe en la consola del servidor.
- **Despliegue.** La aplicación solo se ejecutó en el equipo local. No se desplegó en un servidor.
- **Modelo 3D.** No representa la pendiente del terreno ni comprueba que un cuarto quede dentro del lote. No hay cálculo estructural.
- **Pruebas del frontend.** La mayoría de las pantallas no tiene pruebas automáticas.

Como trabajo futuro, los hitos propuestos están en `13_EVOLUCION_POR_SEMANAS.md`. Las estructuras más avanzadas pueden incorporarse cuando formen parte del contenido de la asignatura.
