#include "LedControl.h"
#define DIN 5
#define CS  4
#define CLK 3


LedControl lc = LedControl(DIN, CLK, CS, 1); //实例化一个LedControl,最后一个是LED设备数量，有的时候是多个设备联级的。


void setup() {
  lc.shutdown(0, false); //第一个参数表示第几个设备，
  lc.setIntensity(0, 1); //亮度显示，范围是0-15
  lc.clearDisplay(0); //清除LED显示
}


void LightMatrix() {
  byte a[8] = {0x18, 0x3C, 0x7E, 0x7E, 0xFF, 0xFF, 0xFF, 0x66}; //某个字母的显示信息


  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, a[i]);
  }
delay(1000);
for (int j = 0; j < 8; j++)
{
  for (int k = 0; k < 8; k++)
  { lc.setRow(0, k, a[k] << j);
  }
  delay(1000);
}
}


void loop() {
  LightMatrix();
}
