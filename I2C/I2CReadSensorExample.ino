#include <Wire.h>

// I2C address of the sensor
#define SENSOR_ADDRESS 0x68

void setup() {
  Wire.begin(); // Initialize I2C communication
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  // Request 6 bytes of data from the sensor
  Wire.beginTransmission(SENSOR_ADDRESS);
  Wire.write(0x3B); // Starting register address of sensor data
  Wire.endTransmission(false);
  Wire.requestFrom(SENSOR_ADDRESS, 6, true);

  // Read the received data
  int16_t accelerometer_x = Wire.read() << 8 | Wire.read();
  int16_t accelerometer_y = Wire.read() << 8 | Wire.read();
  int16_t accelerometer_z = Wire.read() << 8 | Wire.read();

  // Print the sensor data
  Serial.print("Accelerometer X: ");
  Serial.println(accelerometer_x);
  Serial.print("Accelerometer Y: ");
  Serial.println(accelerometer_y);
  Serial.print("Accelerometer Z: ");
  Serial.println(accelerometer_z);

  delay(1000); // Delay for 1 second
}
