# Especificación de Diseño: Presentación Luxury y Navegación Bidireccional (TorreGrúa)

Este documento detalla el plan de diseño y reestructuración de `presentacion.html` para el proyecto Grúa Torre de Control Dual, incorporando las 10 diapositivas del PowerPoint institucional y la lógica de navegación entre páginas.

## Objetivos del Diseño

1. **Estilo Luxury Institucional**: Implementar una paleta de colores de lujo basada en la identidad del Colegio Paulo VI (azul marino profundo `#192b6d` y amarillo/oro `#ede221`) utilizando bordes ultra sutiles y efectos glassmorphism.
2. **Priorización de Multimedia**: Reducir el texto de las diapositivas a viñetas concisas y destinar la mayor parte de las tarjetas a cuadrículas de imágenes extraídas del PowerPoint y al video `.mp4`.
3. **Navegación Bidireccional Inteligente**: Permitir al usuario saltar de la diapositiva electrónica a `schema.html` y retornar automáticamente a la misma posición en `presentacion.html`.
4. **Espacio Tridimensional Prezi**: Ampliar la matriz de coordenadas 3D para 10 diapositivas.

---

## 1. Sistema de Estilos Luxury (CSS)

La presentación se adaptará usando las siguientes variables CSS:

```css
:root {
    --bg-color: radial-gradient(circle at 50% 50%, #0d173c 0%, #050818 100%);
    --panel-bg: rgba(13, 23, 60, 0.65);
    --border-color: rgba(237, 226, 33, 0.18);
    --accent-color: #ede221;
    --accent-glow: rgba(237, 226, 33, 0.25);
    --text-primary: #ffffff;
    --text-secondary: #9da8d1;
    --card-shadow: 0 20px 50px rgba(0, 0, 0, 0.4);
}
```

* **Tipografías**:
  * Títulos y encabezados: `Poppins` (Google Fonts).
  * Texto de lectura: `Nunito` (Google Fonts).
* **Efecto Glassmorphism**: Desenfoque de fondo mejorado (`backdrop-filter: blur(20px)`) para paneles y tarjetas flotantes.

---

## 2. Coordenadas Espaciales 3D (Cámara Prezi)

Las 10 diapositivas se colocarán en posiciones tridimensionales distribuidas alrededor de un espacio virtual de 360 grados:

| Diapositiva | Título | Coordenada X | Coordenada Y | Coordenada Z | Rotación (R) | Escala (S) |
|---|---|---|---|---|---|---|
| **1** | Portada | 0 | 0 | 0 | 0° | 1.0 |
| **2** | Introducción | 1800 | 400 | -400 | -15° | 0.7 |
| **3** | Objetivos | 800 | 1400 | 300 | 20° | 1.1 |
| **4** | Sección Técnica (Separador) | -1200 | 1200 | -200 | -25° | 0.8 |
| **5** | Electrónica y Control (Redirección) | -600 | -800 | 400 | 15° | 1.2 |
| **6** | Diseño Mecánico CAD | 1400 | -1600 | -300 | -10° | 0.95 |
| **7** | Fabricación Física | 2800 | -800 | 600 | 25° | 1.3 |
| **8** | Ensamblaje Operativo | -2800 | 0 | -600 | -35° | 0.75 |
| **9** | Conclusiones | -2200 | -1800 | 200 | 5° | 1.05 |
| **10** | Cierre de Presentación | 0 | -2600 | 900 | 45° | 1.4 |

---

## 3. Lógica de Navegación Bidireccional

### De `presentacion.html` a `schema.html`:
En la Diapositiva 5, se añade un botón interactivo:
```html
<a id="btnLinkSchema" href="schema.html" class="nav-btn-luxury">🔌 Ver Esquema Electrónico Interactivo</a>
```
En el script de la presentación, este botón actualizará su enlace de manera dinámica al cambiar de diapositiva:
```javascript
const schemaBtn = document.getElementById('btnLinkSchema');
if (schemaBtn) {
    schemaBtn.href = `schema.html?fromSlide=${currentSlideIdx}`;
}
```

### De `schema.html` a `presentacion.html`:
En `schema.html`, un script leerá el parámetro al cargar la página y modificará el enlace de retorno:
```javascript
window.addEventListener('DOMContentLoaded', () => {
    const urlParams = new URLSearchParams(window.location.search);
    const fromSlide = urlParams.get('fromSlide');
    if (fromSlide !== null) {
        const presBtn = document.querySelector('a[href="presentacion.html"]');
        if (presBtn) {
            presBtn.href = `presentacion.html?slide=${fromSlide}`;
        }
    }
});
```

### Al recargar `presentacion.html`:
La presentación verificará la URL y cargará directamente la diapositiva correspondiente:
```javascript
const urlParams = new URLSearchParams(window.location.search);
const slideParam = urlParams.get('slide');
if (slideParam !== null) {
    currentSlideIdx = parseInt(slideParam, 10);
}
```

---

## 4. Contenido Simplificado y Multimedia

### Diapositivas e Imágenes Asignadas:
1. **Portada**: Imagen de fondo sutil o collage. Logo Paulo VI: `Marca-Paulo-VI-Fondos-Oscuro.png` en el centro.
2. **Introducción**: Video `.mp4` (`VAHK_haVdY0.mp4`) en reproducción automática muda (`autoplay loop muted`), y fotos contextuales (`image8.png`, `image9.png`, `image10.jpeg`).
3. **Objetivos**: Cuadrícula de fotos (`image11.png` a `image17.png`).
4. **Sección Técnica**: Gran toma de los componentes electrónicos (`image18.png` y `image19.png`).
5. **Electrónica y Control**: Hardware físico en fotos (`image21.png`, `image22.png`, `image23.png`) y el botón de redirección.
6. **Diseño CAD**: Capturas de Fusion 360 (`image24.png` a `image27.png`).
7. **Fabricación**: Impresión 3D y corte láser (`image28.png` a `image32.png`).
8. **Ensamblaje**: Proceso de montura de la grúa (`image33.png` a `image39.png`).
9. **Conclusiones**: Imágenes del prototipo finalizado en pruebas (`image40.png` a `image43.png`).
10. **Cierre**: Logotipo de cierre institucional y foto general.
