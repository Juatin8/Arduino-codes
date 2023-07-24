#include "Servos.h"

Servos::Servos(int num) {
    numServos = num;
    servoArray = new Servo[numServos];
  }
  Servos::~Servos() {
    delete[] servoArray;
  }


void Servos::swing2Max(int index, int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY) {
    for (int angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle++) {
      servoArray[index].write(angle);  // 设置舵机角度
      delay(SERVO_DELAY);
    }
  }

void Servos::swing2Min(int index, int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY) {
    for (int angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle--) {
      servoArray[index].write(angle);  // 设置舵机角度
      delay(SERVO_DELAY);
    }
  }

int Servos::read(int index) {
    if (index >= 0 && index < numServos) {
      return servoArray[index].read();
    }
    return -1;
  }
void Servos::write(int index, int angle) {
    if (index >= 0 && index < numServos) {
      servoArray[index].write(angle);
    }
  }
void Servos::writeAll(int angle) {
    for (int i = 0; i < numServos; i++) {
      servoArray[i].write(angle);
    }
  }

void Servos::attach(int* pins) {
    for (int i = 0; i < numServos; i++) {
      servoArray[i].attach(pins[i]);
      servoArray[i].setPeriodHertz(50);
    }
  }
  void Servos::attach(int index, int pin) {
    if (index >= 0 && index < numServos) {
      servoArray[index].attach(pin);
      servoArray[index].setPeriodHertz(50);
    }
  }
  void Servos::detach(int index) {
    if (index >= 0 && index < numServos) {
      servoArray[index].detach();
    }
  }
  void Servos::detachAll() {
    for (int i = 0; i < numServos; i++) {
      servoArray[i].detach();
    }
  }


