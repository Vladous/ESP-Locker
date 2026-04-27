#include "config.h"
#include "sensors.h"
#include "mqttHandler.h"

// --- Přepínač senzoru ---
// Kontrola, jestli je definovaný senzor (nastavuje se v config.h)
#include "config.h"
#include "sensors.h"
#include "mqttHandler.h"

// --- Přepínač senzoru ---
// Kontrola, jestli je definovaný senzor (nastavuje se v config.h)
#if defined(SENSOR_DHT)

  #if defined(MCU_ESP8266)
    #include <DHTesp.h>
    static DHTesp dht;

  #elif defined(MCU_ESP32)
    #include <DHT.h>
    #include "config.h"
    static DHT dht(DHT_PIN, DHT_TYPE);   // ✅ vytvoří instanci správně
  #endif

#elif defined(SENSOR_AM2320)
  #include <Wire.h>
  #include <Adafruit_AM2320.h>
  static Adafruit_AM2320 am2320 = Adafruit_AM2320();

#else
  #error "❌ Musíš definovat typ senzoru v config.h (SENSOR_DHT nebo SENSOR_AM2320)"
#endif

volatile float Teplota = 0.0;
volatile float Vlhkost = 0.0;
static int ampPin = -1;
volatile int16_t PwrAmp = 0;

void initTempSensor() {
#if defined(SENSOR_DHT)
  #if defined(MCU_ESP8266)
    dht.setup(DHT_PIN, DHTesp::DHT11);
  #elif defined(MCU_ESP32)
    dht.begin();  // ✅ žádné parametry, už je vše inicializováno v konstruktoru
  #endif

#elif defined(SENSOR_AM2320)
  Wire.begin();
  am2320.begin();
#endif
}

void updateTempSensor() {
#if defined(SENSOR_DHT)
  #if defined(ESP8266)
    TempAndHumidity vals = dht.getTempAndHumidity();
    if (!isnan(vals.temperature)) Teplota = roundf(vals.temperature * 10.0f) / 10.0f;
    if (!isnan(vals.humidity))    Vlhkost = roundf(vals.humidity);
  #elif defined(ESP32)
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) Teplota = roundf((t / KalibrT) * 10.0f) / 10.0f;
    if (!isnan(h)) Vlhkost = roundf(h / KalibrV);
  #endif
  
#elif defined(SENSOR_AM2320)
  float t = am2320.readTemperature();
  float h = am2320.readHumidity();
  if (!isnan(t)) Teplota = roundf((t / KalibrT) * 10.0f) / 10.0f;
  if (!isnan(h)) Vlhkost = roundf(h / KalibrV);
#endif
}

// --- Ampérmetr ---
void initAmpSensor(uint8_t pin) {
  ampPin = pin;
}

void updateMeasureAmp() {
  if (ampPin < 0) return;   // nebyl nastaven
  uint16_t raw = analogRead(ampPin);
  if (!isnan(raw)) {
    PwrAmp = (PwrAmp + raw) / 2;
  } else {
    debugMQTT("❌ Chyba: Nepodařilo se načíst hodnotu z ampérmetru.");
  }
}

// --- setter kalibrace (pro MQTT nebo EEPROM) ---
void setTempCalibration(float newKalibrT, float newKalibrV) {
  KalibrT = (newKalibrT > 0.0F) ? newKalibrT : 1.0F;
  KalibrV = (newKalibrV > 0.0F) ? newKalibrV : 1.0F;

  debugMQTT("🔧 Nové kalibrační konstanty: T=" + String(KalibrT) +
            " V=" + String(KalibrV));
}