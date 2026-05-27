# Prompt de Implementación Reutilizable: Control USB (Web Serial API) y Depuración Inalámbrica (ESP32)

Este prompt puede ser utilizado en modelos de IA para aplicar este mismo sistema de control dual cableado e inalámbrico en otros proyectos basados en microcontroladores.

---

```markdown
Actúa como un desarrollador experto en sistemas embebidos, firmware de microcontroladores (Arduino C++ y MicroPython) y desarrollo frontend. Necesito que rediseñes la arquitectura de control y depuración de un proyecto de robótica/IoT que consta de:
1. Un microcontrolador principal de control (ej. Arduino Nano o Arduino Uno) conectado a actuadores.
2. Un módulo secundario de conectividad inalámbrica (ej. ESP32 o ESP8266) programado en MicroPython.
3. Una interfaz web responsiva e interactiva (HTML/CSS/JS).

## Problema
El módulo inalámbrico (ESP32) experimenta cuellos de botella e inestabilidades de memoria (OutOfMemory) al intentar procesar solicitudes de control concurrentes mientras sirve archivos web pesados de la interfaz interactiva.

## Nueva Solución Requerida

### 1. Control Local e Interfaz (Navegador Laptop <---> Arduino vía USB)
*   **Servidor Web**: La interfaz web (`index.html`) se cargará localmente en el navegador de la laptop.
*   **Web Serial API**: Incorporar en el JavaScript de la web el control de conexión directa a través de la Web Serial API nativa de Chrome/Edge.
*   **Canal Principal**: La comunicación de control y telemetría JSON principal debe realizarse exclusivamente por el cable USB a 9600 bps.
*   **Comandos**: Mapear clics de botones y teclas a caracteres simples ('F', 'B', 'U', etc.) transmitidos por puerto serie.
*   **Telemetría**: Leer y procesar asíncronamente las tramas JSON recibidas del microcontrolador por el puerto serie para actualizar el estado visual de la UI.
*   **Fallback**: Mantener el sistema de llamadas HTTP (`fetch`) original únicamente como método de respaldo alternativo si el USB no está conectado.
*   **Navegación Cruzada**: Incorporar botones y enlaces de navegación interactivos en el encabezado de las páginas web para saltar de forma fluida y simétrica entre la interfaz de control principal (`index.html`) y el diagrama de conexiones (`schema.html`) en ambos sentidos.

### 2. Monitor de Depuración Inalámbrico (Arduino <---> ESP32 <---> Celular)
*   **Canal Secundario (SoftwareSerial)**: Configurar un puerto serie emulado por software (ej. pines D12/D9) en el microcontrolador principal a 9600 bps.
*   **Trazas de Log**: Redirigir todos los mensajes de estado textuales, logs de eventos y depuración a este puerto secundario hacia el ESP32.
*   **Lectura en ESP32**: En el código de MicroPython del ESP32, leer del puerto serie RX secundario y guardar las últimas 50 líneas recibidas en una lista circular en RAM.
*   **Servidor de Depuración**: Levantar un servidor socket web minimalista en el ESP32 (puerto 80).
*   **Interfaz HTML de Logs**: Servir una página HTML ultraliviana (con estética de terminal retro, fondo negro y texto verde) que muestre los logs acumulados y aplique un auto-refresco automático (`<meta http-equiv="refresh" content="2">`) cada 2 segundos. Esto permitirá al programador ver el estado de la grúa desde su celular sin cables.

### 3. Organización y Limpieza de Archivos
*   **Estructura de Carpetas**: Clasificar todos los archivos del proyecto en tres directorios dedicados: `/arduino/` (código del microcontrolador principal), `/esp32/` (archivos `boot.py` y `main.py` de MicroPython para depuración) y `/web_server/` (páginas HTML, JS, CSS y esquemas de la interfaz de control).
*   **Depuración de Raíz**: Una vez organizados los archivos en sus respectivas carpetas, eliminar las copias del directorio raíz para evitar archivos huérfanos u obsoletos y mantener el espacio de trabajo limpio.

Por favor, genera:
1. El código C++ para el microcontrolador principal con la integración de SoftwareSerial y el control exclusivo.
2. El script `main.py` de MicroPython para el ESP32 con el lector de logs y el mini servidor web asíncrono.
3. Las modificaciones de diseño y código Javascript en `index.html` para la Web Serial API, el botón de conexión, los enlaces de navegación y la actualización de la UI.
4. Las instrucciones claras para crear las tres carpetas de organización y realizar la limpieza de archivos obsoletos de la raíz.
```
