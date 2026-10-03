# BASE DE DATOS

## Motor

PostgreSQL.

## Acceso

SQLAlchemy.

## Organización

```text
database/
├── schema.sql
├── seed.sql
└── migrations/
```

## Principios

- Separar persistencia de lógica de negocio.
- Validar entradas.
- Evitar credenciales dentro del código.
- Utilizar variables de entorno.
- Mantener relaciones y restricciones documentadas.
