#pragma once
#include <ArduinoJson.h>
#include <PubSubClient.h>

// MQTT klient je definován ve wifi_mqtt.cpp
extern PubSubClient client;

// globální proměnné jsou v .ino (Lock, merit, cekejOdeslat, wifiHostName, VERSION)
extern bool Lock;
extern volatile float Teplota;
extern volatile float Vlhkost;
extern bool merit;
extern float cekejOdeslat;
extern String wifiHostName;

// callback pro PubSubClient
void onMessage(char* topic, byte* payload, unsigned int length);

// publikace
void publishStatus();
void publishHelp();
void publishVersion();
void publishDebug(const String& message);
void publishSend(const String& last = "");

// utility
void restartDevice();
void updateStatus(String uid);

// alias aby to bylo stejné jako dřív
inline void debugMQTT(const String& message) { publishDebug(message); }