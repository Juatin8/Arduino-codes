#include "myvl53l0x.h"
#include "Adafruit_VL53L0X.h"

myvl53l0x::myvl53l0x()
  : lox() {
    uint16_t distance;
}
myvl53l0x::~myvl53l0x() {}

void myvl53l0x::initiate() {
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1)
      ;
  }
  Serial.println(F("VL53L0X API Continuous Ranging\n\n"));
  lox.startRangeContinuous();
}


uint16_t myvl53l0x::getdistance() {
  if (lox.isRangeComplete()) {
    distance = lox.readRange();
    return distance;
  }
}

void myvl53l0x::printdistance(int i) {
  String sensor1Str = String(i + 1);
  String distanceStr = String(distance);
  String output = "sensor" + sensor1Str + " distance: " + distanceStr;
  Serial.println(output);
}
