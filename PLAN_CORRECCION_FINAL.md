# PLAN DE CORRECCIÓN FINAL

Objetivo: el proyecto queda solo con backend en Python (FastAPI) y frontend en TypeScript (React). Se elimina C++ por completo.

- [x] Fase 1 - Verificar que Python cubre todo antes de borrar nada
- [x] Fase 2 - Eliminar C++
- [x] Fase 3 - Configuración y arranque
- [x] Fase 4 - README
- [ ] Fase 5 - Documentación
- [ ] Fase 6 - Comprobación final

## Fase 1: resultado

### Estructuras en Python y sus pruebas

| Estructura | Archivo | Pruebas |
|---|---|---|
| Array unidimensional | `backend/app/data_structures/arrays.py` (`linear_search`, `binary_search`) y `backend/app/services/geometry.py` (`polygon_area`) | `backend/tests/test_data_structures.py`, `backend/tests/test_terrain_points.py` |
| Array dinámico | `backend/app/data_structures/arrays.py` (`DynamicArray`) | `backend/tests/test_data_structures.py`, `backend/tests/test_structure_usage.py` |
| Stack | `backend/app/data_structures/stack.py` | `backend/tests/test_data_structures.py` |
| Queue | `backend/app/data_structures/queue.py` | `backend/tests/test_data_structures.py` |
| Lista simplemente enlazada | `backend/app/data_structures/singly_linked_list.py` | `backend/tests/test_data_structures.py` |
| Lista doblemente enlazada | `backend/app/data_structures/doubly_linked_list.py` | `backend/tests/test_data_structures.py` |

### Comparación con las pruebas de C++ (Catch2)

Ya estaban cubiertos en Python: orden LIFO y FIFO, `pop`, `peek` y `dequeue` en estructura vacía, estructura llena, reutilización de posiciones y vuelta completa de la cola, insertar al inicio, al final y en medio, eliminar cabeza, medio y cola, `pop_front` y `pop_back` en lista vacía, invertir la lista (también vacía), recorrido hacia atrás, búsquedas con resultado y sin él, crecimiento del array dinámico e índices fuera de rango.

Solo estaban en C++ y se añadieron a pytest:

| Caso | Prueba nueva |
|---|---|
| Eliminar en una lista vacía | `test_remove_on_empty_linked_list_returns_false` |
| Eliminar solo la primera aparición de un valor repetido | `test_remove_deletes_only_the_first_match` |
| Lista de un solo elemento que se vacía y se vuelve a usar | `test_single_element_list_empties_and_stays_usable` |
| Lista doble de un solo elemento, en los dos sentidos | `test_single_element_doubly_linked_list_reads_the_same_both_ways` |
| Lista larga (1000 nodos) tras eliminar la mitad | `test_long_linked_list_keeps_order_after_many_removals` |
| Tipos distintos de `int` | `test_structures_store_types_other_than_int`, `test_searches_work_with_strings` |

Casos borde que no estaban en ninguno de los dos y también se añadieron: `test_reverse_single_element_list_keeps_head_and_tail`, `test_dynamic_array_empties_and_is_reused` y `test_polygon_area_of_fewer_than_three_points_is_zero`.

No se trasladaron las pruebas de C++ sobre liberación de memoria (cada nodo destruido una sola vez): en Python la memoria la gestiona el intérprete y no hay destructores que probar.

### Comandos ejecutados el 4 de octubre de 2026

| Comando | Resultado |
|---|---|
| `pytest` (en `backend/`, con SQLite) | 283 pruebas pasan |
| `npm test` (en `frontend/`) | 118 pruebas pasan en 15 archivos |
| `npm run build` (en `frontend/`) | compila sin errores |
| `python scripts/quality.py` | todas las comprobaciones pasan |

No hubo fallos que corregir. `pytest` contra PostgreSQL no se ejecutó en esta fase.

## Fase 3: resultado

Comprobado el 4 de octubre de 2026 en un clon nuevo del repositorio (Windows 11, Python 3.12.10, Node 24.21.0, PostgreSQL 18.6), sobre una base de datos vacía creada para la prueba y borrada al terminar.

| Paso | Resultado |
|---|---|
| `python -m venv .venv` y `pip install -r requirements-dev.txt` | instala sin errores en una ruta corta; en una ruta muy larga falla, como ya avisa el README |
| `python -m app.migrate` y `python -m app.dev` sin `backend/.env` | terminan con el mensaje «DATABASE_URL is not set. Copy backend/.env.example to backend/.env and fill it in.» |
| `python -m app.migrate` (con `backend/.env`, sin `DATABASE_URL` en la terminal) | aplica las 9 migraciones; quedan 0 usuarios y 0 proyectos |
| `python -m app.migrate --seed` | carga 1 usuario y 3 proyectos; una segunda ejecución no duplica nada |
| `python -m app.check` | lee `backend/.env`; el asistente respondió con Ollama |
| `python -m app.dev` | `/health` responde `{"status":"ok"}` y `/docs` responde 200 |
| `npm install` y `npm run dev` | la interfaz responde 200; el usuario de demostración inicia sesión a través de `/api` y ve sus 3 proyectos |

`npm run dev` se probó en el puerto 5183 porque el 5173 estaba ocupado en el equipo. No se abrió la interfaz en un navegador en esta fase: se comprobó con peticiones HTTP.

Cambios: Docker no está instalado en el equipo, así que se eliminó `docker-compose.yml` y sus menciones. `backend/.env.example` traía el usuario y la contraseña de ese contenedor; ahora trae el marcador `CHANGE_ME`.

## Fase 4: resultado

No existía `README_corregido.md`, así que los cambios se aplicaron al `README.md` actual. Se comprobó con un script que todas las rutas citadas existen (`frontend/.env` no existe porque lo crea quien instala) y se contrastaron con el código los datos de la sección «Seguridad» (sesión de 7 días, cookie `HttpOnly`, 5 intentos en 60 segundos) y la ventana de 20 mensajes. No se volvieron a ejecutar los comandos de arranque: se comprobaron en la fase 3. El comando `psql` del «Inicio rápido» no se ejecutó en esta fase.

Pendiente para la fase 5: añadir `docs/15_ESTUDIO_PARA_LA_DEFENSA.md` a la tabla «Documentación» del README cuando se cree.
