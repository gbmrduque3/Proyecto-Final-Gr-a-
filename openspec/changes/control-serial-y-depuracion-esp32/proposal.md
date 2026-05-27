## Why

La implementación actual en MicroPython sobre el ESP32 genera problemas de memoria y congelamiento del programa al intentar servir archivos web pesados concurrentemente con la lógica de red. Esta propuesta delega la interfaz web al navegador de la laptop del usuario mediante la Web Serial API (USB) y reutiliza el ESP32 exclusivamente como un monitor de depuración inalámbrica ultra-sencillo para el programador.

## What Changes

*   **Interfaz de Control Local**: Se traslada el servidor web del ESP32 a la laptop mediante una interfaz local en el navegador usando la Web Serial API nativa de Chrome/Edge por cable USB.
*   **Canal de Depuración Secundario (SoftwareSerial)**: Se configuran los pines `D9` y `D12` del Arduino Nano para enviar registros de eventos en tiempo real.
*   **Depurador Inalámbrico ESP32**: El ESP32 se limita a leer del canal serie secundario del Arduino Nano y a servir una página web en texto plano en la red local (Wi-Fi) con la información del buffer de depuración.

## Capabilities

### New Capabilities
- `serial-control`: Control en tiempo real y recepción de telemetría de la grúa torre mediante Web Serial API directamente en el navegador por USB.
- `developer-debugging`: Visualización de trazas y logs de depuración del Arduino Nano de forma inalámbrica a través del servidor web minimalista del ESP32.

### Modified Capabilities
- `wiring-guide`: Se modifican las especificaciones del cableado del sistema para incorporar la conexión SoftwareSerial (D9/D12) entre el Arduino Nano y el ESP32 para diagnóstico secundario, manteniendo el USB para el flujo principal de telemetría.

## Impact

*   **Código de Arduino (`grua_arduino.ino`)**: Integración de la librería `SoftwareSerial` en los pines D9 y D12, y desvío de los mensajes informativos/de depuración hacia este canal secundario.
*   **Código de ESP32 (`main.py` y `boot.py`)**: Eliminación del servidor de interfaz interactiva pesada. Implementación de una lectura serial simple y un servidor web de socket minimalista con recarga automática.
*   **Interfaz Web (`index.html`/`schema.html`)**: Incorporación de controles para apertura y administración de la conexión Web Serial API en el navegador.
