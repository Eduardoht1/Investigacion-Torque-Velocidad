#include <Arduino.h>
#include <ESP32Encoder.h>

ESP32Encoder encoder;

// Define your safe 3.3V-level encoder GPIO pins here
const int ENC_PIN_A = 16; 
const int ENC_PIN_B = 17;
const int PPR = 600;
const unsigned long interval = 5; // Muestreo cambiado a 50 ms

unsigned long currentTime = 0;
unsigned long lastTime = 0;
int64_t lastCount = 0;
float multiplier = 4.0; 

// Variables para el control de la secuencia
const unsigned long waitTime = 10000;   // 5 segundos de espera inicial
const unsigned long recordTime = 5000; // 5 segundos de registro de datos
bool isRecording = false;
bool isDone = false;

void setup() {
  Serial.begin(115200);
  
  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  encoder.attachFullQuad(ENC_PIN_A, ENC_PIN_B);
  encoder.clearCount();
  
  Serial.println("Esperando 5 segundos antes de iniciar el registro...");
}

void loop() {
  if (isDone) {
    return; // Detiene la ejecución en el loop una vez que termina el registro
  }

  currentTime = millis();
  
  // 1. Esperar 5 segundos antes de empezar a registrar
  if (!isRecording && currentTime >= waitTime) {
    isRecording = true;
    Serial.println("Segundos,RPM"); // Encabezado del CSV
    lastTime = currentTime;
    lastCount = encoder.getCount();
  }

  // 2. Registrar datos durante los 5 segundos asignados
  if (isRecording && currentTime < (waitTime + recordTime)) {
    if (currentTime - lastTime >= interval) {
      int64_t currentCount = encoder.getCount();

      // Cálculo directo de RPM
      float rpm = ((currentCount - lastCount) * 60000.0) / ((currentTime - lastTime) * PPR * multiplier);

      // Calcular el tiempo transcurrido desde que inició el registro (en segundos)
      float elapsedSeconds = (currentTime - waitTime) / 1000.0;

      // Imprimir en formato CSV (Tiempo, RPM)
      Serial.print(elapsedSeconds, 3); // Imprime con 3 decimales (milisegundos)
      Serial.print(",");
      Serial.println(rpm);

      lastCount = currentCount;
      lastTime = currentTime;
    }
  } 
  // 3. Detenerse al completar los 5 segundos de registro
  else if (isRecording && currentTime >= (waitTime + recordTime)) {
    Serial.println("Registro completado.");
    isDone = true; 
  }
}