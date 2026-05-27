## Context

La arquitectura inicial de control de la grúa torre dependía de que el ESP32 sirviera una página web interactiva compleja y gestionara solicitudes HTTP asíncronas para reenviar comandos al Arduino Nano por UART. Sin embargo, debido a las limitaciones de memoria de MicroPython en el ESP32, el procesamiento concurrente causaba congelamiento de la aplicación y problemas de desbordamiento de RAM (OutOfMemory).

## Goals / Non-Goals

**Goals:**
*   Eliminar la carga del servidor de interfaz interactiva en el ESP32.
*   Implementar un canal de control por cable directo desde la laptop al Arduino Nano a través de Web Serial API.
*   Reutilizar el ESP32 como una herramienta inalámbrica exclusiva para depuración del desarrollador (Wireless Debug Monitor).
*   Asegurar que los datos mostrados en el celular del desarrollador se refresquen de forma periódica automática.

**Non-Goals:**
*   No se pretende agregar interfaces interactivas ni lógica de control de motores dentro del ESP32.
*   No se incluye control de la grúa desde el celular (el celular solo sirve para visualización de logs).

## Decisions

### Decision 1: Uso de Web Serial API en el Frontend
*   **Decisión**: La laptop se comunica directamente con el Arduino Nano a través de Web Serial API por USB.
*   **Razón**: Elimina la necesidad de programar un servidor local en Node.js o Python en la laptop, permitiendo que un archivo HTML/JS estático abierto en el navegador controle el hardware directamente de forma segura y sin dependencias.
*   **Alternativas consideradas**: Servidor local en Python con Flask o Node.js. Se descartaron por requerir que el usuario instale entornos de ejecución adicionales.

### Decision 2: Canal Secundario con SoftwareSerial
*   **Decisión**: El Arduino Nano se comunicará con el ESP32 a través de una conexión serie emulada por software (`SoftwareSerial`) en los pines `D12` (RX) y `D9` (TX).
*   **Razón**: Permite mantener el puerto serie físico principal (`Serial`, TX/RX hardware) libre para la comunicación bidireccional USB con el navegador. Esto evita la mezcla y corrupción de datos de depuración con las tramas JSON de telemetría de control.
*   **Alternativas consideradas**: Compartir la línea TX del puerto hardware para que el ESP32 "escuche" pasivamente. Se descartó por el riesgo de ruido y la imposibilidad de enviar trazas exclusivas para depuración.

### Decision 3: Servidor Minimalista en el ESP32
*   **Decisión**: El ESP32 ejecutará un script en MicroPython que mantiene un buffer de los últimos 50 logs y los sirve en una página HTML ultra liviana con auto-refresco cada 2 segundos.
*   **Razón**: Minimiza el uso de CPU y RAM, eliminando por completo los problemas de falta de memoria y bloqueos.

## Risks / Trade-offs

*   **[Riesgo]**: SoftwareSerial a alta velocidad consume recursos de interrupción en el Arduino Nano.
    *   *Mitigación*: Se configurará a una velocidad baja y estable de 9600 baudios, enviando logs concisos.
*   **[Riesgo]**: Niveles lógicos de voltaje incompatibles (Arduino 5V vs ESP32 3.3V).
    *   *Mitigación*: Se debe usar una resistencia de protección o divisor de voltaje en la línea TX del Nano (D9) hacia el RX del ESP32 (GPIO 16).
