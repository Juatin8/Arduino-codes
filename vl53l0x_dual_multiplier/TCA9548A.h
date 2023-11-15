#include <Wire.h>
#include <Arduino.h>  //必须放的

class TCA9548A {
private:
public:
  TCA9548A();
  ~TCA9548A();
  void StartCommunication();
  void Open(uint8_t gate);
};
