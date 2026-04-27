#pragma once
#include <Arduino.h>

#if defined(ESP32)
  #include <Preferences.h>
#elif defined(ESP8266)
  #include <FS.h>
  #include <LittleFS.h>
#endif

// Inicializace
bool initStorage(const char* ns = "config");

// Uložení hodnot
bool saveConfig(const char* key, const String& value);
bool saveConfig(const char* key, float value);
bool saveConfig(const char* key, bool value);

// Načtení hodnot
String loadConfig(const char* key, const String& defaultValue = "");
float  loadConfig(const char* key, float defaultValue = 0.0f);
bool   loadConfig(const char* key, bool defaultValue = false);

// --- Hromadné ---
void loadAllConfig();
void saveAllConfig();