#include <Arduino.h>
#include "ACS712.h"

// GPIO 3, 3.3V referencia, 12 bits ADC, 100 mV/A
ACS712 ACS(8, 3.3, 4095, 100.0);

// Coeficientes de calibración lineal derivados de tus mediciones
const float CAL_SLOPE = 0.825;   // Factor de escala
const float CAL_OFFSET = -46.4;  // Offset en mA

// Filtro EMA (0.05 a 0.2: menor valor = más suave pero respuesta más lenta)
const float ALPHA = 0.15;
float filteredCurrent = 0.0;

const float Kt=0.436822307;
float Torque=0.0;
float current=0.0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  analogSetPinAttenuation(8, ADC_11db);

  Serial.println("Calibrando cero del ACS712...");
  ACS.autoMidPointDC(); 
  Serial.print("Punto medio ADC detectado: ");
  Serial.println(ACS.getMidPoint());
  Serial.println("Calibracion lista.");
}

void loop() {
  // 1. Sobremuestreo para promediar ruido interno del ADC
  float raw_ma = ACS.mA_DC(150);

  // 2. Corrección de pendiente y offset
  float corrected_ma = (raw_ma * CAL_SLOPE) + CAL_OFFSET;

  // 3. Zona muerta para lecturas espurias cercanas a cero
  if (abs(corrected_ma) < 50.0) {
    corrected_ma = 0.0;
  }

  // 4. Filtro EMA para eliminar oscilaciones
  filteredCurrent = ((ALPHA * corrected_ma) + ((1.0 - ALPHA) * filteredCurrent)); 
  Torque=((filteredCurrent+150)/1000)*Kt;

  current=filteredCurrent+170; //150 es un offset calculado

  if (filteredCurrent < 20.0) {
    filteredCurrent = 0.0;
    current=0.0;
    Torque=0.0;
  }

  

  Serial.print("Corriente filtrada: ");
  Serial.print(current, 1); 
  Serial.print(" mA");
  Serial.print(" Torque: ");
  Serial.print(Torque, 1);
  Serial.println(" Nm");

  delay(100);
}