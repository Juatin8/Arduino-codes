#include "VL53L0Xs.h"

VL53L0Xs::VL53L0Xs(int num) {
  NUM_SENSORS = num;
  VL53L0XArray = new Adafruit_VL53L0X[NUM_SENSORS];
  
}
VL53L0Xs::~VL53L0Xs() {
  delete[] VL53L0XArray;
}

// 绑定
void VL53L0Xs::attach(int* XSHUTpins) {
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(XSHUTpins[i], OUTPUT);
    digitalWrite(XSHUTpins[i], LOW);
  }
}

void VL53L0Xs::ChangeAddress(int* XSHUTpins, uint8_t* Addresses) {
  //重置
  for (int i = 0; i < NUM_SENSORS; i++) {
    digitalWrite(XSHUTpins[i], LOW);
  }
  // 对每个设备进行如下操作
  for (int i = 0; i < NUM_SENSORS; i++) {
    digitalWrite(XSHUTpins[i], HIGH);
    delay(10);
    if (!VL53L0XArray[i].begin(Addresses[i])) {  //设置地址
      Serial.println(F("Failed to boot VL53L0X sensor"));
      while (1)
        ;
    }
    delay(10);
  }
}

void VL53L0Xs::read_multiple_sensors() {
  VL53L0X_RangingMeasurementData_t measure[NUM_SENSORS];
  for (int i = 0; i < NUM_SENSORS; i++) {
    VL53L0XArray[i].rangingTest(&measure[i], false);  //获取测量值，且不要debug数据
    Serial.print(i + 1);
    Serial.print(F(": "));
    if (measure[i].RangeStatus != 4) {
      Serial.print(measure[i].RangeMilliMeter);
    } else {
      Serial.print(F("Out of range"));
    }
    Serial.print(F(" "));
  }
  Serial.println();
}