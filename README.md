# ARQUILA PROJECT

Proyecto académico de cuarto semestre de Ingeniería de Software.

## Objetivo

Aplicar los conceptos de estructuras de datos vistos en clase dentro de un proyecto de software organizado.

## Alcance académico

- Arrays unidimensionales.
- Arrays dinámicos.
- Stack con LIFO.
- Queue con FIFO.
- Lista simplemente enlazada.
- Lista doblemente enlazada.

## Tecnologías

- C++ para las implementaciones académicas de estructuras de datos.
- Python y FastAPI para el backend.
- TypeScript para el frontend.
- Git para control de versiones.

## Regla de código

Los identificadores del código están en inglés y el código no contiene comentarios ni documentación interna.

La explicación del proyecto está separada del código y está escrita en español.

## Organización

```text
ARQUILA/
├── data_structures/
├── backend/
├── frontend/
├── docs/
└── .agentes/
```

## Principio de diseño

El proyecto mantiene una complejidad adecuada para cuarto semestre. Se prioriza comprender y defender correctamente las estructuras estudiadas antes de incorporar estructuras más avanzadas.

## Ejecución de una estructura

```bash
g++ data_structures/stack/Stack.cpp -o stack
./stack
```

## Ejecución del backend

```bash
cd backend
pip install -r requirements.txt
uvicorn app.main:app --reload
```

## Pruebas

```bash
cd backend
pytest
```
