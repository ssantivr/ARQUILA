# QUÉ SE PUEDE MEJORAR

Lista ordenada por urgencia. Cada punto dice qué falta, por qué importa y por dónde empezar. Lo que no se hizo está descrito en «Limitaciones y trabajo futuro» de `10_CONCLUSIONES.md`; aquí está lo que conviene hacer con eso.

Estado al 6 de octubre de 2026, después de separar el proyecto en tres repositorios: `pytest` (288 pruebas), `npm test` (124 pruebas), `npm run build` y `python scripts/quality.py` pasan, y la integración continua pasa en `main` de `BACKEND-ARQUILA` y de `FRONTEND-ARQUILA`.

## 1. Antes de la entrega

Son cambios pequeños que se notan a primera vista.

| Qué | Dónde | Por qué |
|---|---|---|
| Completar `[INTEGRANTES]` | `15_ESTUDIO_PARA_LA_DEFENSA.md` | La lista de archivos por integrante no sirve sin nombres. |
| Declarar el uso de herramientas de IA | Donde lo pida la política del curso | El `README.md` ya no tiene una sección para ello. El uso de IA es visible en `prompts/`, en `.agents/` y en los commits; hay que declararlo como pida el docente. |
| Abrir `README.md`, `04_ESTRUCTURAS_DATOS.md`, `03_ARQUITECTURA.md` y `BASE-DE-DATOS-ARQUILA/docs/BASE_DATOS.md` en GitHub | Diagramas Mermaid | Su sintaxis se revisó a mano, pero no se comprobó que GitHub los dibuje. |
| Probar a mano el guion de `14_GUION_DEMO.md` | Aplicación en marcha | El 6 de octubre de 2026 se recorrieron todas las pantallas en Chrome sin ventana, sin errores, pero solo leyendo: falta crear, editar, borrar y deshacer con el ratón, como en la demostración. |

## 2. Antes de la defensa

Preguntas que el historial y el código invitan a hacer.

- **El historial de Git es de dos días.** Los 117 commits son del 3 y el 4 de octubre de 2026 y tienen un solo autor. `13_EVOLUCION_POR_SEMANAS.md` y `CHANGELOG.md` hablan de semanas. Hay que poder explicar esa diferencia sin rodeos. Si el trabajo es en grupo, conviene que cada integrante tenga commits propios de aquí en adelante.
- **El flujo de pull requests no se usó al final.** `08_GIT_Y_TRABAJO_EN_EQUIPO.md` describe pull requests, pero los últimos cambios se subieron directamente a `main`. O se usa el flujo descrito, o el documento debe decir cómo se trabajó de verdad.
- **Poder explicar el código sin leerlo.** Gran parte se escribió con ayuda de IA. `15_ESTUDIO_PARA_LA_DEFENSA.md` lista los archivos que cada integrante debe dominar: practicar escribiendo a mano `remove` de la lista simple y `enqueue` de la cola circular.
- **Dos funciones no se usan en la aplicación.** `linear_search` y `binary_search` solo aparecen en las pruebas y en las mediciones. Ver el punto 3.

## 3. Estructuras de datos

Es la parte que evalúa la asignatura, así que es donde más rinde el esfuerzo.

- **Usar las búsquedas en una función real.** Por ejemplo, buscar un material por nombre con búsqueda lineal y por costo con búsqueda binaria sobre el array ya ordenado. Así las seis estructuras y sus dos algoritmos quedan usados por la aplicación.
- **Pila y cola de capacidad fija.** Hoy fallan cuando se llenan. Se puede hacer que crezcan como `DynamicArray`, o dejar la capacidad fija y explicar en la defensa por qué se eligió.
- **Orden de materiales en O(n²).** El orden por inserción sirve para pocos materiales. Escribir a mano un `merge sort` y comparar los dos tiempos en `05_COMPLEJIDAD.md` muestra la diferencia con números.
- **Mediciones más grandes.** Llegan a 100 000 elementos y no miden memoria. La respuesta sobre 1 millón de elementos de `09_GUIA_DEFENSA.md` es un razonamiento, no una medición: medirlo la vuelve un dato.
- **Deshacer incompleto.** No cubre cuartos ni componentes estructurales, y el historial se pierde al reiniciar el servidor salvo que se active `STATE_STORAGE=database`, que lo guarda en la base.

## 4. Pruebas

- **Frontend.** Solo 4 componentes tienen pruebas (`AsyncStatus`, `Sidebar`, `LoginPage`, `ProjectsPage`), y de las 9 páginas solo 2. Las siguientes en importancia son `MaterialsPage` y `TerrainsPage`, porque son las que activan las estructuras.
- **Cobertura.** El 98 % que cita `06_PRUEBAS.md` se midió antes de la comprobación final y no se repitió. `coverage` no está en `requirements-dev.txt`: añadirlo y medirlo en la integración continua evita que el dato quede viejo.
- **Navegador.** No hay ninguna prueba automática que abra la aplicación completa. El enlace de recuperación de contraseña se comprobó a mano una vez.
- **Servicios reales.** No se probó el envío real de correo.

## 5. Aplicación

- **Estado en memoria.** El historial de Deshacer y el límite de intentos de inicio de sesión viven en la memoria del servidor. Con dos procesos o tras un reinicio dejan de ser correctos. Guardarlos en la base de datos lo resuelve.
- **Tamaño del frontend.** `npm run build` avisa de que el archivo del visor 3D pesa 639 kB (165 kB comprimido). Ya se carga aparte del resto; se puede dividir más con `manualChunks` en `vite.config.ts`.
- **Modelo 3D.** No representa la pendiente del terreno ni comprueba que un cuarto quede dentro del lote.
- **Despliegue.** La aplicación solo se ejecutó en un equipo local, sin HTTPS. No hay una forma de levantar todo con un solo comando.

## 6. Documentación

- **`06_PRUEBAS.md` empieza con listas genéricas** («Insertar varios valores», «Consultar el frente») que repiten lo que dice más abajo con detalle. Se pueden quitar.
- **`13_EVOLUCION_POR_SEMANAS.md` es largo** y mezcla lo hecho con lo propuesto. Separar «hecho» y «propuesto» en dos partes claras lo hace más fácil de defender.
- **Capturas de pantalla.** El `README.md` no muestra la aplicación. Dos o tres imágenes ayudan a quien no la va a instalar.

## 7. Aspecto y acabado de la aplicación

Revisado el 4 de octubre de 2026 con el usuario de demostración: las siete pantallas a 1366 px, la de proyectos en modo oscuro y tres pantallas a 390 px. El diseño es coherente (mismos paneles, misma tipografía, modo oscuro completo) y a 390 px no hay desplazamiento horizontal. No se revisaron las pestañas internas de un proyecto ni los formularios de edición. Lo que se vio:

### Datos de la demostración

Los datos son de la base local, no del repositorio.

- **Ya corregido.** Había un proyecto antiguo, «Demo House», con datos en inglés, que hacía aparecer «Estructura» y «structure» como categorías distintas, y dos copias de «Casa Familiar Andina». Se borraron los tres. `BASE-DE-DATOS-ARQUILA/seed.sql` no crea ninguno de ellos.
- **Ejemplos sin ubicación ni materiales.** «Vivienda compacta» ya trae ubicación y ocho materiales. «Edificio multifamiliar» y «Oficina profesional» siguen con «Sin ubicación» y sin materiales; «Casa Familiar Andina» tiene ubicación pero no materiales.
- **Modelo 3D de «Cabaña Mindo».** «Casa Los Arrayanes» y «Edificio Mirador» ya traen cuartos y columnas en `BASE-DE-DATOS-ARQUILA/seed.sql`. «Cabaña Mindo» sigue sin cuartos ni medidas del lote, así que no tiene modelo.

### Detalles visibles

- **Categorías como texto libre.** La mezcla de «Estructura» y «structure» fue posible porque la categoría de un material se escribe a mano. Una lista cerrada (Estructura, Mampostería, Acabados, Carpintería, Cubierta, Instalaciones) evita duplicados y errores de escritura.
- **Gráfico de costo por categoría.** La barra de «Instalaciones» con 0,00 US$ no se dibuja y las barras pequeñas casi no se distinguen. Mostrar el porcentaje al lado de cada valor ayuda.
- **Tabla de materiales en el móvil.** Cada material ocupa siete filas y la pantalla mide casi 4800 px de alto con 17 materiales. Una tarjeta compacta (nombre, proyecto y subtotal, con el resto al abrirla) la acorta.
- **Barra de módulos en el móvil.** Se desplaza de lado y los últimos módulos quedan ocultos sin ninguna señal. Un degradado en el borde o solo iconos con texto debajo lo hacen evidente.
- **Listas largas.** Proyectos, terrenos y materiales se muestran completos. Con muchos registros hace falta paginar o cargar por partes.

Ya corregido tras esta revisión: el visor 3D tiene cielo, suelo, niebla en el horizonte y árboles redondeados en lugar de cubos; el separador de miles ahora es igual en todos los números (`FRONTEND-ARQUILA/src/utils/format.ts`), la pestaña del navegador tiene icono (`FRONTEND-ARQUILA/public/favicon.svg`) y el primer panel de «Asistente IA» se llama «Proyecto».

### Para que se sienta terminada

- **Página de bienvenida vacía.** Un usuario nuevo ve el resumen en cero. Un mensaje con un botón «Crear desde un ejemplo» guía el primer paso.
- **Capturas en el `README.md`.** Inicio, modelo 3D y materiales, en claro y oscuro.
- **Confirmar con pruebas lo que hoy se ve bien.** Contraste, foco visible y uso solo con teclado se revisaron a mano en su momento; no hay nada que avise si un cambio los rompe.

## Orden sugerido

1. Todo el punto 1, y limpiar los datos de la demostración (punto 7).
2. Usar las búsquedas en la aplicación (punto 3) y actualizar `04_ESTRUCTURAS_DATOS.md`.
3. Pruebas de `MaterialsPage` y `TerrainsPage` (punto 4).
4. Preparar las respuestas del punto 2.
5. El resto, si queda tiempo.
