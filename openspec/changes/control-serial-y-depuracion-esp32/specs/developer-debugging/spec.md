## ADDED Requirements

### Requirement: Consola de Diagnóstico Inalámbrico
El sistema SHALL permitir el acceso inalámbrico a un registro o buffer de depuración generado por el Arduino Nano y servido por el ESP32.

#### Scenario: Visualización del log de depuración
- **WHEN** El desarrollador se conecta a la red Wi-Fi del ESP32 e ingresa a su IP en un navegador
- **THEN** Se presenta una página HTML con el listado de las últimas trazas de depuración de la grúa

### Requirement: Buffer de Logs Limitado
El sistema en el ESP32 SHALL mantener un buffer de tamaño limitado en memoria para evitar saturar el almacenamiento local.

#### Scenario: Rotación de logs viejos
- **WHEN** El buffer de logs en el ESP32 alcanza el límite de 50 líneas y llega una línea nueva
- **THEN** Se descarta la línea más antigua del buffer para almacenar la nueva línea de depuración

### Requirement: Recarga Automática de Consola
La interfaz de depuración del ESP32 SHALL actualizarse periódicamente de forma automática.

#### Scenario: Refresco periódico
- **WHEN** El desarrollador mantiene abierta la página de logs
- **THEN** La página realiza una recarga automática de contenido cada 2 segundos sin intervención manual
