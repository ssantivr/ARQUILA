# ARQUITECTURA

El proyecto separa las responsabilidades en partes que se pueden estudiar, probar y cambiar por separado.

```text
Frontend (TypeScript, React)
   |  HTTP
   v
API (FastAPI)                valida los datos recibidos
   |
   v
Servicios                    lógica de negocio y permisos
   |            \
   v             v
Repositorios    Estructuras de datos (Python)
   |
   v
PostgreSQL
```

Las implementaciones académicas en C++ (`data_structures/`) son un módulo aparte: no las llama la aplicación.

## Frontend

Está en `frontend/`, escrito en TypeScript con React. Muestra la información y envía peticiones; no guarda datos ni decide permisos.

```text
src/pages/        Pantallas: inicio de sesión, inicio, proyectos y detalle de proyecto
src/components/   Piezas reutilizables: paneles, formularios, esquemas del terreno
src/services/     Cliente HTTP tipado, un método por endpoint
src/types/        Tipos que reflejan los del backend
src/hooks/        Lógica compartida de carga de datos
src/state/        Estado compartido entre el modelo 3D, materiales, recomendaciones y asistente
src/utils/        Cálculo, formato y traducción de mensajes
```

## Backend

Está en `backend/`, escrito en Python con FastAPI.

```text
app/api/              Recibe la petición HTTP
app/schemas.py        Forma y validación de los datos
app/services/         Lógica de negocio y control de permisos
app/repositories/     Consultas a la base de datos
app/models.py         Tablas
app/data_structures/  Pila, cola, listas y array dinámico
```

Las decisiones de cada parte están explicadas en `12_BACKEND_Y_API.md`.

## Estructuras de datos

Las estructuras estudiadas existen en dos versiones:

- En C++, dentro de `data_structures/`, como material de estudio. Cada estructura es una plantilla en un `.hpp`, con un programa de ejemplo en el `.cpp` del mismo nombre.
- En Python, dentro de `backend/app/data_structures/`, que son las que usa la aplicación.

Esta separación permite estudiar la materia sin mezclar la implementación académica con la interfaz.

## Base de datos

PostgreSQL. El esquema se define con migraciones numeradas en `database/migrations/` (ver `07_BASE_DATOS.md`).
