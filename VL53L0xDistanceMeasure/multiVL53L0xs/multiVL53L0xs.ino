#include "VL53L0Xs.h"

VL53L0Xs myl0xs(2);
uint8_t sensorAddresses[] = { 0x30, 0x31 };
int shutdownPins[] = { 9, 10 };

void setup() {
  startSerial();
  myl0xs.attach(shutdownPins);  //绑定引脚
  Serial.println(F("Shutdown pins inited..."));
  Serial.println(F("Starting..."));
  myl0xs.ChangeAddress(shutdownPins, sensorAddresses);
}

void loop() {
  myl0xs.read_multiple_sensors();
  delay(100);
}


//------------------------------------------
void startSerial()
{
    Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
}
