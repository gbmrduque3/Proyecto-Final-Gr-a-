# Proyecto Grúa Torre: Requerimientos Técnicos para Generación de Código (v4)

## Contexto del Proyecto
Este documento detalla los requerimientos técnicos y la asignación de hardware para la maqueta de la grúa torre de control dual (Manual vía Joysticks y Remoto vía Web). Utiliza comunicación serial UART a 9600 baudios entre un ESP32 y un Arduino Nano.

---

## 1. Arquitectura de Hardware y Pines

### Controlador A: Arduino Nano (Actuador Principal)
- **Framework:** Arduino / C++
- **Responsabilidad:** Controlar los motores de carro, giro y elevación, leer entradas de joystick, gestionar la conmutación exclusiva de modo y calcular telemetría virtual.
- **Asignación de Pines:**
  - **Joysticks:**
    - Eje X (Giro/Rotación) -> Pin Analógico A0
    - Eje Y (Elevación) -> Pin Analógico A1
    - Eje X del segundo Joystick (Carro) -> Pin Analógico A2
    - Pulsador (Alternancia de modo) -> Pin Analógico A3 (Configurado como `INPUT_PULLUP` digital)
  - **Puente H 1 (TB6612FNG - Motores de Carro y Giro):**
    - Motor Carro (300 RPM): AIN1(D2), AIN2(D4), PWMA(D3)
    - Motor Giro (30 RPM): BIN1(D7), BIN2(D8), PWMB(D5)
  - **Puente H 2 (TB6612FNG - Motor de Elevación):**
    - Motor Elevación (300 RPM): AIN1(D10), AIN2(D11), PWMA(D6)
  - **Puentes H - Pines Comunes:**
    - Pines STBY de ambos controladores -> Conectados permanentemente a VCC (5V)
  - **Comunicación:**
    - RX (D0) conectado al TX del ESP32.
    - TX (D1) conectado al RX del ESP32.

### Controlador B: ESP32 DevKit V1 (Interfaz Web)
- **Framework:** MicroPython (Thonny IDE)
- **Responsabilidad:** Levantar un servidor web asíncrono, gestionar la conexión Wi-Fi, servir el archivo `index.html` y reenviar comandos / recibir telemetría mediante UART.
- **Asignación de Pines:**
  - **UART 2:** RX (GPIO 16), TX (GPIO 17) (Cruzados con TX/RX del Nano respectivamente)
  - **LED Status:** GPIO 2 (Enciende durante el procesamiento de solicitudes)

---

## 2. Requerimientos de Software

### Tarea 1: Firmware Arduino (grua_arduino.ino)
- **Control Exclusivo Alternado:** Una variable de control (`controlWebActivo`) define la fuente de movimiento.
  - Si es verdadera, solo procesa comandos web y bloquea joysticks.
  - Si es falsa, solo procesa joysticks físicos y bloquea comandos web.
  - Se alterna mediante pulsador (Pin A3 con debounce de 50 ms) o al recibir serial `'M'`.
- **Calibración de Velocidad Máxima:** Constantes independientes en la cabecera (`VEL_MAX_CARRO`, `VEL_MAX_GIRO`, `VEL_MAX_ELEV`) en el rango [0-255].
- **Cómputo de Telemetría:**
  - Estimar la distancia del carro (0.0 a 30.0 cm) basándose en una velocidad de 5.0 cm/s cuando el motor del carro está activo.
  - Estimar la altura de la elevación (0.0 a 50.0 cm) basándose en una velocidad de 8.0 cm/s cuando el motor de elevación está activo.
  - Estimar el ángulo de giro (0.0 a 360.0°) basándose en la velocidad de giro de un motorreductor de 30 RPM (tasa de 180° por segundo).
  - Enviar el estado de telemetría completo (modo, joysticks, posiciones y estados) cada 200 ms.

### Tarea 2: Firmware ESP32 (boot.py y main.py)
- **Conexión Wi-Fi Robustecida:** Menú de inicio de 5 segundos para liberar el REPL de programación. Fallback automático a modo Access Point (`ESP32-Grua-Setup`, IP `192.168.4.1`) si falla la conexión en 5 intentos.
- **Servidor Web y Endpoints:**
  - Endpoint `/toggle-mode`: Envía la señal serial `'M'` al Nano para alternar el modo.
  - Endpoints de movimiento: Envían comandos de movimiento correspondientes (`F`, `B`, `U`, `D`, `L`, `R`, `S`).
  - Endpoint `/telemetry`: Sirve el último JSON de telemetría recibido del Nano.

### Tarea 3: Interfaz Web (index.html)
- Mantener intacto el diseño visual actual.
- Actualizar el indicador del switch de modo de control a partir de la propiedad `"mode"` en el JSON de telemetría recibido.

---

## 3. Consideraciones Técnicas
- **Baudrate:** Configurado en ambos dispositivos a 9600 bps.
- **Seguridad:** Mantener el timeout de seguridad de 500 ms en el Nano para detener los motores si se interrumpe la comunicación web.
