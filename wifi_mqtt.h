#pragma once

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
//#include <uMQTTBroker.h>
#include <PubSubClient.h>

typedef void (*MqttCallback)(char* topic, byte* payload, unsigned int length);

// API funkcí
void wifiMqttSetup(const char* ssid,
                   const char* password,
                   const char* hostname,
                   const char* mqtt_server,
                   uint16_t    mqtt_port,
                   const char* mqtt_user = nullptr,
                   const char* mqtt_pswd = nullptr,
                   MqttCallback cb = nullptr);

void wifiMqttLoop();          // volat z loop()
bool wifiMqttConnected();     // vrací true pokud je MQTT připojeno
IPAddress wifiMqttIP();       // vrací IP adresu zařízení
