#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <REG.h>
#include <wit_c_sdk.h>

#define ACC_UPDATE 0x01
#define GYRO_UPDATE 0x02
#define ANGLE_UPDATE 0x04
#define MAG_UPDATE 0x08
#define READ_UPDATE 0x80

#define SDA_PIN 4 // 自定义SDA引脚
#define SCL_PIN 5 // 自定义SCL引脚

static char s_cDataUpdate = 0, s_cCmd = 0xff;

static void AutoScanSensor(void);
static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum);
static int32_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length);
static int32_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length);
static void Delayms(uint16_t ucMs);
static void SetCommunication();

// 设置数据结构体
typedef struct struct_message {
  int board_Num;
  float a1;
  float a2;
  float a3;
  float g1;
  float g2;
  float g3;
  float an1;
  float an2;
  float an3;
} struct_message;

struct_message myData;

uint8_t broadcastAddress[] = { 0xA8, 0x42, 0xE3, 0xAB, 0xB0, 0xF4 };  // 接收设备的 MAC 地址

int i;
float fAcc[3], fGyro[3], fAngle[3];  //加速度、角速度、角度、四元数信息

void setup() {

  //------------传感器设置-------------------
  SetCommunication();  //开始串口和I2C通信
  WitInit(WIT_PROTOCOL_I2C, 0x50);
  WitI2cFuncRegister(IICwriteBytes, IICreadBytes);  //注册读写
  WitRegisterCallBack(CopeSensorData);
  WitDelayMsRegister(Delayms);
  AutoScanSensor();

  //============ ESPNOW设置=============
  initiateESPNOW();
  esp_now_register_send_cb(OnDataSent);  // 设置发送数据回调函数

  // 绑定数据接收端
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.ifidx = WIFI_IF_STA;
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // 检查设备是否配对成功
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}


void loop() {
  WitReadReg(AX, 13);  //从AX开始，13个数据

  for (i = 0; i < 3; i++) {
    fAcc[i] = sReg[AX + i] / 32768.0f * 16.0f;       //获取加速度
    fGyro[i] = sReg[GX + i] / 32768.0f * 2000.0f;    //获取角速度
    fAngle[i] = sReg[Roll + i] / 32768.0f * 180.0f;  //获取角度
  }

  myData.board_Num = 1;
  myData.a1 = fAcc[0];
  myData.a2 = fAcc[1];
  myData.a3 = fAcc[2];
  myData.g1 = fGyro[0];
  myData.g2 = fGyro[1];
  myData.g3 = fGyro[2];
  myData.an1 = fAngle[0];
  myData.an2 = fAngle[1];
  myData.an3 = fAngle[2];

  SendData_CheckResult();  // 发送数据
  delay(50);               //20hz
}


//---------------- ESPNOW函数封装----------------------
// 数据发送回调函数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println();
}

void initiateESPNOW() {
  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
}

void SendData_CheckResult()  //检查发送结果
{
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}



//====================== wit 函数封装 =====================================
static int32_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length)  //读I2C数据
{
  int val;
  Wire.beginTransmission(dev);
  Wire.write(reg);
  Wire.endTransmission(false);          //endTransmission but keep the connection active
  val = Wire.requestFrom(dev, length);  //Ask for bytes, once done, bus is released by default
  if (val == 0) return 0;
  for (int x = 0; x < length; x++) data[x] = Wire.read();
  return 1;
}

static int32_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length)  // 写I2C数据
{
  Wire.beginTransmission(dev);
  Wire.write(reg);
  Wire.write(data, length);
  Wire.endTransmission();  //Stop transmitting
  return 1;
}

static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum) {
  int i;
  for (i = 0; i < uiRegNum; i++) {
    switch (uiReg) {
      case AZ:
        s_cDataUpdate |= ACC_UPDATE;
        break;
      case GZ:
        s_cDataUpdate |= GYRO_UPDATE;
        break;
      case HZ:
        s_cDataUpdate |= MAG_UPDATE;
        break;
      case Yaw:
        s_cDataUpdate |= ANGLE_UPDATE;
        break;
      default:
        s_cDataUpdate |= READ_UPDATE;
        break;
    }
    uiReg++;
  }
}

static void Delayms(uint16_t ucMs) {
  delay(ucMs);
}


static void AutoScanSensor(void)  //扫描I2C上有无传感器
{
  int i, iRetry;
  for (i = 0; i < 0x7F; i++) {
    WitInit(WIT_PROTOCOL_I2C, i);
    iRetry = 2;
    do {
      s_cDataUpdate = 0;
      WitReadReg(AX, 3);
      delay(5);
      if (s_cDataUpdate != 0) {
        Serial.print("find 0x");
        Serial.print(i, HEX);
        Serial.print(" addr sensor\r\n");
        return;
      }
      iRetry--;
    } while (iRetry);
  }
  Serial.print("can not find sensor\r\n");
  Serial.print("please check your connection\r\n");
}

static void SetCommunication() {
  Wire.begin(SDA_PIN,SCL_PIN);
  Wire.setClock(400000);
  Serial.begin(115200);
}
