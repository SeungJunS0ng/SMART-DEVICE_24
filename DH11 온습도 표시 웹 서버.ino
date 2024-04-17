#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// 실습 환경의 WIFI아이디와 비밀번호로 수정하세요.
const char* ssid = "네트워크_SSID";
const char* password = "네트워크_비밀번호";

#define DHTPIN 18     // DHT 센서에 연결된 디지털 핀

// 사용 중인 센서 유형으로 변경하세요. DHT11, DHT22, DHT21
#define DHTTYPE    DHT11     // 
// 온습도 센서 객체 생성
DHT dht(DHTPIN, DHTTYPE);

// 80번 포트에서 AsyncWebServer 객체 생성
AsyncWebServer server(80);

// DHT 온도 읽기 함수
String readDHTTemperature() {
  // 센서 읽기는 최대 2초가 소요될 수 있습니다. (센서가 매우 느립니다.)
  // 기본값으로 섭씨로 온도 읽기
  float t = dht.readTemperature();
  // 화씨로 온도 읽으려면 매개 변수를 true로 설정
  //float t = dht.readTemperature(true);
  // 읽기 실패 시 조기에 종료하고 다시 시도합니다.
  if (isnan(t)) {
    Serial.println("DHT 센서에서 읽기 실패!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

// DHT 습도 읽기 함수
String readDHTHumidity() {
  // 센서 읽기는 최대 2초가 소요될 수 있습니다. (센서가 매우 느립니다.)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("DHT 센서에서 읽기 실패!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

// 인덱스 HTML 페이지
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" 
    href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" 
    integrity=
    "sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" 
    crossorigin="anonymous">
  <style>
    html {
      font-family: Arial;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels {
      font-size: 1.5rem;
      vertical-align: middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="dht-labels">온도</span>
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i>
    <span class="dht-labels">습도</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function() {
  // XMLHttpRequest 객체를 사용하여 "/temperature" 엔드포인트로 GET 요청을 전송
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // 요청이 완료되고 성공적인 응답을 받았을 때, 온도 값을 업데이트합니다.
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 2000); // 2초 간격으로 업데이트합니다.

setInterval(function() {
  // XMLHttpRequest 객체를 사용하여 "/humidity" 엔드포인트로 GET 요청을 전송
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      // 요청이 완료되고 성공적인 응답을 받았을 때, 습도 값을 업데이트합니다.
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 2000); //2초 간격으로 업데이트합니다.
</script>
</html>
)rawliteral";

// 플레이스홀더를 DHT 값으로 대체하는 함수
String processor(const String& var){
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
    return String();
}

void setup(){
  // 디버깅용 시리얼 포트 설정
  Serial.begin(115200);

  dht.begin();

  // Wi-Fi 연결
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wi-Fi에 연결 중...");
}

// ESP32 로컬 IP 주소 출력
Serial.println(WiFi.localIP());

// 루트(/) 웹 페이지 라우팅
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/html", index_html, processor);
});

// "/temperature" 엔드포인트에 대한 GET 요청 처리
server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", readDHTTemperature().c_str());
});

// "/humidity" 엔드포인트에 대한 GET 요청 처리
server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", readDHTHumidity().c_str());
});

// 서버 시작
server.begin();
}

void loop(){

}