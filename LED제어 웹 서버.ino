// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>


// Replace with your network credentials
const char* ssid = "John";
const char* password = "102810281028";

// 현재 출력 상태를 저장하는 보조 변수
String output2State = "off";

// GPIO 핀에 출력 변수 할당
const int output2 = 2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html { font-family: Helvetica; display: inline-block; 
            margin: 0px auto; text-align: center; }
    .button { background-color: #4CAF50; border: none; 
              color: white; padding: 16px 40px; text-decoration: none; 
              font-size: 30px; margin: 2px; cursor: pointer; }
    .button2 { background-color: #555555; }
  </style>
</head>
<body>
  <h1>ESP32 웹 서버</h1>
  <p>GPIO 2 - 상태 %STATE%</p>
  %BUTTON%
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
//Serial.println(var);
  if(var == "STATE")
  {
    return output2State;
  }
  if(var == "BUTTON"){
    String buttons = "";
    
    if (output2State == "off")
    {
      buttons += "<p><a href=\"/2/on\"> \
                  <button class=\"button\">ON</button></a></p>";
    }
    else
    {
      buttons += "<p><a href=\"/2/off\"> \
                  <button class=\"button button2\">OFF</button></a></p>";
    }
    return buttons;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  delay(3000);

  // 출력 변수를 출력으로 초기화
  pinMode(output2, OUTPUT);
  // 출력을 LOW로 설정
  digitalWrite(output2, LOW);
  
  // SSID와 비밀번호로 Wi-Fi 네트워크에 연결
  Serial.print("연결 중: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // 로컬 IP 주소 출력 및 웹 서버 시작
  Serial.println("");
  Serial.println("Wi-Fi 연결됨.");
  Serial.println("IP 주소: ");
  Serial.println(WiFi.localIP());
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/2/on 
  server.on("/2/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("GPIO 2 ON");
    output2State = "on";
    digitalWrite(output2, HIGH);
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/2/off
  server.on("/2/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("GPIO 2 OFF");
    output2State = "off";
    digitalWrite(output2, LOW);
    request->send_P(200, "text/html", index_html, processor);
  });
  
  // Start server
  server.begin();
}

void loop() {

}