#pragma once
#include "rfid.h"
#include <Arduino.h>

// --- Nastavení desky ---
// #define MCU_ESP8266
 #define MCU_ESP32

// // --- Typ senzoru ---
 #define SENSOR_DHT
// #define SENSOR_AM2320
inline bool  merit         = false;
inline float cekejMereni   = 2.0F;
inline float KalibrT = 1.19;     
inline float KalibrV = 0.87;
constexpr int DHT_PIN   = 25;
#define DHT_TYPE DHT11

// --- Verze programu ---
constexpr const char* VERSION = "0.3";

// --- WiFi & MQTT ---
// Soukromé údaje definuj v config_local.h (tento soubor je v .gitignore)
// Zkopíruj config_local.h.example → config_local.h a vyplň své hodnoty
#include "config_local.h"
// --- Globální stav ---
inline String wifiHostName = WIFI_HOSTNAME;
inline String deskName     = "ESP8266";

// MQTT
inline bool  odeslat       = true;
inline float cekejOdeslat  = 20.0F;
inline bool  Lock          = false;

// Nastavení pro RFID RC522
#define RFID_SDA   5
#define RFID_SCK   18
#define RFID_MISO  23
#define RFID_MOSI  19
#define RFID_RST   4

// Povolené karty (UID). Pole, kde je každá karta jako string (HEX).
constexpr const char* ALLOWED_UIDS[] = {
  "A9 54 53 D5",   // testovací karta 1
  "12 34 56 78",   // testovací karta 2
};
constexpr size_t ALLOWED_UIDS_COUNT = sizeof(ALLOWED_UIDS) / sizeof(ALLOWED_UIDS[0]);

// --- Konfigurace servo
#define SERVO_PIN 13
#define SERVO_OPEN 20
#define SERVO_CLOSE 140
#define SERVO_MOVE_TIME 1000 // servo je "zaneprázdněné"

// --- Konfigurace k uložení --- eeprom
struct AppConfig {
  float sendDelay;
  String allowedCardsJson;
};
// Globální instance (bude naplněná při načtení z úložiště)
extern AppConfig appConfig;
// --- Typy pro univerzální práci ---
enum ConfigType { CFG_STRING, CFG_FLOAT, CFG_BOOL, CFG_UINT16};
struct ConfigEntry {
  const char* key;
  void* ptr;
  ConfigType type;
};
// --- Registry: seznam všech klíčů a proměnných ---
static ConfigEntry configTable[] = {
  { "sendDelay",   &appConfig.sendDelay,  CFG_FLOAT },
  { "allowedCards",&appConfig.allowedCardsJson, CFG_STRING },
};
static const int configCount = sizeof(configTable) / sizeof(ConfigEntry);