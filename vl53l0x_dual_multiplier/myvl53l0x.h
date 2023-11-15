#include <Arduino.h>  //必须放的
#include <Wire.h>
#include "Adafruit_VL53L0X.h"

class myvl53l0x {
private:
  Adafruit_VL53L0X lox;
  uint16_t distance;
public:
  myvl53l0x();
  ~myvl53l0x();
  void initiate();
  uint16_t getdistance();
  void printdistance(int i);
};