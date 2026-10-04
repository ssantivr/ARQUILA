# CONCLUSIONES

ARQUILA se concentra en las estructuras de datos estudiadas actualmente: arrays, pilas, colas, listas simples y listas dobles. El proyecto prioriza comprenderlas correctamente.

Las estructuras se implementan dos veces: en C++, como plantillas independientes con sus propias pruebas, para facilitar su análisis y explicación durante la presentación final; y en Python, dentro del backend, que es la versión que usa la aplicación.

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

Las estructuras más avanzadas pueden incorporarse posteriormente cuando formen parte del contenido académico de la asignatura.
