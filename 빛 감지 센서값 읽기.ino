#include <WiFi.h>
#include <FirebaseESP32.h>

// 토큰 생성 프로세스 정보 제공
#include "addons/TokenHelper.h"
// RTDB 페이로드 출력 정보 및 기타 도움 함수 제공
#include "addons/RTDBHelper.h"

// Firebase 설정
#define FIREBASE_HOST "https://esp32-light-sensor-1d4ac-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyBdCib1Kf28yUcY481joIVVpnY1kdxUSf4"
#define WIFI_SSID "Wifi5.0"
#define WIFI_PASSWORD "102810281028"


// Firebase 객체 정의
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;

// 데이터베이스 주 경로
String databasePath = "/room1";
String sensorPath = "/lightsensor";
String timePath = "/timestamp";

/// 부모 노드 (현재 시간 정보로 매 루프마다 업데이트)
String parentPath;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

// 빛 센서 핀
int sensorPin = 34;

// 타이머 변수 (새로운 측정 값을 20초마다 전송)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000;  //1 minutes

// WiFi 초기화
void setup_wifi() {
  delay(10);
  // Wi-Fi 네트워크에 연결 시작
  Serial.println();
  Serial.print("연결 중인 Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi 연결됨");
  Serial.println("IP 주소: ");
  Serial.println(WiFi.localIP());
}
// 현재 epoch 시간을 가져오는 함수
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void setup() {
  Serial.begin(115200);

  // WiFi 연결
  setup_wifi();
  configTime(0, 0, ntpServer);

  // API 키 할당 (필수)
  config.api_key = FIREBASE_AUTH;

  // RTDB URL 할당 (필수)
  config.database_url = FIREBASE_HOST;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  /* 회원 가입 */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // 장기 실행 토큰 생성 작업에 대한 콜백 함수 할당
  config.token_status_callback = tokenStatusCallback;

  // 토큰 생성 최대 재시도 횟수 할당
  config.max_token_generation_retry = 5;

  // Firebase 인증 및 설정과 함께 라이브러리 초기화
  Firebase.begin(&config, &auth);
}

void loop() {

  // Send new readings to database
  if (Firebase.ready() && signupOK
      && (millis() - sendDataPrevMillis > timerDelay
          || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // 현재 타임스탬프 가져오기
    timestamp = getTime();
    Serial.print("time: ");
    Serial.println(timestamp);

    parentPath = databasePath + "/" + String(timestamp);

    json.set(sensorPath.c_str(), String(analogRead(sensorPin)));
    json.set(timePath, String(timestamp));
    Serial.println("Set json... ");
    if (Firebase.setJSON(fbdo, parentPath.c_str(), json))
      Serial.println("ok");
    else
      Serial.println(fbdo.errorReason());
  }
}
