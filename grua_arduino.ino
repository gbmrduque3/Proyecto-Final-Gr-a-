/*
 * grua_arduino.ino
 * Controlador principal de la Grúa Torre basado en Arduino Nano.
 * 
 * Funciones:
 * 1. Lee comandos vía Serial (UART) enviados desde el ESP32 (interfaz web).
 * 2. Lee entradas analógicas de joysticks físicos para control manual.
 * 3. Controla motores DC (carro y elevación) usando el driver TB6612FNG.
 * 4. Controla un motor a pasos Nema 17 (rotación) usando un driver DRV8825.
 * 5. Implementa lógica de prioridad (joystick sobre web) y timeout de seguridad.
 */
// grua_arduino.ino
#include <AccelStepper.h>

// ==== PINES DEL JOYSTICK (Entradas Analógicas) ====
const int JOY_X_CARRO = A0;   // Controla el movimiento del carro
const int JOY_Y_ELEVACION = A1; // Controla el gancho (subir/bajar)
const int JOY_GIRO = A2;      // Controla la rotación de la grúa

// ==== PINES DRIVER TB6612FNG (Motores DC N20) ====
// Motor A: Carro
const int AIN1 = 2;
const int AIN2 = 4;
const int PWMA = 3;
// Motor B: Elevación
const int BIN1 = 7;
const int BIN2 = 8;
const int PWMB = 5;

// ==== PINES DRIVER DRV8825 (Motor Nema 17 - Giro) ====
const int STEP_PIN = 9;
const int DIR_PIN = 10;

// Instancia del motor a pasos (1 = modo driver)
AccelStepper motorGiro(1, STEP_PIN, DIR_PIN);

// ==== VARIABLES DE ESTADO Y SEGURIDAD ====
char comandoWeb = 'S'; 
unsigned long ultimoComandoWeb = 0;
const unsigned long TIMEOUT_WEB = 500; // Si pasan 500ms sin recibir comando, se detiene

// Zona muerta para los joysticks (0-1023, centro ~512)
const int ZONA_MUERTA_MIN = 400;
const int ZONA_MUERTA_MAX = 600;
const int VELOCIDAD_DC = 255; // Velocidad PWM (0-255)

void setup() {
  // Comunicación Serial con el ESP32
  Serial.begin(9600);

  // Configuración de pines de salida Motores DC
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // Motores DC detenidos al iniciar
  detenerCarro();
  detenerElevacion();

  // Configuración Motor a Pasos (Movimiento suave)
  motorGiro.setMaxSpeed(1000.0);
  motorGiro.setAcceleration(300.0);
}

// === FUNCIONES DE CONTROL: MOTOR DC ===
void moverCarro(bool adelante) {
  digitalWrite(AIN1, adelante ? HIGH : LOW);
  digitalWrite(AIN2, adelante ? LOW : HIGH);
  analogWrite(PWMA, VELOCIDAD_DC);
}

void detenerCarro() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 0);
}

void moverElevacion(bool subir) {
  digitalWrite(BIN1, subir ? HIGH : LOW);
  digitalWrite(BIN2, subir ? LOW : HIGH);
  analogWrite(PWMB, VELOCIDAD_DC);
}

void detenerElevacion() {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 0);
}

void loop() {
  // 1. LEER COMANDOS DEL ESP32 (SERVIDOR WEB)
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'F' || c == 'B' || c == 'U' || c == 'D' || c == 'L' || c == 'R' || c == 'S') {
      comandoWeb = c;
      ultimoComandoWeb = millis(); // Reiniciar el contador de timeout de seguridad
    }
  }

  // 2. TIMEOUT DE SEGURIDAD (Si el WiFi falla o el usuario suelta el botón web)
  if (millis() - ultimoComandoWeb > TIMEOUT_WEB) {
    comandoWeb = 'S'; // Forzar parada
  }

  // 3. LEER JOYSTICKS FÍSICOS
  int valCarro = analogRead(JOY_X_CARRO);
  int valElev = analogRead(JOY_Y_ELEVACION);
  int valGiro = analogRead(JOY_GIRO);

  // 4. LÓGICA DE CONTROL MIXTO (El Joystick tiene prioridad sobre la Web)
  
  // ---> Control del Carro (Motor A)
  if (valCarro > ZONA_MUERTA_MAX) moverCarro(true);       // Joystick: Adelante
  else if (valCarro < ZONA_MUERTA_MIN) moverCarro(false); // Joystick: Atrás
  else {
    // Si no se usa el Joystick, usamos la Web
    if (comandoWeb == 'F') moverCarro(true);
    else if (comandoWeb == 'B') moverCarro(false);
    else if (comandoWeb == 'S' || (comandoWeb != 'F' && comandoWeb != 'B')) detenerCarro();
  }

  // ---> Control de Elevación (Motor B)
  if (valElev > ZONA_MUERTA_MAX) moverElevacion(true);       // Joystick: Subir
  else if (valElev < ZONA_MUERTA_MIN) moverElevacion(false); // Joystick: Bajar
  else {
    // Si no se usa el Joystick, usamos la Web
    if (comandoWeb == 'U') moverElevacion(true);
    else if (comandoWeb == 'D') moverElevacion(false);
    else if (comandoWeb == 'S' || (comandoWeb != 'U' && comandoWeb != 'D')) detenerElevacion();
  }

  // ---> Control de Giro (Nema 17)
  if (valGiro < ZONA_MUERTA_MIN) {
    motorGiro.moveTo(motorGiro.currentPosition() - 10000); // Joystick: Izquierda
  } 
  else if (valGiro > ZONA_MUERTA_MAX) {
    motorGiro.moveTo(motorGiro.currentPosition() + 10000); // Joystick: Derecha
  } 
  else {
    // Si no se usa el Joystick, usamos la Web
    if (comandoWeb == 'L') motorGiro.moveTo(motorGiro.currentPosition() - 10000);
    else if (comandoWeb == 'R') motorGiro.moveTo(motorGiro.currentPosition() + 10000);
    else motorGiro.stop(); // Freno suave
  }

  // 5. EJECUTAR PASO DEL MOTOR A PASOS
  // Esta función debe ejecutarse sin bloqueos (sin delays) para garantizar la suavidad
  motorGiro.run();
}
