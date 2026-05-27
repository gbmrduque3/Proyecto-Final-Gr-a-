## Why

El esquema de conexiones electrónicas actual (`schema.html`) utiliza un diseño oscuro y denso que dificulta la legibilidad. Además, la distribución compacta de los componentes en el SVG provoca que las líneas de cableado se crucen de forma amontonada, y la visualización de dos columnas es incómoda para pantallas medianas debido al ancho compartido con la tabla de pines.

## What Changes

- Cambiar la paleta de colores del panel a tonos claros y cálidos (estilo crema/papel).
- Modificar el diseño de la página a una sola columna vertical, posicionando el diagrama interactivo de circuitos en la parte superior y la tabla de asignación de pines y filtros en la parte inferior.
- Ampliar las dimensiones del lienzo del diagrama SVG a `1150 x 700` píxeles para espaciar físicamente los componentes.
- Reorganizar la posición de los componentes en el SVG en columnas lógicas (Entradas ➡️ Cerebro ➡️ Controladores ➡️ Actuadores) y optimizar el ruteo ortogonal de los cables para evitar cruces innecesarios.

## Capabilities

### New Capabilities

- Ninguna.

### Modified Capabilities

- `wiring-guide`: Modificar los requisitos visuales de la guía interactiva, requiriendo un tema claro, un lienzo espaciado con componentes reorganizados y una disposición vertical con la tabla en la parte inferior.

## Impact

- `schema.html`: Rediseño completo de estilos CSS, estructura HTML, coordenadas SVG y rutas de cableado.
