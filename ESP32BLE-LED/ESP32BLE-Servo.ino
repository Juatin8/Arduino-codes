/*蓝牙库*/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"  // UUID 值
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;

/*舵机库*/
#include <ESP32Servo.h>
Servo myservo; 

const int servoPin = 25;

/* 非常重要！接收到数据后的处理逻辑都写这里!!!!*/
// BLECharacteristicCallbacks 为 BLECharacteristic 的回调函数
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    uint8_t *data = pCharacteristic->getData();
    int len = pCharacteristic->getValue().empty() ? 0 : pCharacteristic->getValue().length();
    if (len > 0) {
      if (data[0] == 0x01)
        sweep();
      else
        myservo.detach();
    }
  }
  void sweep() {  //舵机旋转
    myservo.attach(servoPin);
    int pos;
    for (pos = 0; pos <= 120; pos += 3) {
      myservo.write(pos);
      delay(1);
    }
    for (pos = 120; pos >= 0; pos -= 3) {
      myservo.write(pos);
      delay(1);
    }
  }
};

void setup() {
  pinMode(servoPin, OUTPUT);
  StartMessage();  //启动消息
  delay(200);      //给芯片启动时间，然后再启动蓝牙

  SetBLEservice("MyESP32", SERVICE_UUID); //设置蓝牙
  // 创建BLE特征
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |  // 添加可读可写属性
      BLECharacteristic::PROPERTY_NOTIFY                                    // 添加可通知属性
  );

  pCharacteristic->setCallbacks(new MyCallbacks());  //当有数据写入时，调用回调函数
  pCharacteristic->setValue("Hello World");   // 设置特征的初始值

  StartBLEservice(); //启动蓝牙
}

void loop() {
  pCharacteristic->notify();  // 发送通知
  delay(500);
}


//------------------- 函数封装--------------------
void StartMessage() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  Serial.println("Hello!");
}
//
void SetBLEservice(std::string BLEname ,std::string uuid)
{
 // 创建蓝牙设备，名称
  BLEDevice::init(BLEname);
  // 创建蓝牙服务器
  pServer = BLEDevice::createServer();
  // 创建蓝牙服务
  ///BLEService *pService = pServer->createService(SERVICE_UUID);
  pService = pServer->createService(uuid);
}

void StartBLEservice()
{
  pService->start();  // 启动服务
  BLEAdvertising *pAdvertising = pServer->getAdvertising();  // 获取广播对象
  pAdvertising->start();                                     // 启动广播
}
