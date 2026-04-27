// Vložit do ino :
// ---------------
// #include "servo.h"
//
// void setup() {
//   servoInit();
// }
//
// void loop() {  
//   void servoSet(bool _lock);
// }
//
// Vložit do config.h :
// ----------
// Servo SG90
//
// #define SERVO_PIN 1   // D10 = GPIO1 (TX)
// #define SERVO_OPEN 20
// #define SERVO_CLOSE 160
// #define SERVO_MOVE_TIME 2000 // servo je "zaneprázdněné" na 2s

#pragma once
#if defined(ESP32)
  #include <ESP32Servo.h>
#elif defined(ESP8266)
  #include <Servo.h>  
#endif

extern Servo myServo;

void servoInit();
void servoSet(bool _lock);
void servoLoop();   // musíš volat v loop()
