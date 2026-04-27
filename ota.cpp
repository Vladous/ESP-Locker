#include "ota.h"
#include <SHA2Builder.h>
#include <ESPmDNS.h>
//#include "mqttHandler.h"  // kvůli debugMQTT

void initOTA(const char* hostname) {
  ArduinoOTA.setPassword("1");
  ArduinoOTA.setHostname(hostname);
  
  if (!MDNS.begin(hostname)) {
    Serial.println("❌ MDNS start selhal!");
    return;
  }

  ArduinoOTA.onStart([]() {
    // debugMQTT("🔁 OTA aktualizace spuštěna");
  });

  ArduinoOTA.onEnd([]() {
    // debugMQTT("✅ OTA aktualizace dokončena");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    // char buf[64];
    // sprintf(buf, "📶 OTA %u%%", (progress * 100) / total);
    // debugMQTT(buf);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    // char buf[64];
    // sprintf(buf, "❌ OTA chyba: %u", error);
    // debugMQTT(buf);
  });

  ArduinoOTA.begin();
}

void handleOTA() {
  ArduinoOTA.handle();
}