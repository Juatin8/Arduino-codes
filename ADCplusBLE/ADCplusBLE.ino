//---------------蓝牙相关----------------------
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer;
BLECharacteristic *pCharacteristic;

//#include <Wire.h>
const int inputPin = 3; // 定义输入引脚为IO3

//------------------- setup---------------------
void setup() {
  pinMode(inputPin, INPUT);
  StartMessage();   //启动消息

  delay(1000); //给芯片启动时间，然后再启动蓝牙
   
  // 创建蓝牙设备，名称
  BLEDevice::init("MyESP32"); //调用BLEDevice类的init()方法。::可以简单理解成调用
  // 创建蓝牙服务器
  pServer = BLEDevice::createServer();  
  // 创建蓝牙服务
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // 创建BLE特征
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |      // 添加可读可写属性
                      BLECharacteristic::PROPERTY_NOTIFY       // 添加可通知属性
                    );

  // 设置特征的初始值
  pCharacteristic->setValue("Hello World");

  // 启动服务
  pService->start(); //调用pService指向的BLEService对象的start()方法

  // 开始广告BLE服务
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

//------------------- loop---------------------
void loop() {
  int sensorValue = analogRead(inputPin);       // 读取IO3输入引脚的模拟值
  float voltage = sensorValue * (3.3 / 4095.0); // 将读取到的模拟值转换为电压值
  
  Serial.print("Input Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  

  // 并将其写入到BLE特征中
  pCharacteristic->setValue(String(voltage).c_str());
  pCharacteristic->notify(); // 发送通知

  delay(50); // 延迟50ms
}

//------------------- 函数封装--------------------
void StartMessage()
{
  Serial.begin(115200);
  while (!Serial) {
      delay(1);
  }
  Serial.println("Hello!");
}
