#include <Wire.h>
#include <REG.h>
#include <wit_c_sdk.h>

#define ACC_UPDATE 0x01
#define GYRO_UPDATE 0x02
#define ANGLE_UPDATE 0x04
#define MAG_UPDATE 0x08
#define READ_UPDATE 0x80
static char s_cDataUpdate = 0, s_cCmd = 0xff;

static void AutoScanSensor(void);
static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum);
static int32_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length);
static int32_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length);
static void Delayms(uint16_t ucMs);
static void SetCommunication();

int i;
float fAcc[3], fGyro[3], fAngle[3], q[4];  //加速度、角速度、角度、四元数信息

void setup() {
  SetCommunication();  //开始串口和I2C通信
  WitInit(WIT_PROTOCOL_I2C, 0x50);
  WitI2cFuncRegister(IICwriteBytes, IICreadBytes);  //注册读写
  WitRegisterCallBack(CopeSensorData);
  WitDelayMsRegister(Delayms);
  AutoScanSensor();
}

void loop() {
  WitReadReg(AX, 13); //从AX开始，13个数据
  delay(50);  //20hz

  //if (s_cDataUpdate) {
  for (i = 0; i < 3; i++) {
    fAcc[i] = sReg[AX + i] / 32768.0f * 16.0f;       //获取加速度
    fGyro[i] = sReg[GX + i] / 32768.0f * 2000.0f;    //获取角速度
    fAngle[i] = sReg[Roll + i] / 32768.0f * 180.0f;  //获取角度
  }
  for (i = 0; i < 4; i++) {
    q[i] = sReg[q0+ i] / 32768.0f;
  }

  //   if (s_cDataUpdate & ACC_UPDATE) {
  Serial.print("acc:");
  Serial.print(fAcc[0], 3);
  Serial.print(" ");
  Serial.print(fAcc[1], 3);
  Serial.print(" ");
  Serial.print(fAcc[2], 3);
  Serial.print("\r\n");
  //    s_cDataUpdate &= ~ACC_UPDATE;
  //   }
  // if (s_cDataUpdate & GYRO_UPDATE) {
  Serial.print("gyro:");
  Serial.print(fGyro[0], 3);
  Serial.print(" ");
  Serial.print(fGyro[1], 3);
  Serial.print(" ");
  Serial.print(fGyro[2], 3);
  Serial.print("\r\n");
  //   s_cDataUpdate &= ~GYRO_UPDATE;
  //   }
  //  if (s_cDataUpdate & ANGLE_UPDATE) {
  Serial.print("angle:");
  Serial.print(fAngle[0], 3);
  Serial.print(" ");
  Serial.print(fAngle[1], 3);
  Serial.print(" ");
  Serial.print(fAngle[2], 3);
  Serial.print("\r\n");
    Serial.print("qaternion:");
  Serial.print(q[0], 3);
  Serial.print(" ");
  Serial.print(q[1], 3);
  Serial.print(" ");
  Serial.print(q[2], 3);
    Serial.print(" ");
  Serial.print(q[3], 3);
  Serial.print("\r\n");
  //   s_cDataUpdate &= ~ANGLE_UPDATE;
  //  }
  // s_cDataUpdate = 0;
  // }
}


//====================== 函数封装 =====================================
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
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);
}
