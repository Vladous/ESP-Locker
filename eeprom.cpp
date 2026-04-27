#include "config.h"
#include "eeprom.h"

AppConfig appConfig;

#if defined(ESP32)
  #include <Preferences.h>
  Preferences prefs;
#elif defined(ESP8266)
  #include <FS.h>
  #include <LittleFS.h>
  #include <ArduinoJson.h>
#endif

// ---------- ESP32: low-level wrappers ----------
#if defined(ESP32)

String loadConfig(const char* key, const String& def) {
  return prefs.getString(key, def);
}

float loadConfig(const char* key, float def) {
  return prefs.getFloat(key, def);
}

bool loadConfig(const char* key, bool def) {
  return prefs.getBool(key, def);
}

uint16_t loadConfig(const char* key, uint16_t def) {
  return prefs.getUShort(key, def);
}

bool saveConfig(const char* key, const String& value) {
  prefs.putString(key, value);
  return true;
}

bool saveConfig(const char* key, float value) {
  prefs.putFloat(key, value);
  return true;
}

bool saveConfig(const char* key, bool value) {
  prefs.putBool(key, value);
  return true;
}

bool saveConfig(const char* key, uint16_t value) {
  prefs.putUShort(key, value);
  return true;
}

#endif // ESP32


// ---------- ESP8266: LittleFS + JSON ----------
#if defined(ESP8266)

static DynamicJsonDocument doc(2048);

String loadConfig(const char* key, const String& def) {
  return doc.containsKey(key) ? doc[key].as<String>() : def;
}

float loadConfig(const char* key, float def) {
  return doc.containsKey(key) ? doc[key].as<float>() : def;
}

bool loadConfig(const char* key, bool def) {
  return doc.containsKey(key) ? doc[key].as<bool>() : def;
}

uint16_t loadConfig(const char* key, uint16_t def) {
  return doc.containsKey(key) ? doc[key].as<uint16_t>() : def;
}

bool saveConfig(const char* key, const String& value) {
  doc[key] = value;
  return true;
}

bool saveConfig(const char* key, float value) {
  doc[key] = value;
  return true;
}

bool saveConfig(const char* key, bool value) {
  doc[key] = value;
  return true;
}

bool saveConfig(const char* key, uint16_t value) {
  doc[key] = value;
  return true;
}

#endif // ESP8266


// ---------- Hromadné funkce ----------
void loadAllConfig() {
#if defined(ESP32)
  prefs.begin("app", true); // read-only
#elif defined(ESP8266)
  LittleFS.begin();
  File f = LittleFS.open("/config.json", "r");
  if (f) {
    DeserializationError err = deserializeJson(doc, f);
    if (err) {
      Serial.println("❌ JSON config nelze načíst, používám defaulty");
    }
    f.close();
  }
#endif

  for (int i = 0; i < configCount; i++) {
    auto& e = configTable[i];
    switch (e.type) {
      case CFG_STRING:
        *((String*)e.ptr) = loadConfig(e.key, *((String*)e.ptr));
        break;
      case CFG_FLOAT:
        *((float*)e.ptr) = loadConfig(e.key, *((float*)e.ptr));
        break;
      case CFG_BOOL:
        *((bool*)e.ptr) = loadConfig(e.key, *((bool*)e.ptr));
        break;
      case CFG_UINT16:
        *((uint16_t*)e.ptr) = loadConfig(e.key, *((uint16_t*)e.ptr));
        break;
    }
  }

#if defined(ESP32)
  prefs.end();
#endif
}

void saveAllConfig() {
#if defined(ESP32)
  prefs.begin("app", false);
#elif defined(ESP8266)
  doc.clear();
#endif

  for (int i = 0; i < configCount; i++) {
    auto& e = configTable[i];
    switch (e.type) {
      case CFG_STRING:
        saveConfig(e.key, *((String*)e.ptr));
        break;
      case CFG_FLOAT:
        saveConfig(e.key, *((float*)e.ptr));
        break;
      case CFG_BOOL:
        saveConfig(e.key, *((bool*)e.ptr));
        break;
      case CFG_UINT16:
        saveConfig(e.key, *((uint16_t*)e.ptr));
        break;
    }
  }

#if defined(ESP32)
  prefs.end();
#elif defined(ESP8266)
  File f = LittleFS.open("/config.json", "w");
  serializeJson(doc, f);
  f.close();
#endif
}