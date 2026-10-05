# CHANGELOG

Cambios de ARQUILA agrupados por las semanas de `docs/13_EVOLUCION_POR_SEMANAS.md`. Cada semana es un tema del plan, no un periodo del calendario: el código se escribió entre el 3 y el 4 de octubre de 2026 y los temas avanzaron mezclados. Por eso cada entrada cita los commits que la respaldan, y el orden real está en `git log`.

Los tipos de cambio siguen la convención de commits de `docs/08_GIT_Y_TRABAJO_EN_EQUIPO.md`.

## Sin publicar

Cambios posteriores a la etiqueta `v1.0.11`.

- Estructuras en C++ convertidas en plantillas, compiladas con CMake y probadas con Catch2 y AddressSanitizer (`fbe9cad`).
- Contraseñas con Argon2id, registro de eventos en JSON y `DATABASE_URL` leída siempre de `backend/.env` (`e51aeef`).
- Diseño corregido en pantallas pequeñas, botón «Reintentar», aviso «Guardando…» y pruebas de componentes con Testing Library (`ffee820`).
- Ruff, ESLint, Prettier y clang-format con un solo comando, hooks de pre-commit, plantilla de pull request y versiones de dependencias fijadas (`fb248e9`).
- Diagramas de arquitectura y entidad-relación, guion de la demostración, mediciones de las estructuras y este archivo (`81d1294`).
- Casos borde que solo probaba la versión en C++, pasados a `pytest` (`c1db608`).
- C++, CMake, Catch2 y clang-format retirados: la aplicación nunca llamó a esa versión de las estructuras y el proyecto queda solo en Python y TypeScript (`84a5943`). Las entradas de este archivo que los mencionan se conservan como registro histórico.
- Instalación comprobada desde un clon nuevo (`43b87b4`).
- `README.md` reescrito para un proyecto solo en Python y TypeScript (`e9c9106`).
- Modelo 3D dibujado solo cuando cambia, sin parpadeo al redimensionar ni en las losas, con el contexto WebGL liberado al salir; el inspector y el asistente ya no pierden ni duplican cambios (`8b95fcc`).
- Deshacer devuelve los vértices de un terreno y los cuartos y componentes de un plano; los nombres repetidos a la vez y los fallos del correo ya no responden con un error 500 (`5f4c9d7`).
- Los planos de planta y la ocupación del lote se recargan al crear, renombrar o eliminar un plano (`625956c`).
- La subida de archivos se bloquea mientras otra acción se guarda, y hay pruebas de la recarga de los planos y de que la base de datos queda libre mientras responde el asistente (`effad61`).
- Modelo 3D en tema oscuro con escena neón y resplandor en los acentos, cámara que vuela entre vistas y hasta el elemento elegido (doble clic o «Enfocar») y árboles instanciados (`e27206f`).
- Módulo «Recorrido interior»: un loft de doble altura de muestra, con entrepiso, escalera, ciudad al atardecer y líneas de neón, que se recorre sin salir de sus paredes ni atravesar sus muebles (`923558f`).

## Semana 9 — Calidad, seguridad y rendimiento

- Autenticación con sesión en cookie y permisos por dueño de cada proyecto (`a436a31`).
- Límite de intentos de inicio de sesión con la cola (`2a431b7`, `02ee1d6`) y de solicitudes de recuperación de contraseña (`00dbc0e`).
- Recuperación de contraseña por enlace de un solo uso (`c9ff797`, `90d49bb`).
- Pruebas del backend también contra PostgreSQL (`3ce53d6`) y pruebas unitarias del frontend (`860ef9c`, `03d5917`).
- Carga de los puntos de terreno en una sola consulta y pausa en los reintentos de Ollama (`63686ea`).
- Integración continua en GitHub Actions (`508110e`).
- Cabeceras de seguridad, enlace de recuperación fuera de la barra de direcciones y límite de la descripción (`b884a93`).

## Semana 8 — Integración completa

- Mensajes de error del backend traducidos al español en la interfaz (`e0f7330`).
- Edición de proyectos, terrenos, materiales, planos y elevaciones desde la interfaz (`6792703`, `6826b5a`).
- Deshacer eliminaciones con la lista doblemente enlazada (`68ccf81`, `b45e464`); rehacer con la pila, contexto del asistente con la lista simple y orden de materiales con el array dinámico (`a574ffa`).
- Arranque del backend con un solo comando que lee `backend/.env` (`78c4605`).
- Tres proyectos de ejemplo con usuario de demostración (`97f4c1a`) y cuatro ejemplos que abren directamente en el modelo 3D (`f6174e7`).

## Semana 7 — Base de datos y backend

- Esquema inicial y datos de ejemplo (`6132dc3`).
- API de usuarios, proyectos, terrenos y materiales con FastAPI (`a0c0e01`), y de planos y elevaciones (`69980de`).
- Estructuras de datos en Python dentro del backend (`3649bec`).
- Subida de archivos y adjuntos en planos y elevaciones (`0aa435a`).
- Migraciones SQL numeradas con su ejecutor (`af5fd3f`).
- Resumen con los totales de cada usuario (`fbd6a84`).

## Semana 6 — IA integrada al proyecto

- Recomendaciones automáticas por reglas (`154ec13`, `1855371`), con prioridad (`f48e331`).
- Asistente con conversaciones guardadas por proyecto (`c9f1da5`, `37e583c`).
- Respuesta por reglas cuando la IA no está disponible (`e3ff0cd`, `a6543fb`).
- Modelo local con Ollama cuando no hay clave de Anthropic (`a3bf771`).
- Costos de materiales ya calculados para que la IA no los invente (`1c6721e`) e indicador de quién responde (`aa96ab9`).

## Semana 5 — Planos, vistas y arquitectura

- Paneles de planos y elevaciones (`d282b00`, `cab774d`) y visor de archivos adjuntos dentro de la aplicación (`0eda49a`).
- Plano de implantación con retiros y área edificable (`ddcbe2a`, `3b85135`), imprimible o guardable como PDF (`f8d03d9`).
- Cuartos y modelo 3D interactivo del proyecto (`2325d25`, `80ce1f8`), con componentes estructurales (`66bd529`).
- Modelo dibujado como edificio, con vistas de cámara y capas (`41ed4e5`); plantas y elevaciones generadas a partir de los cuartos (`f48e331`, `31f68d5`).
- Material de superficie por elemento, guardado en el backend (`78a3f60`, `de58228`); muros con puertas y ventanas, iluminación y relieve (`c122a30`, `8e35bda`); techo plano opcional (`eae4a84`).

## Semana 4 — Terrenos y visualización técnica

- Ancho y largo de cada terreno (`052e905`) y dibujo de la vista superior y el perfil de pendiente (`d97b7d8`).
- Lotes con forma libre, vista 3D giratoria y capas (`d968702`).
- Vistas frontal y lateral (`5c8d0d5`) y mapa de curvas de nivel (`a0b5601`).

## Semana 3 — Arquitectura de la interfaz

- Frontend en React con cliente de la API tipado (`d3ba383`).
- Página de proyecto dividida en pestañas (`4fbb791`).
- Menú lateral y página de Inicio con métricas (`4092b77`); módulos de Terrenos, Materiales, Visualización 3D, Asistente y Configuración (`892f2df`).
- Tablas como fichas apiladas en pantallas estrechas (`232a027`).

## Semana 2 — Diseño visual y referencia

- Paleta adoptada, registrada en la documentación (`bcb183a`).
- Tema clásico con modo claro y modo oscuro en todo el frontend (`9a2e23a`).

## Semana 1 — Idea, problema y dirección del proyecto

- Documentación inicial del proyecto y notas de planificación (`690d68a`).
- Implementaciones académicas de las estructuras de datos en C++ (`a4e148c`).

## Semana 10 — Versión profesional

Sin cambios todavía: el despliegue fuera del equipo local no se ha hecho.

## Etiquetas existentes

Las versiones de entrega están marcadas con etiquetas anotadas `v1.0.0` a `v1.0.11`. La más reciente, `v1.0.11`, apunta a `1e76003`.

## Etiquetas propuestas

Para marcar en el historial el punto en que cada semana alcanzó su estado actual. Son una propuesta: no están creadas. Cada comando crea una etiqueta anotada sobre un commit que ya existe; no cambia el código ni las ramas.

```bash
git tag -a v0.1 690d68a -m "Semana 1: idea, problema y documentación inicial"
git tag -a v0.2 9a2e23a -m "Semana 2: sistema visual con tema claro y oscuro"
git tag -a v0.3 892f2df -m "Semana 3: arquitectura de la interfaz con los siete módulos"
git tag -a v0.4 a0b5601 -m "Semana 4: terrenos y visualización técnica"
git tag -a v0.5 31f68d5 -m "Semana 5: planos, elevaciones y modelo 3D"
git tag -a v0.6 aa96ab9 -m "Semana 6: asistente de IA con respaldo por reglas"
git tag -a v0.7 af5fd3f -m "Semana 7: backend, PostgreSQL y migraciones"
git tag -a v0.8 f6174e7 -m "Semana 8: integración completa con datos reales"
git tag -a v0.9 fb248e9 -m "Semana 9: calidad, seguridad y rendimiento"
git push origin v0.1 v0.2 v0.3 v0.4 v0.5 v0.6 v0.7 v0.8 v0.9
```

Antes de ejecutarlos conviene saber dos cosas:

- **No quedan en orden cronológico.** Como las semanas son temas y avanzaron mezcladas, `v0.2` apunta a un commit del 4 de octubre y `v0.7` a uno del 3. Quien lea `v0.1 … v0.9` como una secuencia en el tiempo se confundirá; lo que marcan es dónde quedó cada tema.
- **Conviven con `v1.0.x`.** Las etiquetas de entrega ya existen y son anteriores a algunas de estas. Si esa mezcla estorba, una alternativa es no crear las `v0.x` y dejar este archivo como única referencia de las semanas.

`v0.9` apunta a `fb248e9`, que el 4 de octubre de 2026 todavía no se había subido a GitHub: hay que subir la rama antes de subir esa etiqueta.
