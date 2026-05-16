# Proyecto Final: Grúa Torre de Doble Control

Este proyecto implementa un sistema de control dual para una grúa torre, permitiendo su operación tanto de forma manual (mediante joysticks físicos) como remota (a través de una interfaz web). Está diseñado con fines educativos para estudiantes de secundaria, integrando conceptos de microcontroladores, electrónica y desarrollo web.

## Arquitectura del Sistema

El sistema se compone de dos microcontroladores principales que se comunican a través de un puerto serial (UART) a 9600 baudios:

1.  **ESP32 (Módulo de Comunicaciones):**
    *   Actúa como un servidor web asíncrono utilizando MicroPython.
    *   Se conecta a la red Wi-Fi local.
    *   Sirve la interfaz web (`index.html`) a los dispositivos clientes (teléfonos, tablets o computadoras).
    *   Traduce las peticiones HTTP (botones presionados en la web) en comandos de un solo carácter que envía al Arduino Nano.

2.  **Arduino Nano (Controlador de Actuadores):**
    *   Actúa como el cerebro principal para el movimiento mecánico, programado en C++.
    *   Lee las entradas analógicas de tres joysticks para el control manual.
    *   Controla dos motores DC (Carro y Elevación) utilizando un driver TB6612FNG.
    *   Controla un motor a pasos Nema 17 (Rotación/Giro) utilizando un driver DRV8825 y la librería `AccelStepper` para movimientos suaves.
    *   Implementa una lógica de prioridad donde los controles físicos (joysticks) tienen precedencia sobre los comandos web.

## Estructura de Archivos

*   `boot.py`: Script de inicio de MicroPython en el ESP32. Configura la conexión a la red Wi-Fi.
*   `main.py`: Script principal en el ESP32. Implementa el servidor web asíncrono (`uasyncio`), mapea las rutas HTTP y envía comandos UART.
*   `index.html`: Interfaz web de control remoto. Diseñada para dispositivos móviles, utiliza la *Fetch API* para enviar comandos sin recargar la página e implementa medidas de seguridad (envío repetitivo de comandos).
*   `grua_arduino.ino`: Código fuente en C++ para el Arduino Nano. Gestiona los drivers de motores, las entradas de los joysticks y la comunicación serial.
*   `requirements.md`: Archivo de requerimientos técnicos o dependencias adicionales (no documentado en este alcance).

## Configuración y Puesta en Marcha

### 1. ESP32 (Servidor Web)
1. Carga los archivos `boot.py`, `main.py` y `index.html` en la memoria del ESP32 (puedes usar Thonny IDE).
2. Abre `boot.py` y modifica las variables `ssid` y `password` con los datos de tu red Wi-Fi local.
3. Al encender, el ESP32 imprimirá en consola la dirección IP asignada.

### 2. Arduino Nano (Control)
1. Instala la librería `AccelStepper` en tu Arduino IDE (Herramientas > Administrar Bibliotecas).
2. Compila y carga el archivo `grua_arduino.ino` al Arduino Nano.

### 3. Conexiones Físicas
*   **Comunicación:** Conecta el Pin TX (17) del ESP32 al Pin RX (D0) del Arduino Nano. (Asegúrate de unificar las tierras / GND).
*   **Joysticks:** Conectados a los pines analógicos A0, A1 y A2 del Arduino Nano.
*   **Motores DC (TB6612FNG):** Pines D2, D4 y PWM D3 (Motor A). Pines D7, D8 y PWM D5 (Motor B).
*   **Motor a Pasos (DRV8825):** Pin STEP al D9 y Pin DIR al D10 del Arduino Nano.

## Seguridad

*   **Timeout Web:** El Arduino detendrá automáticamente los motores si no recibe un comando válido del ESP32 en un lapso de 500ms.
*   **Prioridad Manual:** Si se detecta movimiento en los joysticks, el sistema ignorará cualquier comando proveniente de la web.
