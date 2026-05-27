## 1. Estructura HTML y Estilos CSS del Modal

- [x] 1.1 Añadir el elemento emergente `<dialog id="componentModal">` en el HTML de `schema.html` con su maquetación básica (Cabecera, Badge de Categoría, Descripción, Tabla de Especificaciones y Botón de Cierre).
- [x] 1.2 Estilizar el modal y su fondo (`::backdrop`) en el archivo CSS de `schema.html`, aplicando el tema crema claro y difuminado de fondo (`backdrop-filter: blur(4px)`).

## 2. Interactividad y Estilos en el SVG

- [x] 2.1 Envolver las representaciones de los 6 tipos de componentes en el SVG de `schema.html` en elementos de grupo `<g>` con la clase `clickable-component` y el atributo `data-component="..."`.
- [x] 2.2 Agregar estilos CSS para la clase `clickable-component`, incluyendo `cursor: pointer`, cambio de escala del 2% (`scale(1.02)`) en `:hover` usando `transform-box: fill-box; transform-origin: center;` y un brillo exterior suave.

## 3. Lógica JavaScript para Ficha Técnica

- [x] 3.1 Definir el objeto `COMPONENT_DETAILS` en el script JavaScript de `schema.html` conteniendo las descripciones y especificaciones de cada componente.
- [x] 3.2 Implementar el controlador de eventos `click` en los elementos `.clickable-component` para cargar dinámicamente los datos técnicos en el modal y llamar a `showModal()`.
- [x] 3.3 Añadir lógica para cerrar el modal al hacer clic en el botón "Cerrar" o en la zona del backdrop.

