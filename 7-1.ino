#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Firebase 설정
#define FIREBASE_HOST "https://esp32-led-7b9a9-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyB_otVJETnFx0TU80oWxUtdkR6OQyZEAys"
#define WIFI_SSID "Wifi5.0"
#define WIFI_PASSWORD "102810281028"

// 내장 LED 핀 설정
#define LED_PIN 2

// Firebase 객체 생성
FirebaseData firebaseData;

FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;
unsigned long sendDataPrevMillis = 0;

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

void setup() {
  // 시리얼 통신 초기화
  Serial.begin(115200);

  // WiFi 연결
  setup_wifi();

  // API 키를 할당합니다.(필수)
  config.api_key = FIREBASE_AUTH;

  // RTDB URL을 할당합니다.(필수)
  config.database_url = FIREBASE_HOST;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("Sign up error: %s\n", config.signer.signupError.message.c_str());
  }

  // 긴 실행 시간이 필요한 토큰 생성 작업을 위한 콜백 함수를 할당합니다.
  config.token_status_callback = tokenStatusCallback; 

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // 내장 LED 핀 설정
  pinMode(LED_PIN, OUTPUT);

  // "/ledState" 경로를 0으로 초기화
  if (Firebase.ready() && signupOK)
  {
    if (Firebase.RTDB.setInt(&firebaseData, "/ledState", 0)){
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
    }
  }
}

void loop() {
  // Firebase.ready() 함수는 인증 작업을 처리하기 위해 반복적으로 호출되어야 합니다.
  if (Firebase.ready() && signupOK && \
  (millis() - sendDataPrevMillis > 2000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    // Firebase에서 LED 상태 가져오기
    int ledState = 0;
    if (Firebase.RTDB.getInt(&firebaseData, "/ledState")) {
      if (firebaseData.dataType() == "int") {
        ledState = firebaseData.intData();
        Serial.println(ledState);
        if (ledState == 1) {
          digitalWrite(LED_PIN, HIGH);
        } else {
          digitalWrite(LED_PIN, LOW);
        }
      }
    }
    else {
      Serial.println(firebaseData.errorReason());
    }
  }
}
