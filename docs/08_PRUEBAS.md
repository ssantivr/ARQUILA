# PRUEBAS

## Estrategia

Se utilizará Pytest para validar:

- Operaciones de estructuras de datos.
- Algoritmos de búsqueda.
- Algoritmos de ordenamiento.
- Recorridos de grafos.
- Operaciones de árboles.
- Servicios y endpoints principales.

## Cada prueba debe documentar

- Entrada.
- Resultado esperado.
- Resultado obtenido.
- Condición de éxito.

## Cobertura

Se recomienda ejecutar:

```bash
pytest --cov=app
```

y revisar qué partes del código aún no están cubiertas.
