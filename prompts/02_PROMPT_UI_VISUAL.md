# PROMPT UI VISUAL

Interfaz en TypeScript de aspecto profesional y clásico, con modo claro y modo oscuro, paneles compactos y un solo color de acento.

## Paleta adoptada

Desde el 4 de octubre de 2026 se usa un tema clásico con modo claro y modo oscuro: fondos neutros, paneles lisos con borde fino, un solo color de acento azul marino (`#1F4E79` en claro, `#8AB4DE` en oscuro) y títulos en tipografía con serifa, con verde, ámbar y rojo para éxito, advertencia y error. No hay degradados, brillos ni vidrio. Los colores están en las variables de `frontend/src/styles.css`: `:root` define el modo claro y `:root[data-theme="dark"]` el oscuro. El modo inicial sigue la preferencia del sistema; el botón de la cabecera lo cambia y `frontend/src/state/theme.ts` lo guarda en `localStorage`.

Los dibujos generados (plantas, fachadas, corte e implantación) son siempre una lámina clara con tinta oscura, en los dos modos, para que el archivo descargado y la impresión se vean igual que en pantalla.

Historia de la decisión: este documento proponía al principio una paleta oscura con neón (`#090A0F`, cian `#00F0FF`, magenta `#FF007F` y vidrio translúcido); el 3 de octubre de 2026 se descartó a favor de la de la Semana 2 de `06_EVOLUCION_POR_SEMANAS.md` (fondo `#07111A`, azul `#1597E5`, cian `#20C7F5`), que era la que estaba aplicada, y el neón quedó solo en el visor 3D. El 4 de octubre de 2026 se aplicó a toda la interfaz y, ese mismo día, se reemplazó por el diseño clásico actual a petición del autor.

Toda implementación visual debe mantenerse modular, accesible y separada de la lógica de negocio.
