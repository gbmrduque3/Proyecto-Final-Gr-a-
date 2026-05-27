## Context

El archivo `schema.html` actual contiene una guía de cableado con un tema oscuro y denso. Para mejorar la legibilidad y la experiencia del usuario, se migrará a un diseño con colores claros de estilo crema/papel. Además, se reubicará la tabla de mapeo debajo del diagrama SVG (en lugar de a la derecha) para permitir que el lienzo interactivo ocupe todo el ancho horizontal, eliminando el amontonamiento del cableado.

## Goals / Non-Goals

**Goals:**
- Implementar un diseño claro de tipo crema/papel con fondo suave (`#fcfbf9`) y cuadrícula técnica sutil (`#f3eee3`).
- Rediseñar el layout de la página en una sola columna vertical: cabecera, diagrama SVG de ancho completo, panel de detalles de conexión y tabla/filtros al final.
- Ampliar el lienzo del SVG interactivo a `1150 x 700` píxeles para separar físicamente los bloques de circuitos.
- Reubicar los componentes del SVG en 4 columnas bien espaciadas para mejorar la claridad de ruteo de las líneas ortogonales.
- Mantener los pines y la lógica de conexiones exactas de `grua_arduino.ino`.

**Non-Goals:**
- Modificar el código fuente de Arduino (`grua_arduino.ino`) o de ESP32.
- Alterar la lógica básica de interactividad bidireccional entre la tabla y el SVG.

## Decisions

### 1. Estructura de Diseño en Una Columna (Vertical Stack)
- **Decisión**: Colocar la tabla de conexiones en la parte inferior de la pantalla en lugar del diseño anterior de dos columnas (izquierda/derecha).
- **Razón**: Al apilar el SVG y la tabla verticalmente, cada uno adquiere un ancho máximo en pantalla de hasta `1250px`. Esto proporciona la anchura óptima necesaria para un SVG de 1150px sin requerir barras de desplazamiento en pantallas de escritorio estándar.
- **Alternativa**: Mantener el diseño en dos columnas pero duplicar el tamaño general de la pantalla. Esto habría requerido resoluciones de pantalla extremadamente altas (más de 1800px de ancho) y provocaría problemas de usabilidad.

### 2. Paleta de Colores de Estilo Crema/Papel
- **Decisión**: Usar un fondo crema suave (`#fcfbf9`) con componentes claros (Arduino en azul suave, Drivers en rosa claro, etc.) y líneas de cableado brillantes.
- **Razón**: Reduce la fatiga visual en comparación con un tema blanco puro brillante, manteniendo un contraste excelente con los cables y etiquetas de los pines.
- **Alternativa**: Usar una paleta de color blanco puro clínico. Considerado menos agradable estéticamente para lecturas prolongadas de planos técnicos.

### 3. Lienzo SVG de 1150 x 700 píxeles y Organización en Columnas
- **Decisión**: Ampliar el lienzo a `1150 x 700` px y organizar los bloques en 4 columnas principales de izquierda a derecha.
- **Razón**: Aumenta la distancia horizontal libre entre los bloques del microcontrolador y los drivers/entradas de 40px a un promedio de 90px-110px. Esto permite trazar los cables con ruteo ortogonal (ángulos de 90 grados) con espacio de separación de 15px entre líneas paralelas.

## Risks / Trade-offs

- **[Riesgo] Pantallas muy pequeñas (Móviles)** ➡️ El SVG de 1150px de ancho requerirá desplazamiento horizontal en pantallas móviles.
  - *Mitigación*: Se utiliza CSS `overflow-x: auto` en el contenedor `.svg-container` para permitir un desplazamiento lateral suave en dispositivos móviles sin desbordar el contenedor de la página.
