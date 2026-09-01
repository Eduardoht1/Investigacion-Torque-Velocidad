#include <Arduino.h>
#include "ACS712.h"

// GPIO 3, 3.3V referencia, 12 bits ADC, 100 mV/A
ACS712 ACS(3, 3.3, 4095, 100.0);

// Coeficientes de calibración lineal derivados de tus mediciones
const float CAL_SLOPE = 0.825;   // Factor de escala
const float CAL_OFFSET = -46.4;  // Offset en mA

// Filtro EMA (0.05 a 0.2: menor valor = más suave pero respuesta más lenta)
const float ALPHA = 0.15;
float filteredCurrent = 0.0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  analogSetPinAttenuation(3, ADC_11db);

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
  filteredCurrent = (ALPHA * corrected_ma) + ((1.0 - ALPHA) * filteredCurrent);

  Serial.print("Corriente filtrada: ");
  Serial.print(filteredCurrent, 1);
  Serial.println(" mA");

  delay(100);
}