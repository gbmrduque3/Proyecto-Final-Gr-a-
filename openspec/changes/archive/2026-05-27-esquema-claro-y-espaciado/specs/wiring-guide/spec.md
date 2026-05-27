## MODIFIED Requirements

### Requirement: Documentación de Conexiones Electrónicas
El sistema SHALL proveer una guía de conexiones electrónica interactiva a través del archivo `schema.html` ubicado en la raíz.

#### Scenario: Visualización del diagrama y componentes
- **WHEN** El usuario abre `schema.html`
- **THEN** Se presenta una guía electrónica interactiva estructurada en una sola columna vertical con un tema de fondo claro (crema/papel), que sitúa el diagrama SVG interactivo con componentes espaciados en la parte superior y la tabla de asignación de pines y filtros en la parte inferior.

#### Scenario: Filtrado interactivo de conexiones
- **WHEN** El usuario selecciona una categoría de conexiones (ej. "Alimentación", "Comunicación Serial")
- **THEN** Los cables correspondientes en el SVG se iluminan mientras que las demás conexiones reducen su opacidad para mejorar la visibilidad.

#### Scenario: Hover cruzado interactivo
- **WHEN** El usuario posiciona el cursor sobre una fila de la tabla o sobre un componente del SVG
- **THEN** El cable asociado en el SVG se ilumina y la fila de la tabla de conexiones correspondiente se resalta visualmente en su lugar sin desplazar la página (no scroll).

#### Scenario: Desplazamiento por clic en SVG
- **WHEN** El usuario hace clic sobre un cable de conexión en el SVG
- **THEN** La tabla de conexiones se desplaza suavemente (scroll) para mostrar la fila de conexión correspondiente y la resalta.
