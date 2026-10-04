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

En ese modo las tablas se crean aplicando las migraciones de `database/migrations/`, de modo que también se comprueba que el esquema coincide con los modelos del backend.

Antes de cada prueba se vacían todas las tablas de esa base. Por seguridad, las pruebas se niegan a ejecutarse si el nombre de la base no termina en `test`. Nunca se debe apuntar a una base con datos reales.

Verificado el 3 de octubre de 2026 con PostgreSQL 16.2 y 18.6: todas las pruebas pasan.

Cubren:

- El endpoint `/health`.
- Las estructuras de datos en Python: orden LIFO y FIFO, estructura vacía y llena, reutilización de posiciones en la cola circular, inserción y eliminación en las listas, y búsquedas.
- Registro, inicio y cierre de sesión, y que cada endpoint exija sesión.
- Que un usuario no pueda ver ni modificar los datos de otro.
- Crear, listar, actualizar y eliminar proyectos, terrenos, materiales, planos y elevaciones, con sus validaciones.
- Subida de archivos: tipos admitidos, tamaño máximo y adjuntarlos a planos y elevaciones.
- Recomendaciones automáticas, con la prioridad que asigna cada regla, y deshacer eliminaciones.
- Cuartos: crear, listar, actualizar y eliminar, que las medidas sean mayores que cero, que el plano sea del mismo proyecto y que se eliminen con su plano o su proyecto.
- Componentes estructurales: crear, listar y filtrar por tipo, actualizar y eliminar, que el tipo sea columna, viga o muro, que las medidas sean mayores que cero, que el plano sea del mismo proyecto y que se eliminen con su plano o su proyecto.
- El modelo 3D (`/projects/{id}/structure`): colocación de los terrenos, qué planos cuentan como nivel, cuartos frente a volumen del nivel, altura de cada nivel, y posición de los componentes, con las vigas colgadas del techo del nivel. También el material de superficie de cada elemento: que se guarde en un cuarto, en un componente y en el plano de un volumen, que se rechace un material o un tipo desconocido, que el elemento sea de ese tipo y de ese proyecto, y que otro usuario no pueda cambiarlo. Y el tipo de cubierta del proyecto: a dos aguas por defecto, que se guarde la plana, que se rechace otro valor y que otro usuario no pueda cambiarla.
- Proyectos de ejemplo: la lista, la creación de un proyecto completo desde un ejemplo, el nombre libre al repetirlo, los permisos, y que los cuartos de cada ejemplo caben en su lote sin solaparse.
- CORS: que se admita el origen de la aplicación con credenciales, que se responda la petición previa y que se ignoren otros orígenes.
- Conversaciones con el asistente, usando un asistente simulado, y la respuesta por reglas cuando la IA falla o no está configurada.
- Los adaptadores de correo y de IA (`test_adapters.py`), sustituyendo `smtplib` y el SDK de Anthropic por objetos simulados: qué adaptador de correo se elige según la configuración, las llamadas SMTP que hace, y cómo el adaptador de IA extrae el texto, trata las negativas y las respuestas vacías y convierte los errores del SDK en el error de la aplicación. El adaptador del modelo local se prueba contra un servidor de Ollama simulado: qué modelo elige, qué envía, y qué hace si Ollama no responde, no tiene modelos o contesta algo inesperado.

### Frontend

Las funciones de cálculo y de texto del frontend tienen pruebas en `frontend/src/utils/*.test.ts`, `frontend/src/state/*.test.ts` y `frontend/src/three/*.test.ts`, que se ejecutan con `npm test`: área, vista frontal, curvas de nivel, cotas de cada lado, área edificable y lectura de los vértices de un lote, agrupación de cuartos por nivel e indicadores de ocupación (huella, área construida, COS y CUS), colocación de ventanas y puerta, forma del techo (a dos aguas o plano), fachadas y corte generados con cada cubierta, geometría de un cuarto en el modelo 3D (muros dentro de su medida, vidrio, hoja de puerta, líneas de cada vano y coordenadas de textura en metros), colores del modelo 3D (por tipo, por coste estimado y por alertas), materiales de superficie (el elegido o el predeterminado de cada tipo, y la lectura de los guardados), el estado compartido `appState` (avisos a los suscriptores, cambio de proyecto, material de superficie por elemento y recarga con peticiones simuladas), traducción de los mensajes de error y formato de números.

Las pantallas se revisaron a mano en un navegador automatizado, pero no tienen pruebas automáticas guardadas en el repositorio.

### Instalación desde cero

Comprobada el 3 de octubre de 2026 con un clon nuevo de la rama `main`, siguiendo los pasos del `README.md` en Windows 11 con Python 3.12, Node 24 y PostgreSQL:

- Backend: entorno virtual, `pip install -r requirements-dev.txt` y `pytest`. Las 216 pruebas pasan.
- Frontend: `npm install`, `npm test` (29 pruebas) y `npm run build`.
  - Desde entonces se añadieron pruebas: al 4 de octubre de 2026 `npm test` ejecuta 70 y `pytest` 225, y todas pasan (el backend, tanto con SQLite como con PostgreSQL). Ese dato es del equipo de desarrollo; la instalación desde un clon nuevo no se repitió.
- Base de datos: sobre una base vacía, `python -m app.migrate --seed` aplicó las seis migraciones y cargó los datos de ejemplo. Una segunda ejecución no aplicó ni duplicó nada.
- Aplicación: con esa base, el usuario de demostración inicia sesión y ve sus tres proyectos con sus datos; generar recomendaciones y preguntar al asistente funcionan.

- Estructuras en C++: los cinco programas de `data_structures/` compilan con GCC 16.2 (`g++ -std=c++17 -static -Wall -Wextra -Wpedantic`) sin ningún aviso, y al ejecutarlos su salida coincide con los casos de este documento: la pila devuelve 30, 20, 10; la cola 10, 20, 30 y sigue funcionando después de dar la vuelta; las listas insertan, eliminan y se recorren en los dos sentidos. No se analizó el uso de memoria con una herramienta, porque GCC para Windows no incluye ese análisis.

No se comprobó el arranque con `docker-compose.yml`, porque el equipo no tiene Docker.

En Windows, si la carpeta del proyecto está en una ruta muy larga, `pip install` puede fallar con el error «No such file or directory» en un archivo del paquete `anthropic`. Se resuelve clonando el proyecto en una ruta corta.

### Sin cobertura automática

La llamada real al servicio de IA, el envío real de correo y las pantallas del frontend. Las dos primeras se comprueban a mano con `python -m app.check`, que necesita credenciales reales (ver `12_BACKEND_Y_API.md`).
