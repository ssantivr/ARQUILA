# CONCLUSIONES

ARQUILA se concentra en las estructuras de datos estudiadas actualmente: arrays, pilas, colas, listas simples y listas dobles. El proyecto prioriza comprenderlas correctamente.

Las estructuras se implementan de forma independiente, en C++, para facilitar su análisis, prueba y explicación durante la presentación final.

Una estructura no se elige solo por saber implementarla, sino por lo que el sistema necesita. La aplicación lo muestra en dos casos:

- El historial de «Deshacer» usa una lista doblemente enlazada porque necesita agregar y quitar por el final, y descartar por el inicio.
- El límite de intentos de inicio de sesión usa una cola porque los intentos caducan en el mismo orden en que ocurrieron.

La arquitectura separa frontend, API, lógica de negocio, estructuras de datos, persistencia y pruebas, de modo que cada parte se puede explicar y comprobar por separado.

La aplicación incluye partes que van más allá del contenido de la asignatura, como el inicio de sesión, la subida de archivos y el asistente de IA. Son el contexto en el que se usan las estructuras, no el objeto de estudio.

Las estructuras más avanzadas pueden incorporarse posteriormente cuando formen parte del contenido académico de la asignatura.
