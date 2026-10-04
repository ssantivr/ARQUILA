# PLAN DE CORRECCIÓN FINAL

Objetivo: el proyecto queda solo con backend en Python (FastAPI) y frontend en TypeScript (React). Se elimina C++ por completo.

- [x] Fase 1 - Verificar que Python cubre todo antes de borrar nada
- [x] Fase 2 - Eliminar C++
- [ ] Fase 3 - Configuración y arranque
- [ ] Fase 4 - README
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
