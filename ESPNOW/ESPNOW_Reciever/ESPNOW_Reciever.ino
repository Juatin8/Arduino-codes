#include <WiFi.h>
#include <esp_now.h>

// 设置数据结构体
typedef struct struct_message {
  String board_name;
  float a1;
  float a2;
  float a3;
  float g1;
  float g2;
  float g3;
  float t;
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
  Serial.print("board_name: ");
  Serial.println(myData.board_name);
  Serial.print("a1:");
  Serial.print(myData.a1);
  Serial.print("a2:");
  Serial.print(myData.a2);
  Serial.print("a3:");
  Serial.print(myData.a3);
  Serial.print("g1:");
  Serial.print(myData.g1);
  Serial.print("g2:");
  Serial.print(myData.g2);
  Serial.print("g3:");
  Serial.print(myData.g3);
  Serial.print("t:");
  Serial.print(myData.t);
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
