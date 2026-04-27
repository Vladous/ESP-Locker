#include "servo.h"
#include "config.h"

Servo myServo;

static unsigned long busyUntil = 0;   // čas, dokdy servo ignoruje příkazy

void servoInit() {
  myServo.attach(SERVO_PIN, 500, 2400);
  myServo.write(SERVO_OPEN);   // startovní poloha
  delay(SERVO_MOVE_TIME);
  myServo.detach();
}

void servoSet(bool _lock) {
  // pokud je servo zaneprázdněné → ignoruj příkaz
  myServo.attach(SERVO_PIN, 500, 2400);
  if (millis() < busyUntil) return;

  if (_lock) {
    myServo.write(SERVO_CLOSE);
  } else {
    myServo.write(SERVO_OPEN);
  }
  delay(SERVO_MOVE_TIME);
  busyUntil = millis();  // zablokuj změny na 2s
  myServo.detach();
}

void servoLoop() {
  // tady by mohly být další logiky, např. plynulé animace
}