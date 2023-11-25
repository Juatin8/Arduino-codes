#include <WiFi.h>
#include <esp_now.h>

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

void setup() {
  Serial.begin(9600);
  initiateESPNOW();
  // 设置接收数据回调函数
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}


//------------------- 函数封装 -----------------
// 数据接收回调函数
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print(myData.board_Num);
  Serial.print(":");
  Serial.print(myData.a1);
  Serial.print(":");
  Serial.print(myData.a2);
  Serial.print(":");
  Serial.print(myData.a3);
  Serial.print(":");
  Serial.print(myData.g1);
  Serial.print(":");
  Serial.print(myData.g2);
  Serial.print(":");
  Serial.print(myData.g3);
  Serial.print(":");
  Serial.print(myData.an1);
  Serial.print(":");
  Serial.print(myData.an2);
  Serial.print(":");
  Serial.print(myData.an3);
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
