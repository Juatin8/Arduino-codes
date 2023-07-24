#include "Servos.h"

Servos myServos(3);  //构造Servos对象

int servoPins[] = { 14, 27, 26 };  // 舵机引脚号数组
int delayTime = 3;       // 脉冲停留
int minAngle = 0;  //初始角度
int maxAngle = 180;

void setup() {
  myServos.attach(servoPins);        // 给舵机绑定引脚
  myServos.writeAll(minAngle);   //初始化位置
  delay(15);                         // 等待舵机到达目标位置
}

void loop() {
  myServos.swing2Max(0, minAngle, maxAngle, delayTime);
  myServos.swing2Max(1, minAngle, maxAngle, delayTime);
  myServos.swing2Max(2, minAngle, maxAngle, delayTime);
  myServos.swing2Min(0, minAngle, maxAngle, delayTime);
  myServos.swing2Min(1, minAngle, maxAngle, delayTime);
  myServos.swing2Min(2, minAngle, maxAngle, delayTime);
  delay(1000);
}
