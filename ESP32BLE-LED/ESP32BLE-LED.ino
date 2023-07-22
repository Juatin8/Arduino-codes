#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"  // UUID 值
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

const int LEDpin = 25;

/* 非常重要！接收到数据后的处理逻辑都写这里!!!!*/
// BLECharacteristicCallbacks 为 BLECharacteristic 的回调函数
class MyCallbacks: public BLECharacteristicCallbacks { 
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value == "1") {
        digitalWrite(LEDpin,HIGH);
        Serial.println("lightOn");
      }
      else{
         digitalWrite(LEDpin,LOW);
         Serial.println("lightOff");
      }
    }
};

void setup() {
  Serial.begin(115200); 
  pinMode(LEDpin, OUTPUT);

  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");  // 连接中的feedback串口打印

  BLEDevice::init("MyESP32");   // 初始化 BLE 设备，设置名称
  BLEServer *pServer = BLEDevice::createServer();  // 创建一个 BLE Server

  BLEService *pService = pServer->createService(SERVICE_UUID);  // 创建一个服务

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(   // 创建一个特征
                                         CHARACTERISTIC_UUID,          
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );  // 特征属性为读写

  pCharacteristic->setCallbacks(new MyCallbacks());  // 设置回调函数，当有数据写入时，会调用回调函数

  pCharacteristic->setValue("Hello World");  // 设置特征的值
  pService->start();   // 启动服务

  BLEAdvertising *pAdvertising = pServer->getAdvertising();  // 获取广播对象
  pAdvertising->start();                                     // 启动广播
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}