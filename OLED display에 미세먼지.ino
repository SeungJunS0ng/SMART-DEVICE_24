#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>
#include <time.h>

#include "images.h"  // 이미지 파일을 포함하는 헤더 파일

const char* ssid = "Wifi5.0";          // 사용하는 WiFi 네트워크 이름 (SSID)
const char* password = "102810281028";  // 사용하는 WiFi 네트워크 비밀번호
const int httpPort = 80;

const char* apiKey = "to3tPztQheE1LjCE%2BXYxxQutrDEQVVkbycZTltv7DFfIlcIzjPCBxy6pHUvzZQc8yTXpXac1XEx2r5bs0iDYzg%3D%3D";
const char* version = "&ver=1.3";
const char* server = "apis.data.go.kr";
const char* stationName = "종로구";
const char* returnType = "xml";  //or json
const char* numOfRows = "1";
const char* pageNo = "1";
const char* dataTerm = "DAILY";

WiFiClient client;

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

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWiFi is connected");

  display.clearDisplay();       // 디스플레이 지우기
  display.setTextSize(1);       // 텍스트 크기를 1로 설정
  display.setCursor(0, 0);      // 커서 위치를 디스플레이 왼쪽 위 모서리로 설정
  display.setTextColor(WHITE);  // 텍스트 색상을 흰색으로 설정

  display.drawBitmap(
    0,
    0,
    VeryGood, 32, 32, 1);  // VeryGood 이미지를 디스플레이에 그리기

  display.display();
}

void loop() {
  String a[3];
  int i = 0;
  String dateNtime;
  String pm10Val;
  String pm10Grade;
  String tmp_str;
  static int IntervalReq = 1800;  // 30분을 초단위로 계산

  if (IntervalReq++ > 1800) {  //30분 간격으로 data요청
    IntervalReq = 0;
    Requesthttp();
  };

  delay(50);
  while (client.available()) {

    String line = client.readStringUntil('\n');
    Serial.println(line);

    i = line.indexOf("</dataTime>");
    if (i > 0) {
      tmp_str = "<dataTime>";
      dateNtime = line.substring(line.indexOf(tmp_str) + tmp_str.length(), i);
      Serial.println(dateNtime);
    }

    i = line.indexOf("</pm10Value>");

    if (i > 0) {
      tmp_str = "<pm10Value>";
      pm10Val = line.substring(line.indexOf(tmp_str) + tmp_str.length(), i);
      Serial.println(pm10Val);
    }

    i = line.indexOf("</pm10Grade>");

    if (i > 0) {
      tmp_str = "<pm10Grade>";
      pm10Grade = line.substring(line.indexOf(tmp_str) + tmp_str.length(), i);
      Serial.println(pm10Grade);
      client.stop();
      display.clearDisplay();
      displayIcon(atoi(pm10Grade.c_str()));  // 등급에 따라 해당 아이콘 그리기
      displayString(dateNtime, pm10Val);     // 날짜 및 시간과 미세먼지 농도 표시
      display.display();
      break;
    }
  }

  delay(1000);
}

void Requesthttp() {
  if (client.connect(server, httpPort)) {
    Serial.println("\nSuccessed connection, and request http protocol");

    // HTTP 요청을 보냄
    client.print(String("Get /B552584/ArpltnInforInqireSvc"));
    client.print(String("/getMsrstnAcctoRltmMesureDnsty?serviceKey="));
    client.print(String(apiKey));
    client.print(String("&returnType=") + String(returnType));
    client.print(String("&numOfRows=") + String(numOfRows));
    client.print(String("&pageNo=") + String(pageNo));
    // URL 인코딩하여 문자열 전송
    client.print(String("&stationName=") + urlencode(String(stationName)));
    client.print(String("&dataTerm=") + String(dataTerm));
    client.print(String(version));
    client.print(String(" HTTP/1.1\r\n"));
    client.print(String("Host: ") + String(server) + String("\r\n"));
    client.print(String("Connection: close\r\n"));
    client.print(String("\r\n\r\n"));
  } else {
    Serial.println("\nfailed connection");
  }
}

void displayString(String dnt, String pmval) {
  display.setCursor(50, 3);
  display.println(dnt.substring(0, 10));  // 날짜를 디스플레이에 출력
  display.setCursor(50, 13);
  display.println(dnt.substring(11));  // 시간을 디스플레이에 출력
  display.setCursor(50, 23);
  display.print(pmval);
  display.println(" ug/m^3");  // 미세먼지 농도를 디스플레이에 출력
}

void displayIcon(int grade) {
  switch (grade) {
    case 1:
      display.drawBitmap(
        0,
        0,
        VeryGood, 32, 32, 1);  // VeryGood 아이콘을 디스플레이에 그리기
      break;
    case 2:
      display.drawBitmap(
        0,
        0,
        Good, 32, 32, 1);  // Good 아이콘을 디스플레이에 그리기
      break;
    case 3:
      display.drawBitmap(
        0,
        0,
        Bad, 32, 32, 1);  // Bad 아이콘을 디스플레이에 그리기
      break;
    case 4:
      display.drawBitmap(
        0,
        0,
        VeryBad, 32, 32, 1);  // VeryBad 아이콘을 디스플레이에 그리기
      break;
  }
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
  }
  return encodedString;
}