#include <Wire.h>
#include "TCA9548A.h"
#include "myvl53l0x.h"

int busArr[3] = { 5, 1, 4 };
uint16_t values[3];
int numSensors = 3;
myvl53l0x lox[3];
TCA9548A gate;

void setup() {
  gate.StartCommunication();
  for (int i = 0; i < numSensors; i++) {
    gate.Open(busArr[i]);
    lox[i].initiate();
  }
}

void loop() {
  for (int i = 0; i < numSensors; i++) {
    gate.Open(busArr[i]);
    values[i]=lox[i].getdistance();
    lox[i].printdistance(i);
    delay(100);
  }
}
