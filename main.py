"""
Módulo principal (main.py) del Servidor Web asíncrono para ESP32.
Gestiona las peticiones HTTP de la interfaz web, traduce las acciones
del usuario en comandos UART y los envía al Arduino Nano.
Sirve también el archivo HTML para la interfaz de control remoto.
"""
# main.py
import uasyncio as asyncio
from machine import UART, Pin

# 1. Configuración de Hardware
# LED de Estado en GPIO 2 (Enciende cuando procesa una petición web)
led = Pin(2, Pin.OUT)

# UART 2 para comunicación bidireccional con Arduino Nano
# RX = 16, TX = 17 (Conectado a TX D1 y RX D0 del Nano respectivamente) a 9600 baudios
uart = UART(2, baudrate=9600, tx=17, rx=16)

# Variable global para almacenar la última telemetría recibida de Arduino
# Se inicializa con una plantilla por defecto por si el Arduino aún no envía datos
latest_telemetry = '{"mode":"INACTIVO","cx":512,"cy":512,"cz":512,"pCarro":0.0,"pElev":0.0,"pGiro":0.0,"mCarro":"DETENIDO","mElev":"DETENIDO","mGiro":"DETENIDO"}'

# 2. Función para renderizar la Interfaz Web (HTML)
def render_html():
    """Lee y devuelve el archivo index.html."""
    try:
        with open("index.html", "r") as f:
            return f.read()
    except Exception as e:
        return "<h1>Error: No se encontro index.html</h1>"

# 3. Tarea Asíncrona: Lectura continua de telemetría desde UART
async def read_telemetry():
    global latest_telemetry
    print("Iniciando tarea asíncrona de telemetría (UART2)...")
    buffer = ""
    while True:
        try:
            if uart.any():
                # Leer bytes disponibles
                chunk = uart.read(uart.any())
                if chunk:
                    # Decodificar omitiendo caracteres inválidos
                    buffer += chunk.decode('utf-8', 'ignore')
                   
                    # Extraer líneas completas separadas por salto de línea
                    while "\n" in buffer:
                        line, buffer = buffer.split("\n", 1)
                        line = line.strip()
                       
                        # Validación de paquete JSON básico
                        if line.startswith("{") and line.endswith("}"):
                            latest_telemetry = line
        except Exception as e:
            # Ignorar fallos de decodificación o tramas incompletas
            pass
        # Pequeña pausa no bloqueante de 50ms para ceder control del CPU
        await asyncio.sleep_ms(50)

# 4. Corrutina Asíncrona: Manejador de Peticiones Web
async def handle_client(reader, writer):
    try:
        led.value(1) # Encender LED de estado
       
        # Leer petición HTTP del navegador
        request_bytes = await reader.read(1024)
        request = request_bytes.decode('utf-8')
       
        # Mapeo de rutas (Endpoints) a comandos UART
        comando = None
        if "GET /forward " in request: comando = 'F'
        elif "GET /backward " in request: comando = 'B'
        elif "GET /up " in request: comando = 'U'
        elif "GET /down " in request: comando = 'D'
        elif "GET /left " in request: comando = 'L'
        elif "GET /right " in request: comando = 'R'
        elif "GET /stop " in request: comando = 'S'
        # Nuevo comando para alternar el modo de control entre Web y Joystick
        elif "GET /toggle-mode " in request: comando = 'M'
       
        # Lógica de respuesta del servidor
        if comando:
            # Enviar el carácter por el puerto serial al Arduino Nano
            uart.write(comando)
            # Para peticiones de control (Fetch API), respondemos rápido y sin HTML
            response = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nConnection: close\r\n\r\nOK"
           
        elif "GET /telemetry " in request:
            # Devolver los datos de telemetría en formato JSON con cabecera CORS
            # Esta ruta es consultada periódicamente por la interfaz web para actualizar el estado
            response = f"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nConnection: close\r\n\r\n{latest_telemetry}"
           
        elif "GET / " in request:
            # Si el usuario entra a la ruta principal, le entregamos la página web
            body = render_html()
            response = f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n{body}"
           
        else:
            # Ruta no encontrada
            response = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\nNot Found"

        # Enviar la respuesta por Wi-Fi
        writer.write(response)
        await writer.drain()
       
    except Exception as e:
        print("Error en el cliente:", e)
    finally:
        led.value(0) # Apagar LED
        writer.close()
        await writer.wait_closed()

# 5. Bucle Principal del Servidor
async def main():
    print("Iniciando Servidor Web en el puerto 80...")
   
    # Iniciar la corrutina de lectura de telemetría en segundo plano
    asyncio.create_task(read_telemetry())
   
    # Iniciar el servidor web asíncrono
    server = await asyncio.start_server(handle_client, "0.0.0.0", 80)
   
    # Mantener el bucle vivo
    while True:
        await asyncio.sleep(1)

# Ejecutar el servidor al final de main.py de manera segura
try:
    asyncio.run(main())
except KeyboardInterrupt:
    print("\nServidor web detenido desde el teclado.")