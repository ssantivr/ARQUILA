# PLAN DE TRABAJO

| Fase | Contenido | Estado |
|---|---|---|
| 1 | Definir el problema, el alcance y los requisitos. | Hecho. Ver `01_PLANTEAMIENTO_PROBLEMA.md` y `02_REQUERIMIENTOS.md`. |
| 2 | Implementar y probar arrays. | Hecho. |
| 3 | Implementar Stack y Queue. | Hecho. |
| 4 | Implementar listas simples y dobles. | Hecho. |
| 5 | Integrar una API y una interfaz. | Hecho, con más alcance que el previsto: la API y la interfaz cubren proyectos, terrenos, planos, elevaciones, materiales, archivos y recomendaciones, además de cuartos, componentes estructurales y un modelo 3D del proyecto. |
| 6 | Realizar pruebas, corregir errores y preparar la presentación. | Pruebas hechas (ver `06_PRUEBAS.md`). La guía para la presentación está en `09_GUIA_DEFENSA.md`. |
| 7 | Revisar documentación, control de versiones y entrega final. | Documentación revisada. La rama principal `main` tiene el mismo contenido que la rama de trabajo `santiago`. La instalación desde cero está comprobada (ver `06_PRUEBAS.md`) y las versiones de entrega están marcadas en Git con etiquetas `v1.0.x`; la más reciente es la que se entrega. Falta presentarla. |

El avance de la aplicación, semana por semana, está en `13_EVOLUCION_POR_SEMANAS.md`, junto con los hitos propuestos para después de la entrega. Los cambios agrupados por semana están en `CHANGELOG.md`, en la raíz del repositorio.

Después de la fase 7 se hicieron dos rondas de mejoras: uso de todas las estructuras en la aplicación, integración continua, contraseñas con Argon2, registro de eventos, diseño para pantallas pequeñas, herramientas de formato, diagramas, guion de la demostración y mediciones.

En la segunda ronda las estructuras también se compilaban y probaban en C++ con CMake. Esa versión se retiró después: la aplicación nunca la llamó y el proyecto quedó solo en Python y TypeScript. Los casos borde que solo se probaban ahí se pasaron a `pytest`.

## Pendiente

- Configurar un servicio de correo para la recuperación de contraseña: escribir las variables `SMTP_*` en `backend/.env` y ejecutar `python -m app.check correo@ejemplo.com`.

El comando está explicado en `12_BACKEND_Y_API.md`. El correo necesita credenciales que no se guardan en el repositorio; mientras no las haya, el enlace de recuperación se escribe en la consola del servidor. Mientras no haya un modelo disponible, el asistente responde con reglas fijas.
