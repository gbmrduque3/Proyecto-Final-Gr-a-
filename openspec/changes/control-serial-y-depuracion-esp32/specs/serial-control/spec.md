## ADDED Requirements

### Requirement: Conexión Serial Directa desde el Navegador
El sistema SHALL permitir al usuario establecer una conexión serie directa de 9600 baudios desde el navegador de su laptop con el Arduino Nano a través de USB, sin depender de un servidor intermedio.

#### Scenario: Conexión USB exitosa
- **WHEN** El usuario hace clic en el botón "Conectar Grúa" en la interfaz web y selecciona el puerto correspondiente
- **THEN** El estado de la interfaz cambia a "Conectado" y se inicia la recepción de la telemetría

### Requirement: Control de Actuadores por Web Serial
El sistema SHALL transmitir comandos de caracteres a través de la conexión Web Serial cuando el usuario accione controles en la interfaz.

#### Scenario: Envío de comando de dirección
- **WHEN** El usuario pulsa una dirección en el panel o el teclado (ej. Adelante)
- **THEN** El navegador codifica y envía el caracter correspondiente (ej. 'F') al puerto serie del Arduino

### Requirement: Procesamiento de Telemetría JSON
El sistema SHALL leer e interpretar de forma asíncrona los flujos de datos en formato JSON enviados por el Arduino Nano.

#### Scenario: Actualización de telemetría en pantalla
- **WHEN** El navegador recibe una trama JSON válida con datos del estado de la grúa (ej. modo, distancias, ángulo)
- **THEN** Los elementos visuales de la interfaz se actualizan con los valores correspondientes sin refrescar la página entera
