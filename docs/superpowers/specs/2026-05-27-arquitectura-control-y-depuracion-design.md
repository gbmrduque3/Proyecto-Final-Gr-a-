# Diseño de Arquitectura: Control USB (Web Serial) y Depuración Inalámbrica (ESP32)

Este documento detalla la reestructuración del sistema de control de la grúa torre para solventar los límites de memoria del ESP32 bajo MicroPython.

## 1. Resumen de la Solución
Se traslada la lógica del servidor web y la interfaz de usuario directamente al navegador web de la laptop del usuario mediante la **Web Serial API** por conexión USB. El **ESP32** se libera de la carga de servir la interfaz interactiva y se reutiliza como una **consola inalámbrica de depuración** a la que el programador puede acceder desde su celular para verificar en tiempo real cómo procesa el Arduino Nano los comandos.

---

## 2. Arquitectura de Hardware y Conexiones

### A. Conexión de Control Principal (Cableada)
*   **Laptop <---> Arduino Nano**: Conexión USB directa física. 
*   **Protocolo**: Hardware Serial estándar (UART) a 9600 baudios.

### B. Conexión de Depuración (Inalámbrica)
*   **Arduino Nano (SoftwareSerial) <---> ESP32 (UART2)**:
    *   Arduino Nano `D9` (TX)  ---> ESP32 `GPIO 16` (RX) [Con resistencia protectora/divisor de tensión]
    *   Arduino Nano `D12` (RX) <--- ESP32 `GPIO 17` (TX) [Opcional]
    *   Arduino Nano `GND`     <---> ESP32 `GND` (Tierra común obligatoria)
*   **Protocolo**: SoftwareSerial a 9600 baudios.

```mermaid
graph TD
    subgraph Laptop [Laptop (Servidor Web & Control)]
        Browser[Navegador Chrome/Edge]
        Browser -- Web Serial API USB -- UART_HW[Hardware Serial 9600]
    end

    subgraph Grua [Maqueta Grúa Torre]
        Nano[Arduino Nano (Controlador Actuadores)]
        UART_HW -- USB -- Nano
        Nano -- SoftwareSerial D9/D12 -- ESP32[ESP32 Developer Monitor]
    end

    subgraph Celular [Celular del Desarrollador]
        CelBrowser[Navegador Celular]
        CelBrowser -- Wi-Fi HTTP Port 80 -- ESP32
    end
```

---

## 3. Especificaciones de Software

### A. Arduino Nano (C++ / `grua_arduino.ino`)
*   Se incluye `<SoftwareSerial.h>`.
*   Se declara `SoftwareSerial debugSerial(12, 9);` (RX=12, TX=9).
*   En `setup()`, se añade `debugSerial.begin(9600);`.
*   Toda información de diagnóstico útil para depurar (ej. comando recibido, estado interno del motor, modo actual) se transmite mediante `debugSerial.println(...)`.
*   El canal serie principal `Serial` se mantiene reservado exclusivamente para la transmisión del JSON de telemetría (cada 200 ms) y la recepción de caracteres de comandos desde la Web Serial API.

### B. ESP32 (MicroPython / `main.py`)
*   Se configura el puerto de escucha `UART(2, baudrate=9600, rx=16, tx=17)`.
*   Se mantiene una lista circular `log_buffer` de tamaño fijo (máximo 50 líneas) en la memoria RAM del ESP32.
*   Se levanta un servidor socket básico en el puerto `80`.
*   Al recibir una petición HTTP en `/`, se responde con una cabecera de texto plano o HTML básico:
    ```html
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="utf-8">
      <meta http-equiv="refresh" content="2">
      <title>Debug Monitor</title>
      <style>
        body { font-family: monospace; background: #0c0c0c; color: #00ff66; padding: 20px; font-size: 14px; }
        h2 { border-bottom: 1px solid #333; padding-bottom: 5px; color: #fff; }
      </style>
    </head>
    <body>
      <h2>📡 Monitor de Depuración Inalámbrico (Grúa)</h2>
      <pre>[Últimos logs recibidos del Arduino Nano]</pre>
      <pre>LOG_DATA_HERE</pre>
    </body>
    </html>
    ```

### C. Interfaz Web de Control (`index.html` / `schema.html`)
*   **Botón de Conexión**: Añadido en la cabecera. Inicia el diálogo de conexión USB nativo mediante `navigator.serial.requestPort()`.
*   **Lectura de Puerto**: Mediante un bucle asíncrono leyendo de `port.readable` y decodificando el stream en líneas.
*   **Escritura de Puerto**: Al interactuar con la interfaz (teclas, botones, toggles de modo), se envía el caracter correspondiente de forma asíncrona codificado con `TextEncoder` hacia `port.writable`.

---

## 4. Plan de Verificación

### Pruebas de Conexión USB (Laptop):
1.  Abrir la interfaz local en Chrome/Edge.
2.  Hacer clic en "Conectar Grúa", seleccionar el Arduino Nano en el diálogo.
3.  Verificar que el estado cambia a "Conectado".
4.  Comprobar que se reciben los datos JSON de telemetría y se actualiza la interfaz.
5.  Enviar comandos de movimiento y verificar que los motores responden.

### Pruebas de Depuración Inalámbrica (ESP32):
1.  Conectar el celular al punto de acceso del ESP32 (`ESP32-Grua-Setup`).
2.  Acceder en el navegador a `http://192.168.4.1`.
3.  Verificar que se muestra la interfaz del monitor de depuración retro.
4.  Accionar la grúa desde la laptop y observar que el celular refleja inmediatamente los logs de eventos del Arduino.
