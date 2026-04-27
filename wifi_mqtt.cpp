#include "config.h"
#include "wifi_mqtt.h"
#include "mqttHandler.h"
#include "ota.h"

// --- Lokální proměnné ---
static MqttCallback _cb = nullptr;
static const char* _ssid;
static const char* _password;
static const char* _hostname;
static const char* _mqtt_server;
static uint16_t    _mqtt_port;
static const char* _mqtt_user;
static const char* _mqtt_pswd;

static unsigned long lastWifiAttempt = 0;

WiFiClient espClient;
PubSubClient client(espClient);

static void wifiConnect() {
  if (WiFi.status() != WL_CONNECTED) {
    if (millis() - lastWifiAttempt > 10000) {   // každých 10 s
      lastWifiAttempt = millis();
      Serial.println("Zkousim pripojit WiFi...");
      WiFi.begin(_ssid, _password);

      if (_hostname && strlen(_hostname) > 0) {
        #if defined(ESP8266)
          WiFi.hostname(_hostname);
        #elif defined(ESP32)
          WiFi.setHostname(_hostname);
        #endif
      }
    }
  }
}

void mqttConnect() {
  if (!client.connected()) {
    static unsigned long lastMqttAttempt = 0;
    if (millis() - lastMqttAttempt > 5000) {   // každých 5 s
      lastMqttAttempt = millis();
      Serial.println("Zkousim pripojit MQTT...");

      // musí být při každém pokusu znovu
      client.setServer(_mqtt_server, _mqtt_port);
      client.setCallback(_cb);

      if (client.connect(_hostname, _mqtt_user, _mqtt_pswd)) {
        Serial.println("MQTT připojeno");
        client.subscribe(_hostname);
        debugMQTT("✅ Úspěšně připojeno");
        initOTA(WIFI_HOSTNAME);
        //reportFirmwareVersion();
      } else {
        Serial.print("MQTT pořád nic... rc=");
        Serial.println(client.state());
      }
    }
  }
}

// --- Veřejné funkce ---
void wifiMqttSetup(const char* ssid,
                   const char* password,
                   const char* hostname,
                   const char* mqtt_server,
                   uint16_t    mqtt_port,
                   const char* mqtt_user,
                   const char* mqtt_pswd,
                    MqttCallback cb) {
  _ssid = ssid;
  _password = password;
  _hostname = hostname;
  _mqtt_server = mqtt_server;
  _mqtt_port = mqtt_port;
  _mqtt_user = mqtt_user;
  _mqtt_pswd = mqtt_pswd;
  _cb = cb;

  wifiConnect();

  client.setServer(_mqtt_server, _mqtt_port);
  mqttConnect();
}

void wifiMqttLoop() {
  wifiConnect();          // neblokující WiFi reconnect
  if (WiFi.status() == WL_CONNECTED) {
    mqttConnect();        // neblokující MQTT reconnect
    client.loop();        // MQTT obsluha
  }
}

IPAddress wifiMqttIP() {
  return WiFi.localIP();
}