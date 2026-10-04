# PLAN DE TRABAJO

| Fase | Contenido | Estado |
|---|---|---|
| 1 | Definir el problema, el alcance y los requisitos. | Hecho. Ver `01_PLANTEAMIENTO_PROBLEMA.md` y `02_REQUERIMIENTOS.md`. |
| 2 | Implementar y probar arrays. | Hecho. |
| 3 | Implementar Stack y Queue. | Hecho. |
| 4 | Implementar listas simples y dobles. | Hecho. |
| 5 | Integrar una API y una interfaz. | Hecho, con más alcance que el previsto: la API y la interfaz cubren proyectos, terrenos, planos, elevaciones, materiales, archivos y recomendaciones. |
| 6 | Realizar pruebas, corregir errores y preparar la presentación. | Pruebas hechas (ver `06_PRUEBAS.md`). La guía para la presentación está en `08_GUIA_DEFENSA.md`. |
| 7 | Revisar documentación, control de versiones y entrega final. | Documentación revisada. La rama principal `main` tiene el mismo contenido que la rama de trabajo `santiago`. La instalación desde cero está comprobada (ver `06_PRUEBAS.md`) y la versión de entrega está marcada en Git con la etiqueta `v1.0.0`. Falta presentarla. |

El avance de la aplicación, semana por semana, está en `06_EVOLUCION_POR_SEMANAS.md`, en la raíz del repositorio.

## Pendiente

- Probar el asistente con Claude: escribir `ANTHROPIC_API_KEY` en `backend/.env` (de pago) y ejecutar `python -m app.check`. Es opcional: el asistente ya está probado con el modelo local `llama3.2` de Ollama (ver `10_BACKEND_Y_API.md`).
- Configurar un servicio de correo para la recuperación de contraseña: escribir las variables `SMTP_*` en `backend/.env` y ejecutar `python -m app.check correo@ejemplo.com`.

El comando está explicado en `10_BACKEND_Y_API.md`. El correo necesita credenciales que no se guardan en el repositorio; mientras no las haya, el enlace de recuperación se escribe en la consola del servidor. Mientras no haya un modelo disponible, el asistente responde con reglas fijas.
