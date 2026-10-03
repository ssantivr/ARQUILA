# PRUEBAS

El proyecto utiliza pruebas sencillas porque el objetivo es validar los conceptos aprendidos y no construir una plataforma de testing avanzada.

## Casos

### Stack

- Insertar varios valores.
- Consultar el elemento superior.
- Retirar valores respetando LIFO.
- Comprobar estado vacío.

### Queue

- Insertar varios valores.
- Consultar el frente.
- Retirar valores respetando FIFO.
- Comprobar estado vacío.

### Lista simple

- Insertar al inicio.
- Insertar al final.
- Eliminar un valor.
- Recorrer la estructura.

### Lista doble

- Insertar al inicio.
- Insertar al final.
- Eliminar un valor.
- Recorrer hacia adelante.
- Recorrer hacia atrás.

### Backend

Las pruebas automatizadas están en `backend/tests/` y se ejecutan con `pytest`. Usan una base SQLite en memoria, así que no necesitan PostgreSQL.

Cubren:

- El endpoint `/health`.
- Las estructuras de datos en Python: orden LIFO y FIFO, estructura vacía y llena, reutilización de posiciones en la cola circular, inserción y eliminación en las listas, y búsquedas.
- Registro, inicio y cierre de sesión, y que cada endpoint exija sesión.
- Que un usuario no pueda ver ni modificar los datos de otro.
- Crear, listar, actualizar y eliminar proyectos, terrenos, materiales, planos y elevaciones, con sus validaciones.
- Subida de archivos: tipos admitidos, tamaño máximo y adjuntarlos a planos y elevaciones.
- Recomendaciones automáticas y deshacer eliminaciones.
- Conversaciones con el asistente, usando un asistente simulado.

No están cubiertos por pruebas automáticas: la llamada real al servicio de IA, la ejecución contra PostgreSQL y la interfaz del frontend.
