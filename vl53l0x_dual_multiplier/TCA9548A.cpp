#include "TCA9548A.h"

TCA9548A::TCA9548A() {
}
TCA9548A::~TCA9548A() {}

void TCA9548A ::StartCommunication()
{
  Serial.begin(115200);
  Wire.begin(); }
void TCA9548A ::Open(uint8_t gate) {
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << gate);         // send byte to select bus
  Wire.endTransmission();
  Serial.print(gate);
  while (!Serial) {
    delay(1);
  }
  }

