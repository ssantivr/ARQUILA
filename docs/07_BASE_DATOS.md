# BASE DE DATOS

## Motor

PostgreSQL.

## Acceso

SQLAlchemy.

## Organización

```text
database/
├── migrations/
│   ├── 001_initial_schema.sql
│   ├── 002_password_reset_tokens.sql
│   └── 003_terrain_points.sql
└── seed.sql
```

## Migraciones

El esquema de la base de datos se define con archivos SQL numerados en `database/migrations/`. Cada archivo es un cambio y se aplica una sola vez, en orden.

Para crear las tablas en una base nueva, o actualizar una existente:

```bash
cd backend
python -m app.migrate
```

Con `python -m app.migrate --seed` se cargan además los datos de ejemplo de `seed.sql`: un usuario de demostración con tres proyectos, sus terrenos, planos, elevaciones, materiales y notas. Se puede ejecutar varias veces sin duplicar nada.

El usuario de demostración es `demo@example.com` con contraseña `arquila-demo`. Es una credencial pública, pensada solo para desarrollo: no se debe cargar `seed.sql` en una base con datos reales ni en un servidor accesible desde internet.

El script (`backend/app/migrate.py`) anota cada archivo aplicado en la tabla `schema_migrations`, y en cada ejecución aplica solo los que faltan. Cada migración corre dentro de una transacción: si falla, no queda anotada y el proceso se detiene.

Para cambiar el esquema:

1. Crear un archivo nuevo con el siguiente número, por ejemplo `002_add_project_budget.sql`.
2. Hacer el mismo cambio en `backend/app/models.py`.
3. Ejecutar `python -m app.migrate`.

Un archivo ya aplicado no se debe modificar: el script no lo volvería a ejecutar. Los cambios siempre van en un archivo nuevo.

No hay migraciones de reversa. Para deshacer un cambio se escribe una migración nueva que lo revierta.

## Principios

- Separar persistencia de lógica de negocio.
- Validar entradas.
- Evitar credenciales dentro del código.
- Utilizar variables de entorno.
- Mantener relaciones y restricciones documentadas.
