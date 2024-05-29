#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "images.h"  // 이미지 파일을 포함하는 헤더 파일

#define SCREEN_WIDTH 128  // OLED 디스플레이의 너비 (픽셀 단위)
#define SCREEN_HEIGHT 32  // OLED 디스플레이의 높이 (픽셀 단위)

#define OLED_RESET -1        // 리셋 핀 번호 # (아두이노 리셋 핀을 공유하는 경우 -1)
#define SCREEN_ADDRESS 0x3C  //128x64의 경우 0x3D, 128x32의 경우 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = 내부적으로 3.3V에서 디스플레이 전압 생성
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // OLED초기화 실패시, 더 이상 진행하지 않음
  }

  // 초기 디스플레이 버퍼 내용을 화면에 표시합니다.
  // 라이브러리는 이를 Adafruit 스플래시 화면으로 초기화합니다.
  display.display();
  delay(2000);  // 2초간 대기

  display.clearDisplay();       // 디스플레이 지우기

  display.drawBitmap(
    0,
    0,
    VeryGood, 32, 32, 1);  // VeryGood 이미지를 디스플레이에 그리기

  display.drawBitmap(
    32,
    0,
    Good, 32, 32, 1);  // Good 이미지를 디스플레이에 그리기

  display.drawBitmap(
    64,
    0,
    Bad, 32, 32, 1);  // Bad 이미지를 디스플레이에 그리기

  display.drawBitmap(
    96,
    0,
    VeryBad, 32, 32, 1);  // VeryBad 이미지를 디스플레이에 그리기

  display.display();
}

void loop() {
}