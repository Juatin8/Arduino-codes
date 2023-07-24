#include "Servos.h"

Servos::Servos(int num) {
  numServos = num;
  servoArray = new Servo[numServos];
}
Servos::~Servos() {
  delete[] servoArray;
}

// 绑定
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
// 解绑，停下
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

// 定义旋转角
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

// 读取旋转角
int Servos::read(int index) {
  if (index >= 0 && index < numServos) {
    return servoArray[index].read();
  }
  return -1;
}

// 摇摆
void Servos::swing2Max(int index, int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY) {
  for (int angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle++) {
    servoArray[index].write(angle);  // 设置舵机角度
    delay(SERVO_DELAY);
  }
}

void Servos::swingAllSyn2Max(int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY) {
  for (int angle = SERVO_MIN_ANGLE; angle <= SERVO_MAX_ANGLE; angle++) {
    for (int i = 0; i < numServos; i++) {
      servoArray[i].write(angle);  // 设置舵机角度
      delay(SERVO_DELAY);
    }
  }
}
void Servos::swingAllASyn2Max(int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY, int Btween_delay) {
  for (int i = 0; i < numServos; i++) {
  for (int angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle++) {
      servoArray[i].write(angle);  // 设置舵机角度
      delay(SERVO_DELAY);
  }
   delay(Btween_delay);
}
}

void Servos::swing2Min(int index, int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY) {
  for (int angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle--) {
    servoArray[index].write(angle);  // 设置舵机角度
    delay(SERVO_DELAY);
  }
}

void Servos::swingAllSyn2Min(int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY) {
  for (int angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle--) {
    for (int i = 0; i < numServos; i++) {
      servoArray[i].write(angle);  // 设置舵机角度
    }
  }
}

void Servos::swingAllASyn2Min(int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY, int Btween_delay) {
  for (int i = 0; i < numServos; i++) {
  for (int angle = SERVO_MAX_ANGLE; angle >= SERVO_MIN_ANGLE; angle--) {
      servoArray[i].write(angle);  // 设置舵机角度
      delay(SERVO_DELAY);
  }
   delay(Btween_delay);
}
}
