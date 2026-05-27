## ADDED Requirements

### Requirement: Interactividad en Pines Individuales
El sistema SHALL permitir la interacción visual e informativa con cada uno de los pines de conexión en el diagrama interactivo.

#### Scenario: Hover en pin individual
- **WHEN** El usuario posiciona el cursor (hover) sobre un punto de pin (`.pin-dot`) o su etiqueta en el SVG de `schema.html`
- **THEN** El pin cambia su estilo visual de forma destacada, se actualiza el panel inferior de información con el nombre y la descripción técnica detallada del pin en español, se iluminan los cables vinculados a su trayectoria (ocultando/atenuando los demás) y se resaltan las filas correspondientes en la tabla de mapeo.

### Requirement: Alineación de Componentes de Salida
El diagrama interactivo SHALL presentar una alineación visual perfecta y profesional de todos los actuadores y componentes de potencia.

#### Scenario: Alineación horizontal de elevación
- **WHEN** El usuario visualiza la sección inferior derecha del diagrama interactivo
- **THEN** Las salidas del segundo puente H y las entradas del motor de elevación se alinean de forma perfectamente simétrica y recta en el plano horizontal, sin que los cables de conexión sobresalgan por encima de los límites de los componentes.
