#include "config.h"
#include "mqttHandler.h"
#include "eeprom.h"
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <Arduino.h>

void onMessage(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic);
  DynamicJsonDocument doc(512);

  // Bezpečné zkopírování payloadu
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    String msg = "❌ Chyba deserializace callback: ";
    msg += error.c_str();
    msg += " | Topic: ";
    msg += topicStr;
    msg += " | Data: ";
    msg += message;
    publishDebug(msg);
    return;
  }

  if (doc.containsKey("settings")) {
    const char* settingAction = doc["settings"];
    if (strcmp(settingAction, "get") == 0) {
      publishStatus();
    }

  } else if (doc.containsKey("restart")) {
    restartDevice();

  } else if (doc.containsKey("help")) {
    publishHelp();

  } else if (doc.containsKey("UIDset")) {
    JsonArray uidArray = doc["UIDset"].as<JsonArray>();
    std::vector<String> newUIDs;

    for (JsonVariant uid : uidArray) {
      if (uid.is<const char*>()) {
        newUIDs.push_back(uid.as<String>());
      }
    }

    if (!newUIDs.empty()) {
      setAllowedCards(newUIDs);
      saveAllConfig();
    }

  } else if (doc.containsKey("UIDget")) {
    std::vector<String> cards = getAllowedCards();
    DynamicJsonDocument response(512);
    JsonArray responseArr = response.createNestedArray("UIDs");

    for (const String& uid : cards) {
      responseArr.add(uid);
    }

    char buffer[512];
    serializeJson(response, buffer);
    client.publish(wifiHostName.c_str(), buffer);  // nebo wifiHostName.c_str()
  } else if (doc.containsKey("LockSet")) {
    Lock = doc["LockSet"].as<bool>();
    updateStatus("MQTT");
  }
}

void publishSend(const String& last) {
  DynamicJsonDocument doc(512);
  doc["Lock"] = Lock;

  doc["temp"] = Teplota;  // Přidání naměřené teploty do výstupu k odeslání (Adding the measured temperature to the output for sending)
  doc["hum"] = Vlhkost;   // Přidání naměřené vlhkosti do výstupu k odeslání (Adding the measured humidity to the output for sending)

  doc["signal"] = WiFi.RSSI();
  uint8_t* mac = WiFi.BSSID();
  if (mac) {
    char bssidStr[18];
    sprintf(bssidStr, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    doc["bssid"] = bssidStr;
  }
  if (last != "") {
    doc["lastUID"] = last;
  }

  char out[256];
  serializeJson(doc, out);
  client.publish(wifiHostName.c_str(), out);
}

void publishStatus() {
  DynamicJsonDocument responseDoc(512);
  responseDoc["ip"] = WiFi.localIP().toString();
  responseDoc["host"] = wifiHostName;
  responseDoc["CekejOdeslat"] = cekejOdeslat;
  responseDoc["Verze"] = VERSION;

  char responseOut[512];
  serializeJson(responseDoc, responseOut);
  client.publish(wifiHostName.c_str(), responseOut);
}

void publishHelp() {
  DynamicJsonDocument helpDoc(512);
  JsonArray arr = helpDoc.createNestedArray("commands");
  arr.add("restart");
  arr.add("settings-get");
  arr.add("help");

  char helpOut[512];
  serializeJson(helpDoc, helpOut);
  client.publish(wifiHostName.c_str(), helpOut);
}

void publishVersion() {
  String payload = "{\"device\":\"" + wifiHostName +
                   "\",\"version\":\"" + String(VERSION) + "\"}";
  client.publish("version", payload.c_str());
}

void publishDebug(const String& message) {
  String messageJson = "{\"device\":\"" + wifiHostName +
                       "\",\"message\":\"" + message + "\"}";
  client.publish("logs", messageJson.c_str());
}

void restartDevice() {
  ESP.restart();
}