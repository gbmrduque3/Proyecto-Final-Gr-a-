## Why

El esquema de circuitos en `schema.html` tiene un desfase de alineación visual en el cableado del segundo puente H y el motor de elevación, lo que resta calidad profesional al diagrama. Además, para facilitar el aprendizaje y depuración de la maqueta de la grúa torre, los usuarios necesitan conocer con precisión el flujo de conexión de cada pin individual al interactuar con él (hover), resaltando toda su trayectoria electrónica de un solo vistazo.

## What Changes

- Re-alineación de las salidas del segundo puente H y el motor de elevación en el diagrama interactivo de `schema.html`.
- Incorporación de interactividad en los pines físicos del diagrama SVG (hover), mostrando su propósito técnico en español y su destino en el panel de información inferior.
- Resaltado dinámico del cableado y filas de la tabla correspondientes al pin sobre el cual se realiza hover.

## Capabilities

### New Capabilities

<!-- Ninguna nueva capacidad; extendemos la guía de cableado existente -->

### Modified Capabilities

- `wiring-guide`: Requisitos de interacción y alineación de la guía de cableado electrónica.

## Impact

- `schema.html`: Estilos CSS, estructura SVG, tabla de asignación de pines y lógica JavaScript.
