# CHANGELOG

Cambios de ARQUILA agrupados por las semanas de `docs/13_EVOLUCION_POR_SEMANAS.md`. Cada semana es un tema del plan, no un periodo del calendario: el código se escribió entre el 3 y el 4 de octubre de 2026 y los temas avanzaron mezclados. Por eso cada entrada cita los commits que la respaldan, y el orden real está en `git log`.

Los tipos de cambio siguen la convención de commits de `docs/08_GIT_Y_TRABAJO_EN_EQUIPO.md`.

## Sin publicar

Cambios posteriores a la etiqueta `v1.0.11`.

- Estructuras en C++ convertidas en plantillas, compiladas con CMake y probadas con Catch2 y AddressSanitizer (`85d2a28`).
- Contraseñas con Argon2id, registro de eventos en JSON y `DATABASE_URL` leída siempre de `backend/.env` (`d2c2c5d`).
- Diseño corregido en pantallas pequeñas, botón «Reintentar», aviso «Guardando…» y pruebas de componentes con Testing Library (`45f1574`).
- Ruff, ESLint, Prettier y clang-format con un solo comando, hooks de pre-commit, plantilla de pull request y versiones de dependencias fijadas (`50b7002`).
- Diagramas de arquitectura y entidad-relación, guion de la demostración, mediciones de las estructuras y este archivo (`44330e1`).
- Casos borde que solo probaba la versión en C++, pasados a `pytest` (`95d69cc`).
- C++, CMake, Catch2 y clang-format retirados: la aplicación nunca llamó a esa versión de las estructuras y el proyecto queda solo en Python y TypeScript (`0ed2b33`). Las entradas de este archivo que los mencionan se conservan como registro histórico.
- Instalación comprobada desde un clon nuevo (`f2d2cc8`).
- `README.md` reescrito para un proyecto solo en Python y TypeScript (`60caa3c`).
- Modelo 3D dibujado solo cuando cambia, sin parpadeo al redimensionar ni en las losas, con el contexto WebGL liberado al salir; el inspector y el asistente ya no pierden ni duplican cambios (`45479bc`).
- Deshacer devuelve los vértices de un terreno y los cuartos y componentes de un plano; los nombres repetidos a la vez y los fallos del correo ya no responden con un error 500 (`33e84e7`).
- Los planos de planta y la ocupación del lote se recargan al crear, renombrar o eliminar un plano (`3bc600c`).
- La subida de archivos se bloquea mientras otra acción se guarda, y hay pruebas de la recarga de los planos y de que la base de datos queda libre mientras responde el asistente (`e2a3c17`).
- Modelo 3D en tema oscuro con escena neón y resplandor en los acentos, cámara que vuela entre vistas y hasta el elemento elegido (doble clic o «Enfocar») y árboles instanciados (`769927f`).
- Módulo «Recorrido interior»: un loft de doble altura de muestra, con entrepiso, escalera, ciudad al atardecer y líneas de neón, que se recorre sin salir de sus paredes ni atravesar sus muebles (`696d30b`) ni su escalera (`ea59ada`).
- El asistente usa solo el modelo local de Ollama, con las reglas fijas como respaldo: se retira el proveedor de IA en la nube y su dependencia (`be38614`).

## Semana 9 — Calidad, seguridad y rendimiento

- Autenticación con sesión en cookie y permisos por dueño de cada proyecto (`c1a1e88`).
- Límite de intentos de inicio de sesión con la cola (`84db8d1`, `58e28a0`) y de solicitudes de recuperación de contraseña (`d036644`).
- Recuperación de contraseña por enlace de un solo uso (`8c4206b`, `9fed01d`).
- Pruebas del backend también contra PostgreSQL (`05d36cf`) y pruebas unitarias del frontend (`f5eefe9`, `f77448d`).
- Carga de los puntos de terreno en una sola consulta y pausa en los reintentos de Ollama (`4665bcb`).
- Integración continua en GitHub Actions (`dd086ea`).
- Cabeceras de seguridad, enlace de recuperación fuera de la barra de direcciones y límite de la descripción (`56abe42`).

## Semana 8 — Integración completa

- Mensajes de error del backend traducidos al español en la interfaz (`449eb9e`).
- Edición de proyectos, terrenos, materiales, planos y elevaciones desde la interfaz (`b86b914`, `892edd9`).
- Deshacer eliminaciones con la lista doblemente enlazada (`195c072`, `bc0bad2`); rehacer con la pila, contexto del asistente con la lista simple y orden de materiales con el array dinámico (`08db424`).
- Arranque del backend con un solo comando que lee `backend/.env` (`ca5afea`).
- Tres proyectos de ejemplo con usuario de demostración (`6da400c`) y cuatro ejemplos que abren directamente en el modelo 3D (`94521f6`).

## Semana 7 — Base de datos y backend

- Esquema inicial y datos de ejemplo (`a1860ec`).
- API de usuarios, proyectos, terrenos y materiales con FastAPI (`d96b8b0`), y de planos y elevaciones (`e259553`).
- Estructuras de datos en Python dentro del backend (`b361018`).
- Subida de archivos y adjuntos en planos y elevaciones (`bc5c9f6`).
- Migraciones SQL numeradas con su ejecutor (`707f9a5`).
- Resumen con los totales de cada usuario (`ecc3c1b`).

## Semana 6 — IA integrada al proyecto

- Recomendaciones automáticas por reglas (`4f28238`, `e19f3ba`), con prioridad (`869d048`).
- Asistente con conversaciones guardadas por proyecto (`c8c02d1`, `e4ff7fa`).
- Respuesta por reglas cuando la IA no está disponible (`62d3e35`, `6d67491`).
- Modelo local con Ollama (`f3f8575`).
- Costos de materiales ya calculados para que la IA no los invente (`cbdde09`) e indicador de quién responde (`a541bc6`).

## Semana 5 — Planos, vistas y arquitectura

- Paneles de planos y elevaciones (`fb524d2`, `0d3006d`) y visor de archivos adjuntos dentro de la aplicación (`c88231e`).
- Plano de implantación con retiros y área edificable (`3e662e1`, `6f6c1cd`), imprimible o guardable como PDF (`962639e`).
- Cuartos y modelo 3D interactivo del proyecto (`4153b4d`, `50b4553`), con componentes estructurales (`bccfd6f`).
- Modelo dibujado como edificio, con vistas de cámara y capas (`7f9cdc7`); plantas y elevaciones generadas a partir de los cuartos (`869d048`, `245fc0b`).
- Material de superficie por elemento, guardado en el backend (`bfe10f8`, `a75aec5`); muros con puertas y ventanas, iluminación y relieve (`f4301fc`, `090d648`); techo plano opcional (`0445839`).

## Semana 4 — Terrenos y visualización técnica

- Ancho y largo de cada terreno (`1966d76`) y dibujo de la vista superior y el perfil de pendiente (`8e494d2`).
- Lotes con forma libre, vista 3D giratoria y capas (`e240da1`).
- Vistas frontal y lateral (`24739ef`) y mapa de curvas de nivel (`0ea773f`).

## Semana 3 — Arquitectura de la interfaz

- Frontend en React con cliente de la API tipado (`aa6ca70`).
- Página de proyecto dividida en pestañas (`a8135d5`).
- Menú lateral y página de Inicio con métricas (`9829683`); módulos de Terrenos, Materiales, Visualización 3D, Asistente y Configuración (`4611180`).
- Tablas como fichas apiladas en pantallas estrechas (`c4ca7c0`).

## Semana 2 — Diseño visual y referencia

- Paleta adoptada, registrada en la documentación (`9c15016`).
- Tema clásico con modo claro y modo oscuro en todo el frontend (`93c7e22`).

## Semana 1 — Idea, problema y dirección del proyecto

- Documentación inicial del proyecto y notas de planificación (`5a90176`).
- Implementaciones académicas de las estructuras de datos en C++ (`817ec9c`).

## Semana 10 — Versión profesional

Sin cambios todavía: el despliegue fuera del equipo local no se ha hecho.

## Etiquetas existentes

Las versiones de entrega están marcadas con etiquetas anotadas `v1.0.0` a `v1.0.11`. La más reciente, `v1.0.11`, apunta a `7400d3f`.

## Etiquetas propuestas

Para marcar en el historial el punto en que cada semana alcanzó su estado actual. Son una propuesta: no están creadas. Cada comando crea una etiqueta anotada sobre un commit que ya existe; no cambia el código ni las ramas.

```bash
git tag -a v0.1 5a90176 -m "Semana 1: idea, problema y documentación inicial"
git tag -a v0.2 93c7e22 -m "Semana 2: sistema visual con tema claro y oscuro"
git tag -a v0.3 4611180 -m "Semana 3: arquitectura de la interfaz con los siete módulos"
git tag -a v0.4 0ea773f -m "Semana 4: terrenos y visualización técnica"
git tag -a v0.5 245fc0b -m "Semana 5: planos, elevaciones y modelo 3D"
git tag -a v0.6 a541bc6 -m "Semana 6: asistente de IA con respaldo por reglas"
git tag -a v0.7 707f9a5 -m "Semana 7: backend, PostgreSQL y migraciones"
git tag -a v0.8 94521f6 -m "Semana 8: integración completa con datos reales"
git tag -a v0.9 50b7002 -m "Semana 9: calidad, seguridad y rendimiento"
git push origin v0.1 v0.2 v0.3 v0.4 v0.5 v0.6 v0.7 v0.8 v0.9
```

Antes de ejecutarlos conviene saber dos cosas:

- **No quedan en orden cronológico.** Como las semanas son temas y avanzaron mezcladas, `v0.2` apunta a un commit del 4 de octubre y `v0.7` a uno del 3. Quien lea `v0.1 … v0.9` como una secuencia en el tiempo se confundirá; lo que marcan es dónde quedó cada tema.
- **Conviven con `v1.0.x`.** Las etiquetas de entrega ya existen y son anteriores a algunas de estas. Si esa mezcla estorba, una alternativa es no crear las `v0.x` y dejar este archivo como única referencia de las semanas.

`v0.9` apunta a `50b7002`, que el 4 de octubre de 2026 todavía no se había subido a GitHub: hay que subir la rama antes de subir esa etiqueta.
