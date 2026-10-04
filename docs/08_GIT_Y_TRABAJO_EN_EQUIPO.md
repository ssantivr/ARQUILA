# GIT Y TRABAJO EN EQUIPO

Como proyecto de cuarto semestre, el control de versiones forma parte de la calidad del trabajo.

## Flujo recomendado

1. Crear una rama para una tarea.
2. Realizar cambios pequeños.
3. Probar antes de hacer commit.
4. Usar mensajes de commit claros.
5. Integrar los cambios mediante revisión.

## Convención de commits

Los mensajes siguen [Conventional Commits](https://www.conventionalcommits.org/): una línea en inglés, en imperativo y en minúsculas, con la forma `tipo: descripción`. El tipo dice qué clase de cambio es, de modo que el historial se puede leer y filtrar sin abrir cada commit.

| Tipo | Cuándo se usa |
|---|---|
| `feat` | Funcionalidad nueva o cambio visible en el comportamiento. |
| `fix` | Corrección de un error. |
| `docs` | Solo documentación. |
| `test` | Solo pruebas. |
| `refactor` | Cambio interno que no altera el comportamiento. |
| `style` | Solo formato (espacios, saltos de línea). |
| `ci` | Automatización en GitHub. |
| `chore` | Dependencias, configuración y tareas de mantenimiento. |

Reglas:

- La descripción dice qué hace el commit, no cómo: `fix: correct linked list removal`, no `fix: changed the if`.
- Un commit, un cambio. Si la descripción necesita una «y» que une dos cosas distintas, conviene separarlo en dos.
- Sin punto final y, de preferencia, en menos de 72 caracteres.
- Un cambio que rompe la compatibilidad lleva `!` tras el tipo: `feat!: rename the projects endpoint`.
- El ámbito es opcional y va entre paréntesis: `fix(frontend): keep the form after a failed save`.

Ejemplos del historial de este repositorio:

```text
feat: add a flat roof option saved per project and visible floor slabs
fix: correct linked list removal
docs: record the browser check of the password reset link
test: cover password hashing, the HTTP client, openings and the remaining structure operations
ci: run the C++ structures, backend tests and frontend build on GitHub Actions
```

## Formato y hooks

El formato no se discute en la revisión: lo aplican las herramientas. `python scripts/quality.py` lo comprueba y `python scripts/quality.py --fix` lo corrige (ver «Calidad del código» en el `README.md`).

El archivo `.pre-commit-config.yaml` define hooks que formatean los archivos modificados antes de cada commit: Ruff para Python, Prettier para TypeScript y clang-format para C++. Se activan una vez por clon con `pre-commit install`. Si un hook cambia un archivo, el commit se detiene: basta con añadir el archivo corregido y repetirlo.

GitHub ejecuta la misma comprobación en cada subida, así que un cambio mal formateado no pasa inadvertido aunque no se hayan activado los hooks.

## Pull requests

Al abrir un pull request, GitHub rellena la descripción con la plantilla de `.github/pull_request_template.md`: qué cambia, de qué tipo es, cómo se probó y una lista de comprobación con las reglas del proyecto. El título sigue la misma convención que los commits.

## Versiones de las dependencias

Las dependencias tienen versión exacta en `backend/requirements.txt`, `backend/requirements-dev.txt` y `frontend/package.json`, y `frontend/package-lock.json` fija además las dependencias indirectas. Así una instalación nueva obtiene lo mismo que se probó. Para actualizar una, se cambia su versión, se ejecutan las pruebas y se sube el cambio en un commit `chore`.
