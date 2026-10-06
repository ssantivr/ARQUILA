# REQUERIMIENTOS

El proyecto tiene dos partes: un núcleo académico, que es lo que evalúa la asignatura, y una aplicación que usa ese núcleo en un caso real.

## Núcleo académico

| Requerimiento | Dónde se cumple |
|---|---|
| Demostrar el comportamiento de un array. | `BACKEND-ARGUILA-/app/data_structures/arrays.py` (búsqueda lineal y binaria) y `BACKEND-ARGUILA-/app/services/geometry.py` (área de un lote) |
| Demostrar un array dinámico. | `BACKEND-ARGUILA-/app/data_structures/arrays.py` (clase `DynamicArray`) |
| Ejecutar operaciones básicas de Stack. | `BACKEND-ARGUILA-/app/data_structures/stack.py` |
| Ejecutar operaciones básicas de Queue. | `BACKEND-ARGUILA-/app/data_structures/queue.py` |
| Insertar, eliminar y recorrer una lista simple. | `BACKEND-ARGUILA-/app/data_structures/singly_linked_list.py` |
| Insertar, eliminar y recorrer una lista doble. | `BACKEND-ARGUILA-/app/data_structures/doubly_linked_list.py` |
| Exponer un endpoint básico de salud del backend. | `GET /health` |
| Mantener el frontend separado del backend. | Carpetas `FRONTEND-ARQUILA/` y `BACKEND-ARGUILA-/`, que solo se comunican por HTTP |

## Aplicación

| Código | Requerimiento | Dónde se cumple |
|---|---|---|
| RF-01 | Gestionar las entidades principales del proyecto. | API y pantallas de proyectos, terrenos, planos, elevaciones, materiales, archivos y recomendaciones |
| RF-02 | Validar los datos recibidos por la API. | `BACKEND-ARGUILA-/app/schemas.py` |
| RF-03 | Consultar información mediante búsquedas. | Búsqueda de proyectos por nombre y filtros por estado, categoría y orientación |
| RF-04 | Usar estructuras de datos apropiadas. | Lista doble en «Deshacer» y cola en el límite de intentos (ver `05_COMPLEJIDAD.md`) |
| RF-05 | Persistir la información en la base de datos. | PostgreSQL mediante SQLAlchemy (ver `BASE-DE-DATOS-ARQUILA/docs/BASE_DATOS.md`) |
| RF-06 | Exponer endpoints documentados para el frontend. | FastAPI genera la documentación en `/docs` |
| RF-07 | Incluir pruebas automatizadas. | `BACKEND-ARGUILA-/tests/`, `FRONTEND-ARQUILA/src/utils/*.test.ts`, `FRONTEND-ARQUILA/src/state/*.test.ts`, `FRONTEND-ARQUILA/src/three/*.test.ts`, `FRONTEND-ARQUILA/src/services/*.test.ts` y las pruebas de componentes `*.test.tsx` (ver `06_PRUEBAS.md`) |

## Requerimientos no funcionales

- Código académico legible, sin comentarios ni documentación interna.
- Identificadores de código en inglés.
- Documentación externa al código en español.
- Backend en Python y frontend en TypeScript.
- Uso de un patrón estructural en el backend: Adapter, para los servicios externos (ver `BACKEND-ARGUILA-/docs/BACKEND_Y_API.md`).
- Separación clara entre presentación, lógica, persistencia y estructuras de datos.
- La complejidad de las operaciones críticas está documentada en `05_COMPLEJIDAD.md`.
- Las credenciales y secretos no se guardan en el repositorio.
- Uso de control de versiones mediante Git.
