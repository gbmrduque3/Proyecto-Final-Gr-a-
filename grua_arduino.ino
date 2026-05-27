/*
 * grua_arduino.ino
 * Controlador principal de la Grúa Torre basado en Arduino Nano (v4).
 *
 * Movimientos y Actuadores:
 * 1. Carro horizontal (Trolley): Motor DC en Puente H 1 (Canal A).
 * 2. Giro de Cabina (Slew): Motorreductor 30 RPM en Puente H 1 (Canal B).
 * 3. Elevación (Hoist): Motor 300 RPM en Puente H 2 (Canal A).
 *
 * Controles:
 * 1. Físico: Joysticks analógicos en A0 (Giro), A1 (Elevación) y A2 (Carro).
 * 2. Remoto: Peticiones web a través del ESP32 por Serial (UART).
 * 3. Selector de Modo: Pulsador en pin A3 (toggles controlWebActivo).
 *
 * Lógica de Prioridad y Sincronización:
 * - El modo de control es exclusivo (Web o Joystick).
 * - Se alterna mediante pulsador (Pin A3) o el comando serial 'M'.
 */
// grua_arduino.ino

// ==== PINES DEL JOYSTICK (Entradas Analógicas y Digital) ====
const int JOY_GIRO = A0;        // Joystick para Rotación (Izquierda/Derecha)
const int JOY_ELEVACION = A1;   // Joystick para Subir/Bajar
const int JOY_CARRO = A2;       // Joystick para movimiento del Carro
const int PIN_PULSADOR = A3;    // Pulsador físico del joystick para cambio de modo (INPUT_PULLUP)

// ==== PINES DRIVER TB6612FNG #1 (Motores de Carro y Giro) ====
// Motor Carro (Trolley)
const int AIN1_1 = 2;
const int AIN2_1 = 4;
const int PWMA_1 = 3;
// Motor Giro (Slew - 30 RPM)
const int BIN1_1 = 7;
const int BIN2_1 = 8;
const int PWMB_1 = 5;

// ==== PINES DRIVER TB6612FNG #2 (Motor de Elevación) ====
// Motor Elevación (300 RPM)
const int AIN1_2 = 10;
const int AIN2_2 = 11;
const int PWMA_2 = 6;

// ==== CONFIGURACIÓN DE VELOCIDADES MÁXIMAS (0 a 255) ====
const int VEL_MAX_CARRO = 255;  // Velocidad máxima del carro
const int VEL_MAX_GIRO = 200;   // Velocidad máxima de giro (30 RPM)
const int VEL_MAX_ELEV = 255;   // Velocidad máxima elevación (300 RPM)

// ==== VARIABLES DE ESTADO Y SEGURIDAD ====
bool controlWebActivo = true;   // Por defecto se inicia permitiendo control web
char comandoWeb = 'S'; 
unsigned long ultimoComandoWeb = 0;
const unsigned long TIMEOUT_WEB = 500; // Parada si no hay comando web en 500ms

// Zona muerta para joysticks (0-1023, centro ~512)
const int ZONA_MUERTA_MIN = 400;
const int ZONA_MUERTA_MAX = 600;

// Debounce del pulsador físico
unsigned long ultimoTiempoRebote = 0;
const unsigned long RETARDO_REBOTE = 50; // 50 ms
bool ultimoEstadoBoton = HIGH;
bool estadoBoton = HIGH;

// Variables internas para registrar la velocidad actual de movimiento (para telemetría)
int velocidadActualCarro = 0;
int velocidadActualGiro = 0;
int velocidadActualElev = 0;

// ==== VARIABLES DE TELEMETRÍA (FÍSICA VIRTUAL) ====
float posCarro = 0.0;          // Posición estimada del carro (0 a 30.0 cm)
float posElev = 0.0;           // Altura estimada del gancho (0 a 50.0 cm)
float posGiro = 0.0;           // Ángulo estimado de la cabina (0 a 360.0°)

const float CARRO_LIMITE_MAX = 30.0;
const float CARRO_VELOCIDAD_CMS = 5.0; // Velocidad estimada del carro (5 cm/s)
const float ELEV_LIMITE_MAX = 50.0;
const float ELEV_VELOCIDAD_CMS = 8.0;  // Velocidad de elevación (8 cm/s a máxima potencia)
const float GIRO_VELOCIDAD_DEGS = 180.0; // Giro teórica de 30 RPM (180 grados/segundo a máxima potencia)

unsigned long ultimaActualizacionFisica = 0;
unsigned long ultimoReporteTelemetria = 0;
const unsigned long INTERVALO_TELEMETRIA = 200; // Enviar telemetría cada 200 ms

void setup() {
  // Serial a 9600 bps para comunicar con el ESP32
  Serial.begin(9600);

  // Configuración de pines de salida Puente H 1 (Carro y Giro)
  pinMode(AIN1_1, OUTPUT);
  pinMode(AIN2_1, OUTPUT);
  pinMode(PWMA_1, OUTPUT);
  pinMode(BIN1_1, OUTPUT);
  pinMode(BIN2_1, OUTPUT);
  pinMode(PWMB_1, OUTPUT);

  // Configuración de pines de salida Puente H 2 (Elevación)
  pinMode(AIN1_2, OUTPUT);
  pinMode(AIN2_2, OUTPUT);
  pinMode(PWMA_2, OUTPUT);

  // Configurar pin del pulsador
  pinMode(PIN_PULSADOR, INPUT_PULLUP);

  // Detener todos los motores
  detenerCarro();
  detenerGiro();
  detenerElevacion();

  ultimaActualizacionFisica = millis();
}

// ==== FUNCIONES DE MOVIMIENTO - CARRO ====
void moverCarro(bool adelante, int velocidad) {
  digitalWrite(AIN1_1, adelante ? HIGH : LOW);
  digitalWrite(AIN2_1, adelante ? LOW : HIGH);
  analogWrite(PWMA_1, velocidad);
  velocidadActualCarro = velocidad;
}

void detenerCarro() {
  digitalWrite(AIN1_1, LOW);
  digitalWrite(AIN2_1, LOW);
  analogWrite(PWMA_1, 0);
  velocidadActualCarro = 0;
}

// ==== FUNCIONES DE MOVIMIENTO - GIRO ====
void moverGiro(bool derecha, int velocidad) {
  digitalWrite(BIN1_1, derecha ? HIGH : LOW);
  digitalWrite(BIN2_1, derecha ? LOW : HIGH);
  analogWrite(PWMB_1, velocidad);
  velocidadActualGiro = velocidad;
}

void detenerGiro() {
  digitalWrite(BIN1_1, LOW);
  digitalWrite(BIN2_1, LOW);
  analogWrite(PWMB_1, 0);
  velocidadActualGiro = 0;
}

// ==== FUNCIONES DE MOVIMIENTO - ELEVACIÓN ====
void moverElevacion(bool subir, int velocidad) {
  digitalWrite(AIN1_2, subir ? HIGH : LOW);
  digitalWrite(AIN2_2, subir ? LOW : HIGH);
  analogWrite(PWMA_2, velocidad);
  velocidadActualElev = velocidad;
}

void detenerElevacion() {
  digitalWrite(AIN1_2, LOW);
  digitalWrite(AIN2_2, LOW);
  analogWrite(PWMA_2, 0);
  velocidadActualElev = 0;
}

void loop() {
  // 1. LECTURA DE COMANDOS UART (DESDE ESP32)
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'F' || c == 'B' || c == 'U' || c == 'D' || c == 'L' || c == 'R' || c == 'S') {
      comandoWeb = c;
      ultimoComandoWeb = millis(); // Reiniciar timeout
    } 
    else if (c == 'M') {
      // Comando 'M': alterna entre modo Web y modo Joystick en el firmware
      // Este cambio de modo también se refleja en la telemetría enviada al ESP32.
      controlWebActivo = !controlWebActivo;
    }
  }

  // 2. TIMEOUT DE COMANDOS WEB
  if (millis() - ultimoComandoWeb > TIMEOUT_WEB) {
    comandoWeb = 'S'; // Detener si se pierde el latido
  }

  // 3. DEBOUNCE DEL PULSADOR FÍSICO (PIN A3)
  bool lectura = digitalRead(PIN_PULSADOR);
  if (lectura != ultimoEstadoBoton) {
    ultimoTiempoRebote = millis();
  }

  if ((millis() - ultimoTiempoRebote) > RETARDO_REBOTE) {
    if (lectura != estadoBoton) {
      estadoBoton = lectura;
      if (estadoBoton == LOW) { // Pulsado (flanco de bajada en PULLUP)
        controlWebActivo = !controlWebActivo; // Alternar modo
      }
    }
  }
  ultimoEstadoBoton = lectura;

  // 4. LÓGICA DE CONTROL EXCLUSIVO
  int valGiro = analogRead(JOY_GIRO);
  int valElev = analogRead(JOY_ELEVACION);
  int valCarro = analogRead(JOY_CARRO);

  if (!controlWebActivo) {
    // === MODO JOYSTICK ACTIVO ===
    
    // Control de Carro
    if (valCarro > ZONA_MUERTA_MAX) {
      int vel = map(valCarro, ZONA_MUERTA_MAX, 1023, 0, VEL_MAX_CARRO);
      moverCarro(true, vel); // Adelante
    } 
    else if (valCarro < ZONA_MUERTA_MIN) {
      int vel = map(valCarro, ZONA_MUERTA_MIN, 0, 0, VEL_MAX_CARRO);
      moverCarro(false, vel); // Atrás
    } 
    else {
      detenerCarro();
    }

    // Control de Giro
    if (valGiro > ZONA_MUERTA_MAX) {
      int vel = map(valGiro, ZONA_MUERTA_MAX, 1023, 0, VEL_MAX_GIRO);
      moverGiro(true, vel); // Giro Derecha
    } 
    else if (valGiro < ZONA_MUERTA_MIN) {
      int vel = map(valGiro, ZONA_MUERTA_MIN, 0, 0, VEL_MAX_GIRO);
      moverGiro(false, vel); // Giro Izquierda
    } 
    else {
      detenerGiro();
    }

    // Control de Elevación
    if (valElev > ZONA_MUERTA_MAX) {
      int vel = map(valElev, ZONA_MUERTA_MAX, 1023, 0, VEL_MAX_ELEV);
      moverElevacion(true, vel); // Subir
    } 
    else if (valElev < ZONA_MUERTA_MIN) {
      int vel = map(valElev, ZONA_MUERTA_MIN, 0, 0, VEL_MAX_ELEV);
      moverElevacion(false, vel); // Bajar
    } 
    else {
      detenerElevacion();
    }

  } 
  else {
    // === MODO WEB ACTIVO ===
    
    // Control de Carro Web
    if (comandoWeb == 'F') {
      moverCarro(true, VEL_MAX_CARRO);
    } 
    else if (comandoWeb == 'B') {
      moverCarro(false, VEL_MAX_CARRO);
    } 
    else if (comandoWeb == 'S') {
      detenerCarro();
    }

    // Control de Giro Web
    if (comandoWeb == 'R') {
      moverGiro(true, VEL_MAX_GIRO);
    } 
    else if (comandoWeb == 'L') {
      moverGiro(false, VEL_MAX_GIRO);
    } 
    else if (comandoWeb == 'S') {
      detenerGiro();
    }

    // Control de Elevación Web
    if (comandoWeb == 'U') {
      moverElevacion(true, VEL_MAX_ELEV);
    } 
    else if (comandoWeb == 'D') {
      moverElevacion(false, VEL_MAX_ELEV);
    } 
    else if (comandoWeb == 'S') {
      detenerElevacion();
    }
  }

  // 5. CÓMPUTO DE TELEMETRÍA (FÍSICA VIRTUAL)
  unsigned long ahora = millis();
  float dt = (ahora - ultimaActualizacionFisica) / 1000.0;
  if (dt < 0.0) dt = 0.0; // Desbordamiento
  ultimaActualizacionFisica = ahora;

  // Analizar estado y actualizar posición del carro (Trolley)
  String estadoCarro = "DETENIDO";
  if (velocidadActualCarro > 0) {
    float factorVelocidad = (float)velocidadActualCarro / 255.0;
    if (digitalRead(AIN1_1) == HIGH && digitalRead(AIN2_1) == LOW) {
      posCarro = posCarro + (CARRO_VELOCIDAD_CMS * factorVelocidad * dt);
      if (posCarro > CARRO_LIMITE_MAX) posCarro = CARRO_LIMITE_MAX;
      estadoCarro = "ADELANTE";
    } 
    else if (digitalRead(AIN1_1) == LOW && digitalRead(AIN2_1) == HIGH) {
      posCarro = posCarro - (CARRO_VELOCIDAD_CMS * factorVelocidad * dt);
      if (posCarro < 0.0) posCarro = 0.0;
      estadoCarro = "ATRÁS";
    }
  }

  // Analizar estado y actualizar posición de giro
  String estadoGiro = "DETENIDO";
  if (velocidadActualGiro > 0) {
    float factorVelocidad = (float)velocidadActualGiro / 255.0;
    if (digitalRead(BIN1_1) == HIGH && digitalRead(BIN2_1) == LOW) {
      posGiro = posGiro + (GIRO_VELOCIDAD_DEGS * factorVelocidad * dt);
      estadoGiro = "DERECHA";
    } 
    else if (digitalRead(BIN1_1) == LOW && digitalRead(BIN2_1) == HIGH) {
      posGiro = posGiro - (GIRO_VELOCIDAD_DEGS * factorVelocidad * dt);
      estadoGiro = "IZQUIERDA";
    }
  }

  // Normalizar ángulo giro a [0.0, 360.0]
  if (posGiro >= 360.0) posGiro -= 360.0;
  if (posGiro < 0.0) posGiro += 360.0;

  // Analizar estado y actualizar posición de elevación
  String estadoElev = "DETENIDO";
  if (velocidadActualElev > 0) {
    float factorVelocidad = (float)velocidadActualElev / 255.0;
    if (digitalRead(AIN1_2) == HIGH && digitalRead(AIN2_2) == LOW) {
      posElev = posElev + (ELEV_VELOCIDAD_CMS * factorVelocidad * dt);
      if (posElev > ELEV_LIMITE_MAX) posElev = ELEV_LIMITE_MAX;
      estadoElev = "SUBIENDO";
    } 
    else if (digitalRead(AIN1_2) == LOW && digitalRead(AIN2_2) == HIGH) {
      posElev = posElev - (ELEV_VELOCIDAD_CMS * factorVelocidad * dt);
      if (posElev < 0.0) posElev = 0.0;
      estadoElev = "BAJANDO";
    }
  }

  // Determinar modo de control para telemetría
  String modoTelemetria = controlWebActivo ? "WEB" : "JOYSTICK";

  // 6. ENVIAR REPORTE SERIAL CADA 200MS
  // El firmware ahora envía un JSON de telemetría ampliado que incluye:
  // - mode: modo activo (WEB o JOYSTICK)
  // - cx, cy, cz: valores crudos de los joysticks de Carro, Elevación y Giro
  // - sw: estado del pulsador físico (0 = pulsado, 1 = liberado)
  // - pCarro, pElev, pGiro: posiciones estimadas para animar la interfaz web
  // - mCarro, mElev, mGiro: estados de movimiento simplificados para la UI
  if (ahora - ultimoReporteTelemetria >= INTERVALO_TELEMETRIA) {
    ultimoReporteTelemetria = ahora;

    // Crear JSON manual con valores dinámicos
    Serial.print("{\"mode\":\""); Serial.print(modoTelemetria);
    Serial.print("\",\"cx\":"); Serial.print(valCarro);
    Serial.print(",\"cy\":"); Serial.print(valElev);
    Serial.print(",\"cz\":"); Serial.print(valGiro);
    Serial.print(",\"sw\":"); Serial.print(digitalRead(PIN_PULSADOR) == LOW ? 0 : 1);
    Serial.print(",\"pCarro\":"); Serial.print(posCarro, 1);
    Serial.print(",\"pElev\":"); Serial.print(posElev, 1);
    Serial.print(",\"pGiro\":"); Serial.print(posGiro, 1);
    Serial.print(",\"mCarro\":\""); Serial.print(estadoCarro);
    Serial.print("\",\"mElev\":\""); Serial.print(estadoElev);
    Serial.print("\",\"mGiro\":\""); Serial.print(estadoGiro);
    Serial.println("\"}");
  }
}
