"""
Módulo de arranque (boot.py) para ESP32.
Este archivo se ejecuta automáticamente al encender o reiniciar el microcontrolador.
Su propósito principal es establecer la conexión a la red Wi-Fi local para
permitir el acceso al servidor web que controla la grúa.
Incluye un menú interactivo en terminal para detener el arranque y permitir
la programación/modificación de archivos (REPL libre).
"""
# boot.py
import network
import time
import sys
import uselect

# --- CONFIGURACIÓN DE WI-FI ---
# Los estudiantes pueden cambiar estas credenciales por las de su propia red o del colegio.
SSID = "Claro_JC"  # Cambia esto por el nombre de tu red Wi-Fi
PASSWORD = "1713283321!"

def connect_wifi(ssid=SSID, password=PASSWORD):
    """
    Función para conectar el ESP32 a la red Wi-Fi.
    """
    # Configuramos el ESP32 en modo "Station" (Cliente Wi-Fi)
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
   
    # Si no estamos conectados, iniciamos el proceso de conexión
    if not wlan.isconnected():
        print('Conectando a la red:', ssid)

        # Intentos de conexión con manejo de errores para diagnóstico
        max_attempts = 5
        connected = False
        for attempt in range(1, max_attempts + 1):
            try:
                wlan.connect(ssid, password)
            except OSError as e:
                print('OSError en wlan.connect():', e)
                # Intentaremos recopilar información del adaptador
                try:
                    print('Estado WLAN (wlan.status):', wlan.status())
                except Exception:
                    pass
            # Esperar un poco y comprobar si se conectó
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
                print('\nIntento', attempt, 'fallido. Reintentando...')

        if not connected:
            print('\nNo se pudo conectar en modo STA tras', max_attempts, 'intentos.')
            # Mostrar redes detectadas para ayudar al diagnóstico
            try:
                nets = wlan.scan()
                print('Redes detectadas (SSID, RSSI, Authmode):')
                for n in nets:
                    ss = n[0].decode('utf-8', 'ignore') if isinstance(n[0], (bytes, bytearray)) else str(n[0])
                    rssi = n[3]
                    auth = n[4]
                    print('-', ss, rssi, auth)
            except Exception as e:
                print('No se pudo realizar scan():', e)

            # Como fallback y para facilitar configuración, habilitamos un AP local
            try:
                ap = network.WLAN(network.AP_IF)
                ap.active(True)
                ap.config(essid='ESP32-Grua-Setup')
                print('Modo AP activo. Conéctate a la red "ESP32-Grua-Setup" y abre http://192.168.4.1')
            except Exception as e:
                print('Error al iniciar modo AP:', e)
            return

    # Cuando nos conectamos, imprimimos la IP para poder ingresar desde el navegador web
    print('\n¡Conexión exitosa al Wi-Fi!')
    print('Ingresa esta Dirección IP en tu navegador para ver los controles:', wlan.ifconfig()[0])

def menu_inicio(timeout_segundos=5):
    """
    Muestra un menú en la terminal. Avanza automáticamente a ejecución si no hay respuesta.
    Permite detener el inicio automático para liberar la consola REPL.
    """
    print("\n" + "="*40)
    print("      SISTEMA DE CONTROL - GRÚA TORRE")
    print("="*40)
    print("1. Iniciar sistema normalmente (Modo Ejecución)")
    print("2. Detener en modo programación (Liberar REPL)")
    print(f"Selecciona una opción (Avanza a opción 1 en {timeout_segundos}s)...")
   
    # Configurar la terminal para escuchar la entrada del usuario sin bloquear
    poller = uselect.poll()
    poller.register(sys.stdin, uselect.POLLIN)
   
    tiempo_inicio = time.time()
    while (time.time() - tiempo_inicio) < timeout_segundos:
        # Revisar si hay datos en la terminal (espera hasta 100ms por ciclo)
        if poller.poll(100):
            caracter = sys.stdin.read(1)
            if caracter == '1':
                print("\n-> Opción 1 seleccionada. Iniciando...")
                return True
            elif caracter == '2':
                print("\n-> Opción 2 seleccionada. Modo programación activo.")
                print("Consola REPL liberada. Puedes subir o modificar archivos.")
                return False
   
    # Si se agota el tiempo sin respuesta, asumimos que está corriendo en la grúa de forma autónoma
    print("\n-> Tiempo de espera agotado. Iniciando de forma automática...")
    return True

# --- FLUJO DE INICIO ---

# Ejecutamos el menú ANTES de conectar al WiFi o cargar el main
if menu_inicio(timeout_segundos=5):
    # Si elige 1 o se agota el tiempo, conecta a WiFi y avanza a main.py
    connect_wifi(SSID, PASSWORD)
else:
    # Si elige 2, forzamos la detención del script
    # Esto evita que MicroPython salte automáticamente a ejecutar el main.py
    sys.exit()