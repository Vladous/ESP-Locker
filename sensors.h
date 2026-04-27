// Vložit do ino :
// ---------------
// #include "sensors.h"
//
// void setup() {
//   initTempSensor();
//   initAmpSensor(AMP_PIN);
// }
//
// void loop() {  
//   updateTempSensor();
//   updateMeasureAmp();
// }
//
// Vložit do config.h :
// ----------
// Senzory
//
// // --- Typ senzoru ---
// #define SENSOR_DHT
// // #define SENSOR_AM2320
//
// inline bool  merit         = false;
// inline float cekejMereni   = 2.0F;
// constexpr int AMP_PIN      = A0;
// constexpr int DHT_PIN   = 4;  // pokud bys někdy chtěl
//

#pragma once
#include <Arduino.h>

// --- Globální hodnoty, na které můžeš přímo koukat ---
// (pozor: volatile, protože se aktualizují z jiných částí kódu)
extern volatile float Teplota;
extern volatile float Vlhkost;
extern volatile int16_t PwrAmp;

// --- Kalibrační konstanty ---
// (můžou se měnit přes EEPROM nebo MQTT)
extern float KalibrT;
extern float KalibrV;

// Inicializace senzoru (pin použij jen pro DHT variantu, jinak ignoruj)
void initTempSensor();
// Aktualizace hodnot (neblokující, volat pravidelně z loop())
void updateTempSensor();

// Inicializace senzoru
void initAmpSensor(uint8_t pin);
// Měření ampérmetru (na zadaném analogovém pinu)
void updateMeasureAmp();

// Nastavení kalibračních konstant (např. po příjmu MQTT zprávy)
void setTempCalibration(float newKalibrT, float newKalibrV);
