#include <DHT.h>

#define DHTPIN 18          // DHT11 센서에 연결된 핀 번호

// 사용 중인 센서 유형으로 변경하세요. DHT11, DHT22, DHT21
#define DHTTYPE DHT11     // DHT 센서 유형

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  delay(2000);  // 2초마다 센서 값을 읽습니다.

  // 온도 및 습도 값 읽기
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // 센서 값 출력
  Serial.print("온도: ");
  Serial.print(temperature);
  Serial.print(" °C, 습도: ");
  Serial.print(humidity);
  Serial.println(" %");
}
