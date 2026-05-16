"""
Módulo de arranque (boot.py) para ESP32.
Este archivo se ejecuta automáticamente al encender o reiniciar el microcontrolador.
Su propósito principal es establecer la conexión a la red Wi-Fi local para 
permitir el acceso al servidor web que controla la grúa.
"""
# boot.py
import network
import time

def connect_wifi():
    """
    Función para conectar el ESP32 a la red Wi-Fi.
    Los estudiantes pueden cambiar el 'ssid' y 'password' por el de su propia red o del colegio.
    """
    ssid = "TU_RED_WIFI"
    password = "TU_PASSWORD"

    # Configuramos el ESP32 en modo "Station" (Cliente Wi-Fi)
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    
    # Si no estamos conectados, iniciamos el proceso de conexión
    if not wlan.isconnected():
        print('Conectando a la red:', ssid)
        wlan.connect(ssid, password)
        
        # Esperamos en un ciclo hasta que el router nos asigne una dirección IP
        while not wlan.isconnected():
            time.sleep(0.5)
            print('.', end='')
            
    # Cuando nos conectamos, imprimimos la IP para poder ingresar desde el navegador web
    print('\n¡Conexión exitosa al Wi-Fi!')
    print('Ingresa esta Dirección IP en tu navegador para ver los controles:', wlan.ifconfig()[0])

# Ejecutamos la función al encender el ESP32
connect_wifi()
