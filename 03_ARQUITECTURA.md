# ARQUITECTURA DEL SISTEMA

## Capas

### Presentación
Frontend desarrollado en TypeScript.

### API
FastAPI recibe, valida y responde solicitudes HTTP.

### Servicios
Contienen la lógica de negocio.

### Repositorios
Aíslan el acceso y persistencia de datos.

### Estructuras y algoritmos
Implementan las operaciones que requieren análisis de estructuras de datos.

### Persistencia
PostgreSQL mediante SQLAlchemy.

## Flujo

```text
Frontend
   ↓
HTTP Request
   ↓
Controller / API
   ↓
Schema Validation
   ↓
Service
   ↓
Data Structure / Algorithm
   ↓
Repository
   ↓
Database
```
