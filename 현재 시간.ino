#include <WiFi.h>  // WiFi 통신을 위한 라이브러리
#include <time.h>  // 시간과 관련된 함수를 위한 라이브러리

#include <Adafruit_GFX.h>      // 디스플레이를 위한 그래픽 라이브러리
#include <Adafruit_SSD1306.h>  // SSD1306 OLED 디스플레이를 위한 라이브러리
// SSD1306 디스플레이 객체 초기화
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire, -1);

const char* ssid = "Wifi5.0";          // 여기에 사용하는 WiFi 네트워크 이름 (SSID)을 입력하세요
const char* password = "102810281028";  // 여기에 사용하는 WiFi 네트워크 비밀번호를 입력하세요

int GMTOffset = 60 * 60 * 9;  // 시간 오프셋 설정, 한국은 UTC/GMT +9입니다.
int daylightOffset = 0;       // 국가에서 서머타임을 사용하는 경우 오프셋 값을 설정하세요.

const String weekDays[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };  // 요일 이름 배열

void setup() {
  Serial.begin(115200);  // 디버깅을 위한 시리얼 통신 시작

  // SSD1306 디스플레이 초기화
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 초기화 실패"));
    while (1)
      ;  // 디스플레이 초기화에 실패하면 프로그램 실행 중지
  }

  delay(2000);                  // 2초 동안 대기
  display.clearDisplay();       // 디스플레이 지우기
  display.setTextSize(1);       // 텍스트 크기를 1로 설정
  display.setCursor(0, 0);      // 커서 위치를 디스플레이 왼쪽 위 모서리로 설정
  display.setTextColor(WHITE);  // 텍스트 색상을 흰색으로 설정

  WiFi.begin(ssid, password);  // 제공된 SSID와 비밀번호를 사용하여 WiFi 네트워크에 연결
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to Wi-Fi!");
  // 시간 설정을 위한 NTP 서버 설정
  configTime(GMTOffset, daylightOffset, "pool.ntp.org", "time.nist.gov");
}


void loop() {

  // 현재 시간 가져오기
  time_t rawtime = time(nullptr);
  struct tm* timeinfo = localtime(&rawtime);

  // 시리얼 모니터에 날짜 출력
  Serial.print(timeinfo->tm_mday);
  Serial.print("/");
  Serial.print(timeinfo->tm_mon + 1);
  Serial.print("/");
  Serial.print(timeinfo->tm_year + 1900);

  Serial.print(" ");

  // 시리얼 모니터에 시간 출력
  Serial.print("Time: ");
  Serial.print(timeinfo->tm_hour);
  Serial.print(":");
  Serial.print(timeinfo->tm_min);
  Serial.print(":");
  Serial.println(timeinfo->tm_sec);

  // OLED 디스플레이 초기화
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // 시간 출력
  if (timeinfo->tm_hour < 10)
    display.print("0");
  display.print(timeinfo->tm_hour);

  display.print(":");

  if (timeinfo->tm_min < 10)
    display.print("0");
  display.print(timeinfo->tm_min);

  display.print(":");

  // 초 출력
  display.setTextSize(2);
  display.setCursor(102, 5);

  if (timeinfo->tm_sec < 10)
    display.print("0");
  display.print(timeinfo->tm_sec);

  // 날짜 출력
  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print(timeinfo->tm_mday);
  display.print("/");
  display.print(timeinfo->tm_mon + 1);
  display.print("/");
  display.print(timeinfo->tm_year + 1900);

  display.print(" ");
  display.print(weekDays[timeinfo->tm_wday]);

  // 디스플레이에 표시
  display.display();

  delay(1000);
}