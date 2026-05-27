"""
Módulo de arranque (boot.py) para ESP32.
Este archivo se ejecuta automáticamente al encender o reiniciar el microcontrolador.
Su propósito principal es establecer la conexión a la red Wi-Fi local o activar el modo AP como fallback.
"""
import network
import time
import sys
import uselect

# --- CONFIGURACIÓN DE WI-FI ---
SSID = "TU_RED_WIFI"
PASSWORD = "TU_CLAVE_WIFI"

# --- CONFIGURACIÓN ACCESS POINT FALLBACK ---
AP_SSID = "ESP32-Grua-Setup"
AP_PASSWORD = "" # Sin contraseña (red abierta)

def menu_inicio(timeout_segundos=5):
    """
    Muestra un menú en la terminal. Avanza automáticamente a ejecución si no hay respuesta.
    Permite detener el inicio automático para liberar la consola REPL.
    """
    print("\n" + "="*40)
    print("      SISTEMA DE DEPURACIÓN INALÁMBRICA - GRÚA TORRE")
    print("="*40)
    print("1. Iniciar sistema normalmente (Modo Ejecución)")
    print("2. Detener en modo programación (Liberar REPL)")
    print(f"Selecciona una opción (Avanza a opción 1 en {timeout_segundos}s)...")

    poller = uselect.poll()
    poller.register(sys.stdin, uselect.POLLIN)

    tiempo_inicio = time.time()
    while (time.time() - tiempo_inicio) < timeout_segundos:
        if poller.poll(100):
            caracter = sys.stdin.read(1)
            if caracter == '1':
                print("\n-> Opción 1 seleccionada. Iniciando...")
                return True
            elif caracter == '2':
                print("\n-> Opción 2 seleccionada. Modo programación activo.")
                print("Consola REPL liberada. Puedes subir o modificar archivos.")
                return False

    print("\n-> Tiempo de espera agotado. Iniciando de forma automática...")
    return True

def connect_wifi(ssid=SSID, password=PASSWORD):
    """
    Conecta el ESP32 a la red Wi-Fi local. Si falla, levanta un Access Point.
    """
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    if not wlan.isconnected():
        print('Conectando a la red:', ssid)
        max_attempts = 5
        connected = False

        for attempt in range(1, max_attempts + 1):
            try:
                wlan.connect(ssid, password)
            except OSError as e:
                print('OSError:', e)

            wait_start = time.time()
            while (time.time() - wait_start) < 5:
                if wlan.isconnected():
                    connected = True
                    break
                time.sleep(0.5)
                print('.', end='')

            if connected:
                break
            else:
                print(f'\nIntento {attempt} fallido. Reintentando...')

        if not connected:
            print('\nNo se pudo conectar al Wi-Fi. Iniciando Access Point de respaldo...')
            start_ap()
            return

    print('\n¡Conexión exitosa al Wi-Fi!')
    print('Ingresa esta Dirección IP en tu navegador para ver la depuración:', wlan.ifconfig()[0])

def start_ap():
    """
    Inicializa el ESP32 en modo Access Point.
    """
    ap = network.WLAN(network.AP_IF)
    ap.active(True)
    ap.config(essid=AP_SSID, password=AP_PASSWORD, authmode=network.AUTH_OPEN)
    
    print('Access Point creado con éxito.')
    print('SSID:', AP_SSID)
    print('Dirección IP (Gateway):', ap.ifconfig()[0])

# Menú de inicio de 5 segundos
if menu_inicio(timeout_segundos=5):
    connect_wifi(SSID, PASSWORD)
else:
    sys.exit()
