## Context

Actualmente, `schema.html` representa el circuito mediante un SVG estático en cuanto a pines individuales se refiere (los hover y clicks solo actúan sobre cables y componentes completos). Adicionalmente, el segundo puente H y el motor de elevación tienen coordenadas desfasadas que dibujan los pines AO1 y AO2 fuera del cuerpo del puente H, requiriendo un cable con codos que se sale del esquema.

## Goals / Non-Goals

**Goals:**
- Alinear horizontalmente las salidas AO1 y AO2 del TB6612FNG (2) con las entradas M3+ y M3- del motor de elevación.
- Diseñar e implementar el mapeo de datos `PIN_DATA` en Javascript.
- Implementar listeners de hover (`mouseenter` / `mouseleave`) en todos los pines (`.pin-dot` y `.pin-label`).
- Actualizar el info panel y resaltar la trayectoria y tablas correspondientes.

**Non-Goals:**
- Modificar el funcionamiento del firmware del Arduino o ESP32.
- Modificar las conexiones físicas reales.

## Decisions

### Decisión 1: Resaltado dinámico en pines individuales utilizando referencias cruzadas existentes
- **Alternativa A**: Crear tooltips flotantes contextuales sobre el SVG.
- **Alternativa B (Elegida)**: Reutilizar el panel inferior de información `#wireName` y `#wireDesc` y el sistema de clases CSS (`.active-connection`, `.dimmed`, `.active-row`) ya implementado para el cableado.
- **Razón**: Aporta consistencia estética al panel y evita problemas de superposición y posicionamiento responsivo de popups en pantallas pequeñas.

### Decisión 2: Re-alineación horizontal directa del motor de elevación y sus pines
- **Alternativa A**: Mantener los codos del cable pero subir el bloque del motor.
- **Alternativa B (Elegida)**: Desplazar los pines AO1 y AO2 en el SVG del TB6612FNG (2) para que coincidan en la coordenada vertical `y` con el motor (`590` y `610`), haciendo que el cable sea una línea recta horizontal.
- **Razón**: Es la solución más limpia que mantiene intacto el contorno de ambos componentes y resulta en un diagrama más profesional y fácil de leer.

## Risks / Trade-offs

- **Riesgo**: Que al hacer hover en el pin se active también el hover del cable que pasa por encima o del componente agrupado.
- **Mitigación**: Añadir `e.stopPropagation()` en los manejadores de los pines para evitar burbujeo de eventos hacia los elementos contenedores (`.clickable-component` o `.wire-line`).
