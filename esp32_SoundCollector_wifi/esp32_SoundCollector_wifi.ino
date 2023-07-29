#include <WiFi.h>
#include "ESPAsyncWebServer.h"
AsyncWebServer server(80);
float db=0.0;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
  
void SmartConfig()
{    WiFi.mode(WIFI_STA);
    Serial.println("\r\n wait for smartconfig....");
    WiFi.beginSmartConfig();
    while (1)
    {
        Serial.print(".");
        delay(500);
        if (WiFi.smartConfigDone())
        {
            Serial.println("SmartConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
            break;          }     }  }

bool AutoConfig()
{    WiFi.begin();
    for (int i = 0; i < 20; i++)
    {
        int wstatus = WiFi.status();
        if (wstatus == WL_CONNECTED)
        {
            Serial.println("wifi smartConfig success");
            Serial.printf("SSID:%s", WiFi.SSID().c_str());
            Serial.printf(",PWS:%s\r\n", WiFi.psk().c_str());
            Serial.print("localIP:");
            Serial.println(WiFi.localIP());
            Serial.print(",GateIP:");
            Serial.println(WiFi.gatewayIP());
            return true;
        }
        else
        {
            Serial.print("WIFI AutoConfig Waiting ....");
            Serial.println(wstatus);
            delay(1000);
        }
    }
    Serial.println("Wifi autoconfig failed!");
    return false;
}


void setup()
{
Serial.begin(115200);
    delay(100);
    if (!AutoConfig())
    {        SmartConfig();    }
    Serial.print("http://");
    Serial.println(WiFi.localIP());
    server.on("/text", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(db).c_str());
  });
    server.begin();}

void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
   unsigned int signalMax = 0;
   unsigned int signalMin = 4096;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(35);
      if (sample < 4096)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   db = (peakToPeak * 3.3) / 1.83/4;  // convert to volts
   Serial.println(db);
}
