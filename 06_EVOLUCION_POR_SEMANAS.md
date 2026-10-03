# ARQUILA — Evolución del proyecto por semanas

## Propósito

Este documento resume cómo fue evolucionando la idea del proyecto y cómo se propone organizar su implementación. Debe servir como contexto para Claude Code y también como memoria técnica del proyecto.

> Importante: las primeras semanas describen decisiones y trabajo que sí fueron definidos durante el desarrollo de la idea. Las semanas futuras son una propuesta de implementación y evolución, no tareas ya ejecutadas.

---

# SEMANA 1 — Idea, problema y dirección del proyecto

## Qué se planteó

La idea inicial fue construir una aplicación enfocada en arquitectura, terrenos y proyectos de construcción que no se sintiera como una página web convencional.

Se buscó que el producto tuviera una identidad propia y que pudiera reunir en un mismo lugar:

- proyectos arquitectónicos
- información del terreno
- planos
- vistas y elevaciones
- análisis técnico
- materiales
- visualización
- asistencia mediante IA

## Evolución de la idea

Se pasó de pensar en una interfaz informativa a pensar en una **plataforma de trabajo profesional**.

La prioridad empezó a ser:

```text
Información
    ↓
Visualización
    ↓
Análisis
    ↓
Decisión
    ↓
Asistencia mediante IA
```

---

# SEMANA 2 — Diseño visual y referencia

## Qué se definió

Se tomó como referencia la interfaz visual proporcionada y se identificó una dirección de diseño:

**Dark Architectural / Engineering Pro Software**

Características:

- fondo oscuro
- paneles compactos
- sidebar
- header técnico
- dashboard denso pero ordenado
- visor principal
- planos
- análisis del terreno
- información técnica
- asistente IA

## Decisión visual

Se descartó una estética excesivamente gamer/cyberpunk.

La dirección quedó enfocada en:

- profesional
- moderna
- tecnológica
- técnica
- elegante
- compacta

## Sistema visual inicial

```text
#07111A  Fondo
#0B1722  Fondo secundario
#0F1D2A  Panel
#122536  Panel elevado
#F4F7FA  Texto
#8FA3B5  Texto secundario
#1597E5  Azul
#20C7F5  Cian
#39D98A  Éxito
#F2C94C  Advertencia
#FF647C  Error
```

---

# SEMANA 3 — Arquitectura de la interfaz

## Qué se pensó implementar

La interfaz se organizó como un software profesional:

```text
Sidebar
   ↓
Header
   ↓
Proyecto
   ↓
Tabs
   ↓
Dashboard
```

Con módulos para:

- Inicio
- Proyectos
- Nuevo proyecto
- Terrenos
- Materiales
- Visualización 3D
- IA
- Configuración

## Componentes identificados

Se decidió trabajar con componentes reutilizables como:

- Sidebar
- Header
- ProjectHeader
- ProjectTabs
- Panel
- Button
- Badge
- TerrainViewer
- LayerPanel
- ViewSelector
- TerrainInfo
- PlanCard
- ElevationCard
- TerrainAnalysis
- TerrainSection
- ImplantationPlan
- AIAssistant
- MetricCard
- Modal
- ImageViewer

La intención fue evitar duplicar HTML/CSS y permitir que el proyecto creciera de forma ordenada.

---

# SEMANA 4 — Terrenos y visualización técnica

## Evolución funcional

La plataforma dejó de centrarse solamente en mostrar proyectos y pasó a incorporar información técnica del terreno.

Se definieron datos como:

```text
Ancho
Largo
Área
Pendiente
Elevación
Orientación
Forma
```

## Visualizaciones previstas

- Vista 3D
- Vista superior
- Vista frontal
- Vista lateral
- Vista isométrica
- perfil de elevación
- sección del terreno
- mapa de análisis
- plano de implantación

## Capas

Se planteó un sistema de capas:

```text
☑ Terreno
☑ Construcción
☑ Vegetación
☐ Vías y accesos
☐ Límites
```

La intención es que el usuario pueda activar/desactivar información sin abandonar la vista principal.

---

# SEMANA 5 — Planos, vistas y arquitectura

## Módulos definidos

Se incorporaron al concepto del producto:

### Planos

- Planta Baja
- Planta Alta
- descarga de plano

### Elevaciones

- Fachada Frontal
- Fachada Posterior
- Fachada Lateral Izquierda
- Fachada Lateral Derecha

### Implantación

- terreno
- vivienda
- acceso
- norte
- cotas
- áreas verdes
- parqueadero
- límites

La aplicación empezó a evolucionar hacia una herramienta que combina **documentación arquitectónica + análisis + visualización**.

---

# SEMANA 6 — IA integrada al proyecto

## Evolución

La IA dejó de pensarse como un simple chatbot decorativo.

La idea evolucionó hacia una IA contextual que conozca el proyecto que el usuario está consultando.

Flujo previsto:

```text
Usuario selecciona proyecto
        ↓
Backend obtiene datos relevantes
        ↓
Proyecto + terreno + información técnica
        ↓
Contexto enviado a IA
        ↓
Respuesta contextual
        ↓
Asistente IA en la interfaz
```

## Ejemplos de consultas

```text
¿Qué me recomiendas para este terreno?

¿Qué orientación sería conveniente?

¿Qué materiales podría considerar?

¿Cómo aprovechar mejor el terreno?

¿Qué problemas debería revisar antes de construir?
```

## Recomendaciones

La IA puede devolver módulos como:

- Distribución
- Materiales
- Orientación
- Aprovechamiento del terreno
- Construcción
- Drenaje

Si todavía no existe un backend de IA, utilizar respuestas mock durante la primera implementación y dejar la integración real preparada mediante un servicio separado.

---

# SEMANA 7 — Base de datos y backend

## Evolución arquitectónica

Para evitar que toda la información viva en el frontend, se plantea separar:

```text
Frontend
    ↓
API / Backend
    ↓
Services
    ↓
Repositories
    ↓
PostgreSQL
```

## Entidades principales previstas

```text
Usuario
Proyecto
Terreno
Plano
Elevación
Material
Recomendación
Archivo
ConversaciónIA
MensajeIA
```

## PostgreSQL

La base de datos debe almacenar información persistente y permitir que el proyecto crezca sin depender de datos hardcodeados.

El frontend no debe conectarse directamente a PostgreSQL.

---

# SEMANA 8 — Integración completa

## Objetivo propuesto

Unir todos los módulos:

```text
LOGIN
  ↓
PROYECTOS
  ↓
PROYECTO SELECCIONADO
  ├── Terreno
  ├── Vista 3D
  ├── Planos
  ├── Elevaciones
  ├── Materiales
  ├── Análisis
  └── IA
```

Cada módulo debe utilizar información real del proyecto seleccionado.

---

# SEMANA 9 — Calidad, seguridad y rendimiento

## Objetivos propuestos

Revisar:

- autenticación
- autorización
- validación de datos
- manejo de errores
- variables de entorno
- secretos fuera del código
- consultas parametrizadas
- protección de endpoints
- rendimiento del frontend
- carga de imágenes
- estados de loading
- estados vacíos
- estados de error

## UX

Agregar estados para:

```text
Loading
Success
Empty
Error
Disabled
Selected
Hover
Focus
```

---

# SEMANA 10 — Versión profesional

## Objetivo final

Llegar a una plataforma que pueda sentirse como un producto profesional completo.

### Frontend

```text
HTML
CSS
TypeScript
Componentes
Estado
Responsive
```

### Backend

```text
API
Services
Repositories
Validación
Autenticación
IA
```

### Base de datos

```text
PostgreSQL
Migraciones
Relaciones
Índices
Datos persistentes
```

### IA

```text
Contexto del proyecto
Conversaciones
Recomendaciones
Historial
```

---

# EVOLUCIÓN GENERAL DEL PROYECTO

La evolución conceptual puede resumirse así:

```text
IDEA INICIAL
Aplicación de arquitectura
        ↓
DISEÑO
Interfaz profesional y técnica
        ↓
VISUALIZACIÓN
Terreno + planos + vistas
        ↓
INTERACCIÓN
Capas + tabs + visor + proyecto
        ↓
INTELIGENCIA
Asistente IA contextual
        ↓
DATOS
Backend + PostgreSQL
        ↓
PLATAFORMA
Arquitectura completa y escalable
```

---

# PRINCIPIOS QUE SE DEBEN MANTENER

## 1. El diseño debe servir al trabajo

La estética nunca debe ocultar información técnica.

## 2. La IA debe tener contexto

No crear un chatbot aislado. La IA debe conocer el proyecto cuando sea necesario.

## 3. La base de datos debe ser la fuente persistente

No depender permanentemente de datos mock.

## 4. El frontend no debe contener toda la lógica

Separar UI, estado, servicios y datos.

## 5. Modularidad

Cada módulo debe poder evolucionar sin romper los demás.

## 6. No sobreingenierizar

Agregar arquitectura solo cuando aporte valor real.

## 7. Mantener la trazabilidad

Cada nueva funcionalidad debe poder relacionarse con:

```text
Proyecto
Usuario
Datos
UI
Servicio
Base de datos
```

---

# ESTADO DE IMPLEMENTACIÓN

Usar esta sección para que Claude Code marque el progreso real.

```text
[ ] Semana 1 — Idea y alcance
[ ] Semana 2 — Sistema visual
[ ] Semana 3 — Arquitectura UI
[ ] Semana 4 — Terrenos y visualización
[ ] Semana 5 — Planos y elevaciones
[ ] Semana 6 — IA contextual
[ ] Semana 7 — Backend y PostgreSQL
[ ] Semana 8 — Integración completa
[ ] Semana 9 — Seguridad y rendimiento
[ ] Semana 10 — Versión profesional
```

**No marcar una semana como terminada si realmente no fue implementada.**

## Estado real

Ninguna semana está marcada porque ninguna está completa. Lo que existe de cada una:

| Semana | Hecho | Falta |
|---|---|---|
| 2 — Sistema visual | Paleta de colores aplicada en el frontend. | El resto del sistema visual; hay dos paletas distintas en los documentos y hay que elegir una. |
| 3 — Arquitectura UI | Páginas de inicio de sesión, proyectos y detalle de proyecto con cabecera, pestañas por sección y paneles. | Sidebar y los demás componentes listados. |
| 4 — Terrenos y visualización | Registro de terrenos (ancho, largo, área, pendiente, suelo, coordenadas). Esquemas de vista superior y perfil de pendiente. | Vista 3D, vistas frontal, lateral e isométrica, mapa de análisis, capas y forma del lote distinta de un rectángulo. |
| 5 — Planos y elevaciones | Registro de planos y elevaciones, con archivos adjuntos (PDF o imagen). | Plano de implantación y visor. |
| 6 — IA contextual | Asistente por proyecto con conversaciones guardadas; recomendaciones automáticas por reglas. | Probar el asistente con una clave real. |
| 7 — Backend y PostgreSQL | API completa para todas las entidades, con pruebas automatizadas. | Migraciones: los cambios de esquema se aplican a mano con `schema.sql`. Las pruebas pasan contra PostgreSQL 16 (ver `docs/06_PRUEBAS.md`). |
| 8 — Integración completa | Login, proyectos y módulos de terreno, planos, elevaciones, materiales, análisis e IA usando datos reales del proyecto. | Vista 3D. |
| 9 — Seguridad y rendimiento | Autenticación, permisos por dueño, validación de datos, manejo de errores, secretos fuera del código, estados de carga, vacío y error. | Límite de intentos de login, recuperación de contraseña, revisión de rendimiento. |
| 10 — Versión profesional | — | Todo. |

---

# INSTRUCCIÓN PARA CLAUDE CODE

Utiliza este documento como contexto de evolución del producto.

Cuando implementes una nueva funcionalidad:

1. Identifica en qué etapa encaja.
2. Respeta las decisiones visuales y arquitectónicas anteriores.
3. No inventes funcionalidades que contradigan el concepto.
4. Si una etapa futura todavía no está implementada, déjala preparada de forma razonable, pero no la declares terminada.
5. Prioriza primero las funcionalidades necesarias para que el producto funcione.

No gastes tokens explicando toda esta historia en cada ejecución. Úsala como contexto y trabaja directamente.
