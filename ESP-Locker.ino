//
const char* VERSION = "1.0";
// Přidat alarm

#include "config.h"
#include "display.h"
#include "wifi_mqtt.h"
#include "mqttHandler.h"
#include "ota.h"
#include "rfid.h"
#include "eeprom.h"
#include "servo.h"
#include "sensors.h"
#include <Ticker.h>
#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

Ticker TimerOdeslat, TimerMerit;

struct Pos {
  int16_t x;
  int16_t y;
};
Pos icoPos = {90,8};
Pos stavPos = {1,1};
Pos identPos = {1,28};

void setup() {
  Serial.begin(115200);
  displayInitialize();
  delay(100);
  displayPrint("START", 10, 0, 2);
  int16_t x = 6;
  x = dotWrite(x, 35, 1);
  displayPrint((String("Verze : ") + VERSION).c_str(),10,55,1);
  x = dotWrite(x, 35, 1);
  wifiMqttSetup(WIFI_NAME,WIFI_PASSWORD,WIFI_HOSTNAME,MQTT_SERVER,MQTT_PORT,"","",onMessage);
  x = dotWrite(x, 35, 1);
  initTempSensor();
  x = dotWrite(x, 35, 1);
  initTimers();
  x = dotWrite(x, 35, 1);
  initRFID();
  x = dotWrite(x, 35, 1);
  servoInit();
  x = dotWrite(x, 35, 1);
  delay(1000);
  displayClear();
  drawImage(UnLock_bitmap, icoPos.x, icoPos.y, 1);
  displayPrint("Odemceno", stavPos.x , stavPos.y, 2);
  displayPrint("SYSTEM", identPos.x , identPos.y, 2);
  displayShow();
  loadAllConfig();
}

void loop() {
  handleOTA();
  wifiMqttLoop();
  handleRFID();  
  String uid;
  bool present = false;
  bool accepted =false;
  rfidCheckCard(uid, present, accepted);
  if (present) {
    if (accepted) {
      changeState(uid);  // Předáš UID do funkce
    } else  {
      if (Lock) {
        displayPrint("---X---", identPos.x , identPos.y, 2);
        displayShow();
        publishSend(uid);
        delay(500);
        displayPrint("   X   ", identPos.x , identPos.y, 2);
        displayShow();
      }
    }
  }

  if (merit) {
    updateTempSensor();
    char teplo[32];
    snprintf(teplo, sizeof(teplo), "%.1f'C  %ddB", Teplota, WiFi.RSSI());
    displayPrint(teplo, 1 , 56, 1);
    displayShow();
    merit =  false;
  }

  if (odeslat) {
    publishSend();
    odeslat = false;
  }

  // int LedBright = LedL;
  // if (defaultConfig.NightKontrolLedEnable) {
  //   LedBright = LedBright /10;
  // }
  // // Úprava nstavení jasu kontrolek (Adjusting the brightness setting of the lights)
  // analogWrite(LedPWR, LedBright / defaultConfig.KalibrKontrolRed);
  // if (IsConnected) {
  //   analogWrite(LedWi, LedBright / defaultConfig.KalibrKontrolBlue);
  // }  else {
  //   analogWrite(LedWi, 0);
  // }
  if (Lock == true) {
    //analogWrite(PwrSw, LedBright / defaultConfig.KalibrKontrolGreen);
  }
}

void initTimers() {
  TimerOdeslat.attach(cekejOdeslat, odeslatData);
  TimerMerit.attach(cekejMereni, meritTemp);
}

void odeslatData() {
  odeslat = true;
}

void meritTemp() {
  merit = true;
}

// Callback pro PubSubClient
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  onMessage(topic, payload, length);
}

void changeState(String uid) {
  Lock = !Lock;   // přepni stav hned na začátku
  updateStatus(uid);
}

void updateStatus(String uid) {
  if (Lock) {
    //displayClear();
    drawImage(UnLock_bitmap, icoPos.x, icoPos.y, 0);
    drawImage(Lock_bitmap, icoPos.x, icoPos.y, 1);
    displayPrint("Zamceno ", stavPos.x , stavPos.y, 2);
    displayPrint("   X   ", identPos.x , identPos.y, 2);
    displayShow();
  } else {
    //displayClear();
    drawImage(Lock_bitmap, icoPos.x, icoPos.y, 0);
    drawImage(UnLock_bitmap, icoPos.x, icoPos.y, 1);
    displayPrint("Odemceno", stavPos.x , stavPos.y, 2);
    const char* name = helpName(uid);
    displayPrint(name, identPos.x , identPos.y, 2);
    displayShow();
  }
  servoSet(Lock);
  publishSend(rfidGetLastUID());
}

int dotWrite(int16_t x, int16_t y, int8_t size ) {
  displayPrint(".", x+=6 ,35,1);
  displayShow();
  delay(500);
  return x;
}

const char* helpName(const String& uid) {
  if (uid == "A9 54 53 D5") {
    return "Vladous";
  } else if (uid == "12 34 56 78") {
    return "TestKarta";
  }
   else if (uid == "MQTT") {
    return "MQTT";
  }
  return "Neznamy";
}
