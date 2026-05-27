# 🏗️ ¡Bienvenidos al Proyecto Grúa Torre de Control Dual!

Este proyecto es una maqueta interactiva de una **Grúa Torre** real. Está diseñado para que estudiantes y entusiastas de la tecnología aprendan cómo dar vida a la ingeniería y la electrónica de forma divertida.

La grúa se puede manejar de dos formas:
1.  **Con palancas manuales (Joysticks):** Moviendo las palancas físicas directamente en la maqueta.
2.  **Desde la Laptop (Control Web USB):** Presionando botones en la interfaz web local conectando la grúa por USB y usando la tecnología nativa **Web Serial API** de tu navegador.

Además, incorpora un **Monitor de Depuración Inalámbrico (ESP32)** que te permite ver en tiempo real desde tu celular qué comandos está recibiendo el Arduino Nano y cómo los está procesando.

---

## 🖥️ Nueva Arquitectura de Control y Conexión

Para ofrecer una conexión 100% estable y evitar saturaciones de memoria en el ESP32, el proyecto se divide en tres componentes:

### 1. Control Local (Laptop a Grúa vía USB)
*   **Interfaz Interactiva (`index.html`):** Se abre localmente en el navegador de la laptop (Chrome o Edge).
*   **Web Serial API:** Te permite conectar la interfaz web directamente al puerto USB del Arduino Nano haciendo clic en el botón `🔌 Conectar USB`.
*   **Control y Telemetría:** Se transmiten a 9600 baudios por el cable USB, garantizando cero retrasos y máxima fiabilidad.

### 2. Monitor Inalámbrico (ESP32)
*   **SoftwareSerial (Arduino Nano <---> ESP32):** El Arduino Nano utiliza un puerto serie secundario en los pines `D12` (RX) y `D9` (TX) para enviar registros de depuración (logs) al ESP32 a 9600 baudios.
*   **Consola de Logs en el Celular:** El ESP32 recibe los logs, los almacena en un buffer circular (últimos 50 logs) y los sirve en una página web ultraliviana.
*   **Acceso**: Conéctate a la red Wi-Fi `ESP32-Grua-Setup` desde tu celular e ingresa a `http://192.168.4.1` para ver la consola de depuración en vivo.

### 3. Programa Principal (Arduino Nano)
*   Lee los joysticks físicos, ejecuta la conmutación de modo (exclusividad), computa la telemetría física virtual y controla los puentes H TB6612FNG de los motores.

---

## 📂 Organización de Archivos en el Proyecto
*   [`/arduino/`](file:///home/chsop/Proyectos/BC3/Proyecto-Final-Gr-a-/arduino/): Contiene el sketch principal `grua_arduino.ino` para el Arduino Nano.
*   [`/esp32/`](file:///home/chsop/Proyectos/BC3/Proyecto-Final-Gr-a-/esp32/): Contiene los firmwares `boot.py` y `main.py` de MicroPython para el módulo inalámbrico de depuración.
*   [`/web_server/`](file:///home/chsop/Proyectos/BC3/Proyecto-Final-Gr-a-/web_server/): Contiene los archivos frontend `index.html` (interfaz con Web Serial) y `schema.html` (guía de conexiones actualizada).
*   [`prompt.md`](file:///home/chsop/Proyectos/BC3/Proyecto-Final-Gr-a-/prompt.md): Prompt estructurado para replicar este mismo sistema en otros proyectos similares.

---

## 🕹️ Manual de Movimientos: ¿Qué puedes hacer?
Con cualquiera de los dos controles (físico o digital), puedes realizar los siguientes movimientos:
1.  **Carro (Trolley - Adelante y Atrás):** Mueve el carrito rojo a lo largo del brazo horizontal de la grúa para acercar o alejar la carga de la torre.
2.  **Gancho (Hoist - Subir y Bajar):** Sube o baja el gancho naranja con el cable para levantar o soltar objetos.
3.  **Giro (Slew - Rotación):** Gira toda la cabina y el brazo de la grúa en redondo hacia la izquierda o hacia la derecha.

---

## 🛡️ Sistemas de Seguridad Integrados (¿Cómo se autoprotege la grúa?)
1.  **Prioridad Manual (El humano siempre manda primero):** Si estás controlando desde la laptop, pero alguien mueve una palanca física en la maqueta, el control manual toma el mando de inmediato e ignora las órdenes del navegador para reaccionar rápido ante cualquier peligro.
2.  **Detector de Desconexión (Parada de Emergencia):** La interfaz envía señales de latido constantemente. Si el Arduino Nano deja de recibir señales de la laptop por más de **medio segundo (500 ms)** estando en modo Web, todos los motores se apagan automáticamente.
