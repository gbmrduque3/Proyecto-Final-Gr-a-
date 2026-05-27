## 1. Alineación Visual de Conexiones del Segundo Puente H y Motor de Elevación

- [x] 1.1 Modificar las coordenadas verticales `cy` de los pines `AO1` y `AO2` del segundo puente H (`TB6612FNG (2)`) a `590` y `610` en `schema.html`.
- [x] 1.2 Actualizar las etiquetas de texto correspondientes a `AO1` y `AO2` en el SVG para que tengan sus coordenadas `y` en `593` y `613` respectivamente.
- [x] 1.3 Modificar el cableado de salida `out-elev` en el SVG para representarse mediante dos líneas rectas horizontales desde `x=970` a `x=1150` a las alturas `590` y `610`.

## 2. Interactividad y Resaltado de Pines

- [x] 2.1 Crear el objeto de mapeo `PIN_DATA` en el script JavaScript de `schema.html` conteniendo el nombre, descripción y cables asociados para todos los pines físicos del diagrama.
- [x] 2.2 Implementar manejadores de eventos `mouseenter` y `mouseleave` para todos los elementos `.pin-dot` para actualizar el panel de información, iluminar/atenuar cables, y marcar las filas de la tabla de conexiones.
- [x] 2.3 Detener la propagación de eventos con `e.stopPropagation()` al hacer hover sobre los pines para evitar disparar los eventos de hover del cable o componente base.
