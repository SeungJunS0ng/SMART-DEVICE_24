#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);

  // OLED 디스플레이 초기화
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 오류");
    while (true);
  }

  // OLED 디스플레이 클리어
  display.clearDisplay();
}

void loop() {
  // "Hello, World!"를 크기가 다른 세 개의 텍스트로 표시
  display.clearDisplay();
  
  // 크기 6의 폰트로 텍스트 표시
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("SEUNG JUN SONG");

  // 크기 8의 폰트로 텍스트 표시
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 15);
  display.println("202110100");

  display.display();
  delay(2000);
}
