#include <Arduino.h> //必须放的
#include <ESP32Servo.h>

class Servos {
private:
  int numServos;
  Servo* servoArray;
public:
  Servos(int num);
  ~Servos();
  void attach(int* pins);
  void attach(int index, int pin);
  void detach(int index);
  void detachAll();
  void write(int index, int angle);
  void writeAll(int angle);
  int read(int index);
  void swing2Max(int index, int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY);
  void swing2Min(int index, int SERVO_MIN_ANGLE, int SERVO_MAX_ANGLE, int SERVO_DELAY) ;
};