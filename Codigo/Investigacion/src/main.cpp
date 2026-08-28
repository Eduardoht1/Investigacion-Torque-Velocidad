#include <Arduino.h>
#include <ESP32Encoder.h>

ESP32Encoder encoder;

// Define your safe 3.3V-level encoder GPIO pins here
const int ENC_PIN_A = 16; 
const int ENC_PIN_B = 17;
const int PPR = 600;
const unsigned long interval = 100; 

unsigned long currentTime =0;
unsigned long lastTime = 0;
int64_t lastCount = 0;
float multiplier = 4.0; 

void setup() {
  Serial.begin(115200);
  
  ESP32Encoder::useInternalWeakPullResistors = puType::up;

  encoder.attachFullQuad(ENC_PIN_A, ENC_PIN_B);
  
  encoder.clearCount();
  Serial.println("Encoder Initialized.");
}

void loop() {
  currentTime = millis();
  
  if (currentTime - lastTime >= interval) {
    int64_t currentCount = encoder.getCount();

    // Cálculo directo de RPM
    float rpm = ((currentCount - lastCount) * 60000.0) / ((currentTime - lastTime) * PPR * multiplier);

    Serial.print(" | RPM: ");
    Serial.println(rpm);

    lastCount = currentCount;
    lastTime = currentTime;
  }
}
