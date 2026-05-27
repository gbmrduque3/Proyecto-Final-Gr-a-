## Why

Actualmente, el esquema interactivo de conexiones lógicas (`schema.html`) muestra información al interactuar con los cables (conexiones), pero los bloques de componentes físicos (Arduino Nano, ESP32, Motores, Drivers, Joysticks, Fuente de potencia) no son interactivos y no proveen información sobre su propósito, rol o especificaciones básicas de hardware al usuario.

## What Changes

- Añadir interactividad al hacer clic en los bloques de componentes del esquema SVG.
- Mostrar una ventana emergente (modal flotante) elegante con el nombre del componente, su descripción en español y una tabla con especificaciones técnicas básicas de hardware.
- Implementar efectos visuales al hacer hover en los componentes del SVG (brillo de borde, cursor pointer, y escalado leve del 2%) para denotar interactividad.
- Utilizar el elemento HTML5 nativo `<dialog>` y estilos CSS avanzados para lograr un desenfoque de fondo (`backdrop-filter`) suave en el modal.

## Capabilities

### New Capabilities

- Ninguna.

### Modified Capabilities

- `wiring-guide`: Expandir la especificación del esquema de conexiones interactivo para requerir que los componentes físicos del SVG muestren su ficha técnica en un modal emergente al hacer clic sobre ellos, incluyendo cursor pointer y efecto hover de escala/brillo.

## Impact

- `schema.html`: Inclusión del modal `<dialog>`, estilos CSS para los componentes interactivos del SVG, ampliación del objeto Javascript de datos, y controladores de eventos `click` y `hover` en los componentes.
