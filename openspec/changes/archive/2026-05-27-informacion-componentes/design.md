## Context

Actualmente en `schema.html`, los componentes del SVG están dibujados como bloques estáticos. Para enriquecer la documentación del hardware, se añadirá interactividad. Al hacer clic sobre los componentes del SVG, se mostrará una ventana emergente (modal nativo `<dialog>`) que contenga el nombre, descripción técnica en español y parámetros de hardware básicos del componente. Al pasar el cursor, estos componentes se escalarán y brillarán ligeramente para denotar que son interactivos.

## Goals / Non-Goals

**Goals:**
- Implementar un modal emergente interactivo utilizando el elemento nativo `<dialog>` de HTML5.
- Mostrar una ficha técnica bien estructurada (nombre, categoría, descripción y tabla de parámetros) al hacer clic en cualquiera de los 6 tipos de componentes del SVG (Arduino Nano, ESP32, Joysticks, H-Bridges, DC Motors, Fuente de Potencia).
- Estilizar la interactividad de hover en el SVG con un cursor pointer, incremento de escala suave al 102% y un brillo cian exterior.
- Mantener la accesibilidad nativa del modal (tecla Escape para cerrar, bloqueo de scroll en el fondo mediante el uso de showModal).

**Non-Goals:**
- Cambiar la interactividad existente de los cables y la tabla de conexiones.

## Decisions

### 1. Elemento Nativo `<dialog>` de HTML5
- **Decisión**: Utilizar la etiqueta `<dialog>` en lugar de un modal hecho con `<div>` tradicionales.
- **Razón**: El elemento `<dialog>` proporciona comportamiento de accesibilidad nativo (como atrapar el foco del teclado dentro del modal, cerrarse con la tecla `Escape` de forma predeterminada, y aislar la página detrás del backdrop). Además, permite el uso del pseudo-elemento `::backdrop` para desenfocar y oscurecer el fondo con CSS moderno: `backdrop-filter: blur(4px); background-color: rgba(15, 23, 42, 0.4);`.
- **Alternativa**: Un modal hecho con divs controlados por JS. Requiere mucha más programación y código CSS repetitivo.

### 2. Estilos Interactivos y Escala en SVG
- **Decisión**: Envolver los elementos de cada componente del SVG en un elemento de grupo `<g class="clickable-component" data-component="...">`. Aplicar `transform-origin: center` mediante CSS y dar un efecto de escala `:hover { transform: scale(1.02); }` y sombra `:hover { filter: drop-shadow(0 4px 10px rgba(8, 145, 178, 0.18)); }`.
- **Razón**: Brinda retroalimentación visual inmediata e intuitiva al usuario de que el componente es interactivo sin interferir con las etiquetas de pines de su alrededor.
- **Alternativa**: Resaltar solo cambiando el grosor del borde. Menos llamativo e intuitivo.

### 3. Ficha Técnica y Objeto en JS
- **Decisión**: Crear un objeto `const COMPONENT_DETAILS = { ... }` en el script JavaScript de `schema.html` que guarde toda la información técnica en español de los componentes para inyectarla de forma dinámica en el modal antes de abrirlo.
- **Razón**: Permite mantener una única ventana modal en el HTML y reutilizarla dinámicamente, ahorrando líneas de código repetidas y facilitando el mantenimiento.

## Risks / Trade-offs

- **[Riesgo] Escala del SVG descentrada** ➡️ Al escalar los grupos `<g>` en SVG mediante CSS `transform: scale()`, el punto de pivote puede desalinearse si no se define bien.
  - *Mitigación*: Se definirá `transform-origin` de cada componente individualmente en CSS usando su centro aproximado o se aplicará el estilo CSS `transform-box: fill-box; transform-origin: center;` que calcula automáticamente el centro del bloque.
