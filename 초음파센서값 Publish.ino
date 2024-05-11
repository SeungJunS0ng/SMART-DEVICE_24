#include <WiFi.h>          //Wi-Fi 연결 관련 라이브러리
#include <PubSubClient.h>  //MQTT 프로토콜을 사용하기 위한 라이브러리

// 다음 변수들을 당신의 SSID와 비밀번호로 대체하세요.
const char* ssid = "Your_SSID";          // 사용하는 WiFi 네트워크 이름 (SSID)
const char* password = "Your_Password";  // 사용하는 WiFi 네트워크 비밀번호

const int trigPin = 12;  // 초음파 센서의 트리거 핀
const int echoPin = 14;  // 초음파 센서의 에코 핀

// MQTT 브로커 IP 주소를 여기에 입력하세요 (예: "192.168.1.144")
const char* mqtt_server = "test.mosquitto.org";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsgTime = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqttPort);

  pinMode(echoPin, INPUT);  // 에코 핀을 입력으로 설정
  // 트리거 핀 초기화
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
}

void setup_wifi() {
  delay(10);
  // Wi-Fi 네트워크에 연결 시작
  Serial.println();
  Serial.print("연결 중인 Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi 연결됨");
  Serial.println("IP 주소: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // 연결이 될 때까지 반복
  while (!client.connected()) {
    Serial.print("MQTT 연결 시도 중...");
    // 랜덤 클라이언트 ID 생성
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    Serial.print("클라이언트 ID: ");
    Serial.println(clientId);
    // 연결 시도
    if (client.connect(clientId.c_str())) {
      Serial.println("연결됨");
    } else {
      Serial.print("실패, rc=");
      Serial.print(client.state());
      Serial.println(" 5초 후 다시 시도");
      // 5초 대기 후 다시 시도
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  // 클라이언트가 메시지를 처리하고 서버와 연결 유지
  client.loop();

  long now = millis();
  if (now - lastMsgTime > 1000) {  //1초 간격
    lastMsgTime = now;
    // 초음파 센서 값을 읽어옵니다.
    float sensorValue = readUltrasonicSensor();
    char sensorString[8];
    dtostrf(sensorValue, 1, 2, sensorString);
    client.publish("user1/esp32/ultra", sensorString);
  }
}

float readUltrasonicSensor() {
  // 트리거 핀을 10 마이크로초 동안 HIGH로 설정하여 초음파를 발사합니다.
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 에코 핀에서 펄스의 지속 시간을 측정합니다.
  pinMode(echoPin, INPUT);
  float duration = pulseIn(echoPin, HIGH);

  // 소리의 속도를 기준으로 거리를 계산합니다.(343m/s로 가정)
  float distance = duration * 0.0343 / 2.0;

  return distance;
}
