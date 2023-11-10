#include <Arduino.h> //必须放的
#include "Adafruit_VL53L0X.h"

class VL53L0Xs {
private:
int NUM_SENSORS;
Adafruit_VL53L0X* VL53L0XArray;
public:
VL53L0Xs(int num);
~VL53L0Xs();
void attach(int* XSHUTpins);
void ChangeAddress(int* XSHUTpins, uint8_t* Addresses);
void read_multiple_sensors();
};