## 1. Diseño y Estilos de schema.html

- [x] 1.1 Modificar la paleta de variables CSS (`:root`) en `schema.html` para el tema de fondo claro (crema/papel).
- [x] 1.2 Actualizar los estilos de tarjetas, componentes del SVG (Arduino, ESP32, Drivers, Joysticks, Motores) y cables para reflejar los nuevos colores claros y sombras sutiles.
- [x] 1.3 Modificar la estructura CSS para colocar el SVG y la tabla en una disposición de una columna de pila vertical de ancho completo.

## 2. Reubicación Física de Componentes y Cables en el SVG

- [x] 2.1 Actualizar las coordenadas de las cajas de componentes en el SVG para alinearlos en 4 columnas bien espaciadas (dimensiones de lienzo: `1150 x 700` px).
- [x] 2.2 Reubicar y trazar todas las rutas de los cables del SVG (`path d="..."`) para conectarlos a las nuevas posiciones de los componentes con ruteo ortogonal amplio y ordenado.
- [x] 2.3 Mantener la asignación exacta de pines de `grua_arduino.ino` en las etiquetas de texto del SVG.

## 3. Estructuración y Funciones de la Tabla

- [x] 3.1 Reubicar la tabla de mapeo de conexiones debajo del SVG en el HTML de `schema.html`.
- [x] 3.2 Modificar los colores y estilos de los botones de filtro rápido y las etiquetas (`badge`) de la tabla para que armonicen con el tema claro.
- [x] 3.3 Validar que el script Javascript mantenga el hover bidireccional, el panel de detalles técnico (`info-panel`), la búsqueda y la función de click-to-scroll.
