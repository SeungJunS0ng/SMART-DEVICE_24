#include <WiFi.h>
#include <PubSubClient.h>
 
// 다음 변수들을 당신의 SSID와 비밀번호로 대체하세요.
const char* ssid = "여기에_당신의_SSID_입력";
const char* password = "여기에_당신의_비밀번호_입력";

// MQTT 브로커 IP 주소를 여기에 입력하세요 (예: "192.168.1.144")
const char* mqtt_server = "test.mosquitto.org";
const int mqttPort = 1883 ;
const char * mqttTopic = "user1/esp32/buzzer"; // 사용자에 맞게 변경
const int buzzerPin = 2 ; // 부저에 연결된 GPIO 핀 번호
 
WiFiClient espClient;
PubSubClient client(espClient);

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
 
void setup(){
    // 시리얼 통신 초기화
  Serial.begin(115200);
  // Wi-Fi 연결 설정
  setup_wifi();

  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
 
  // 부저 핀을 출력 모드로 설정
  pinMode(buzzerPin,OUTPUT);
}
 
void loop(){
  if(!client.connected()){
    reconnect();
  }
  client.loop();//MQTT 클라이언트를 유지하기 위해 호출
}
 
void callback(char*topic , byte *payload , unsigned int length){
  Serial.print("Receivedmessage: ");
  Serial.print(topic);
  Serial.print(" ");
  for(int i = 0 ; i <length;i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
 
  if(strcmp(topic, mqttTopic)== 0){
    if(payload[0] == '1'){
     // 부저를 켜는 코드 작성
     digitalWrite(buzzerPin, HIGH); 
    }else if(payload[0] == '0'){
     // 부저를 끄는 코드 작성
     digitalWrite(buzzerPin, LOW); 
    }
  }
}
 
void reconnect(){
  while(!client.connected()){
    Serial.print("Connectingto MQTT Broker...");
   String clientId = "ESP32Client-";
   clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())){
     Serial.println("Connected to MQTT Broker");
     client.subscribe(mqttTopic);
    }else {
     Serial.print("Failed, rc=");
     Serial.print(client.state());
     Serial.println("Retrying in 5 seconds...");
     delay(5000);
    }
  }
}