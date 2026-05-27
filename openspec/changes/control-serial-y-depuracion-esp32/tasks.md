## 1. Firmware de Arduino Nano (SoftwareSerial y Depuración)

- [x] 1.1 Incluir `SoftwareSerial.h` e instanciar el puerto secundario `debugSerial(12, 9)` (RX=12, TX=9) en `grua_arduino.ino`.
- [x] 1.2 Inicializar `debugSerial.begin(9600)` en el `setup()` del Arduino.
- [x] 1.3 Configurar las trazas y mensajes informativos/diagnósticos para transmitirse por `debugSerial.println()` en lugar del canal serie principal USB.

## 2. Firmware de ESP32 (Consola Inalámbrica en MicroPython)

- [x] 2.1 Configurar `UART(2, baudrate=9600, rx=16, tx=17)` en `main.py` para escuchar los datos del canal de depuración del Arduino.
- [x] 2.2 Implementar la estructura de buffer circular (limite de 50 líneas) en memoria para acumular las trazas entrantes.
- [x] 2.3 Programar un servidor HTTP nativo por sockets en el puerto 80 que exponga los logs en una interfaz HTML minimalista con refresco automático cada 2 segundos.

## 3. Interfaz Web (Web Serial API)

- [x] 3.1 Diseñar e insertar un botón de "Conectar Grúa" y un indicador de estado de puerto USB en `index.html`.
- [x] 3.2 Escribir la lógica en JavaScript para solicitar el puerto serie (`requestPort`), abrirlo a 9600 baudios y leer asíncronamente el stream de JSON de telemetría.
- [x] 3.3 Adaptar el envío de comandos de dirección y conmutación de modo para que se escriban como caracteres directamente en `port.writable`.

## 4. Documentación y Guías de Cableado

- [x] 4.1 Actualizar el diagrama SVG y la tabla de pines en `schema.html` para ilustrar las conexiones cruzadas de SoftwareSerial entre Arduino (D9/D12) y ESP32 (GPIO 16/17), además de la conexión USB a la laptop.

## 5. Organización de Archivos y Documentación General

- [x] 5.1 Crear las carpetas `arduino/`, `esp32/` y `web_server/` para clasificar y organizar cada componente del sistema.
- [x] 5.2 Crear el archivo `prompt.md` con el prompt reutilizable para replicar estas funcionalidades en otros proyectos de grúas o controladores similares.
- [x] 5.3 Actualizar el archivo `README.md` del proyecto con la nueva arquitectura Web Serial + ESP32 Debug Monitor.
