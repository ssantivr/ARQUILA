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

Las pruebas automatizadas están en `backend/tests/` y se ejecutan con `pytest`. Por defecto usan una base SQLite en memoria, así que no necesitan PostgreSQL.

### Ejecución contra PostgreSQL

Las mismas pruebas se pueden ejecutar contra un PostgreSQL real definiendo `TEST_DATABASE_URL`:

```bash
set TEST_DATABASE_URL=postgresql+psycopg://usuario:clave@localhost:5432/arquila_test
pytest
```

En ese modo las tablas se crean con `database/schema.sql`, de modo que también se comprueba que el esquema coincide con los modelos del backend.

Antes de cada prueba se vacían todas las tablas de esa base. Por seguridad, las pruebas se niegan a ejecutarse si el nombre de la base no termina en `test`. Nunca se debe apuntar a una base con datos reales.

Verificado el 3 de octubre de 2026 con PostgreSQL 16.2: las 126 pruebas pasan, y `schema.sql` y `seed.sql` se pueden aplicar dos veces seguidas sin errores.

Cubren:

- El endpoint `/health`.
- Las estructuras de datos en Python: orden LIFO y FIFO, estructura vacía y llena, reutilización de posiciones en la cola circular, inserción y eliminación en las listas, y búsquedas.
- Registro, inicio y cierre de sesión, y que cada endpoint exija sesión.
- Que un usuario no pueda ver ni modificar los datos de otro.
- Crear, listar, actualizar y eliminar proyectos, terrenos, materiales, planos y elevaciones, con sus validaciones.
- Subida de archivos: tipos admitidos, tamaño máximo y adjuntarlos a planos y elevaciones.
- Recomendaciones automáticas y deshacer eliminaciones.
- Conversaciones con el asistente, usando un asistente simulado.

No están cubiertos por pruebas automáticas: la llamada real al servicio de IA y la interfaz del frontend.
