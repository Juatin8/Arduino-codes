#include "Adafruit_VL53L0X.h"

#define NUM_SENSORS 2

Adafruit_VL53L0X lox[NUM_SENSORS];
VL53L0X_RangingMeasurementData_t measure[NUM_SENSORS];
uint8_t sensorAddresses[NUM_SENSORS] = { 0x30, 0x31 };
uint8_t shutdownPins[NUM_SENSORS] = { 9, 10 };

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }

  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(shutdownPins[i], OUTPUT);
    digitalWrite(shutdownPins[i], LOW);
  }

  Serial.println(F("Shutdown pins inited..."));
  Serial.println(F("Starting..."));
  setID();
}

void loop() {
  read_multiple_sensors();
  delay(100);
}

//----------------- 函数封装------------------------------
void setID() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    digitalWrite(shutdownPins[i], LOW);
  }

  for (int i = 0; i < NUM_SENSORS; i++) {   // 对每个设备进行如下操作i
    digitalWrite(shutdownPins[i], HIGH);
    delay(10);
    if (!lox[i].begin(sensorAddresses[i])) {  //设置地址
      Serial.println(F("Failed to boot VL53L0X sensor"));
      while (1);
    }
    delay(10);
  }
}

void read_multiple_sensors() {
  for (int i = 0; i < NUM_SENSORS; i++) {
    lox[i].rangingTest(&measure[i], false);  //获取测量值，且不要debug数据
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
