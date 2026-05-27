"""
main.py del Monitor de Depuración Inalámbrico para ESP32.
Escucha las trazas del puerto SoftwareSerial del Arduino Nano
y las expone en una interfaz web ultra-sencilla y liviana.
"""
import uasyncio as asyncio
from machine import UART, Pin

# LED indicador de actividad
led = Pin(2, Pin.OUT)

# Configuración del puerto UART 2 para depuración
# RX = 16, TX = 17 conectado al SoftwareSerial del Arduino Nano D9(TX)/D12(RX)
uart = UART(2, baudrate=9600, rx=16, tx=17)

# Buffer circular en memoria para almacenar las últimas 50 líneas de log
LOG_LIMIT = 50
log_buffer = []

def add_log(line):
    """Agrega una línea al buffer circular."""
    global log_buffer
    log_buffer.append(line)
    if len(log_buffer) > LOG_LIMIT:
        log_buffer.pop(0)

# Tarea Asíncrona: Lectura del flujo serie de depuración
async def read_debug_logs():
    print("Iniciando escucha del canal de depuración (UART2)...")
    buffer = ""
    while True:
        try:
            if uart.any():
                led.value(1) # Encender led de actividad al recibir
                chunk = uart.read(uart.any())
                if chunk:
                    buffer += chunk.decode('utf-8', 'ignore')
                    while "\n" in buffer:
                        line, buffer = buffer.split("\n", 1)
                        line = line.strip("\r\n ")
                        if line:
                            print(f"[DEBUG] {line}")
                            add_log(line)
                led.value(0)
        except Exception as e:
            pass
        await asyncio.sleep_ms(20)

# Corrutina Asíncrona: Manejador Web del servidor
async def handle_client(reader, writer):
    try:
        # Leer cabecera de la petición (la ignoramos para optimizar)
        await reader.read(512)
        
        # Construir la lista de logs como texto
        logs_html = ""
        for log in reversed(log_buffer):
            logs_html += f"{log}\n"
            
        if not logs_html:
            logs_html = "Esperando mensajes del Arduino Nano...\n"

        # Plantilla HTML minimalista con refresco automático
        html_response = f"""<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta http-equiv="refresh" content="2">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Monitor de Grúa</title>
  <style>
    body {{
      font-family: 'Courier New', Courier, monospace;
      background-color: #0d0e11;
      color: #00ff66;
      margin: 0;
      padding: 15px;
      font-size: 14px;
    }}
    .container {{
      max-width: 900px;
      margin: 0 auto;
    }}
    h1 {{
      font-size: 18px;
      border-bottom: 2px solid #00ff66;
      padding-bottom: 10px;
      color: #ffffff;
      margin-top: 0;
    }}
    pre {{
      background-color: #16181d;
      border: 1px solid #2a2d37;
      padding: 12px;
      border-radius: 6px;
      overflow-x: auto;
      white-space: pre-wrap;
      word-wrap: break-word;
      line-height: 1.5;
    }}
    .status {{
      font-size: 11px;
      color: #888888;
      margin-top: 15px;
      text-align: right;
    }}
  </style>
</head>
<body>
  <div class="container">
    <h1>📡 Monitor de Depuración Inalámbrico (ESP32)</h1>
    <pre>{logs_html}</pre>
    <div class="status">Última actualización. Auto-refresco cada 2s. Buffer: {len(log_buffer)}/{LOG_LIMIT} líneas.</div>
  </div>
</body>
</html>
"""
        response = f"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n{html_response}"
        writer.write(response)
        await writer.drain()
    except Exception as e:
        print("Error atendiendo cliente:", e)
    finally:
        writer.close()
        await writer.wait_closed()

async def main():
    print("Iniciando Servidor Web de Depuración en el puerto 80...")
    # Tarea de lectura serie
    asyncio.create_task(read_debug_logs())
    # Servidor asíncrono
    server = await asyncio.start_server(handle_client, "0.0.0.0", 80)
    
    # Mantener el bucle vivo
    while True:
        await asyncio.sleep(1)

try:
    asyncio.run(main())
except KeyboardInterrupt:
    print("\nServidor de depuración detenido.")
