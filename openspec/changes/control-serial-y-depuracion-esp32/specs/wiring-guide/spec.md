## ADDED Requirements

### Requirement: Registro de Conexión de Depuración y Control USB
El diagrama y la tabla de conexiones en `schema.html` SHALL reflejar e ilustrar los nuevos canales físicos para la depuración inalámbrica secundaria y el control USB.

#### Scenario: Visualización de pines de depuración secundaria
- **WHEN** El usuario inspecciona el cableado y los pines en `schema.html`
- **THEN** El diagrama ilustra claramente la conexión serial cruzada entre los pines digitales D9/D12 de Arduino Nano y los pines GPIO 16/17 del ESP32 Gateway para el canal de diagnóstico, así como la conexión del cable USB de control principal.
