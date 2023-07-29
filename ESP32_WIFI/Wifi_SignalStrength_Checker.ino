#include "WiFi.h"
 
// SSID and Password - modify for your WiFi network
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
 
void setup(){
  // Setup serial monitor
  Serial.begin(115200);
  // Start WiFi and conect to network
  WiFi.begin(ssid, password);
  // Wait while connection established
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }
  // WiFi is connected
  Serial.println("");
  Serial.print("Connected to network ");
  Serial.println(ssid);
  delay(100);
}
 
void loop(){
  Serial.print(ssid);
  Serial.print(" Signal Level: ");
  Serial.println(WiFi.RSSI());
  delay(2000);
}
