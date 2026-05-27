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

#### Scenario: Clic en componente y despliegue de ficha técnica
- **WHEN** El usuario hace clic sobre un bloque de componente del SVG (Arduino, ESP32, Joysticks, Motores, Drivers o Fuente)
- **THEN** Se despliega una ventana emergente (modal nativo `<dialog>`) con fondo difuminado mostrando el título del componente, un badge de su categoría, su propósito en el sistema y una tabla con especificaciones técnicas básicas de hardware.

#### Scenario: Hover visual en componentes
- **WHEN** El usuario posiciona el cursor sobre un componente del SVG
- **THEN** El cursor cambia a tipo mano (pointer), el componente aumenta su escala sutilmente al 102% y adquiere un brillo cian suave en sus bordes.
