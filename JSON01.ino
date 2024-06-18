#include <ArduinoJson.h>
void setup() {
  // Initialize serial port
  Serial.begin(115200);
  while (!Serial) continue;
  delay(3000);
  // JSON 문서 할당
  //
  // 괄호 안의 200은 메모리 풀의 용량(바이트)입니다.
  // JSON 문서와 일치하도록 이 값을 변경하는 것을 잊지 마십시오.
  // https://arduinojson.org/v6/assistant 를 사용하여 용량을 계산합니다.
  StaticJsonDocument<256> doc;
  // StaticJsonDocument<N>은 스택에 메모리를 할당합니다.
  // 힙에 할당하는 DynamicJsonDocument로 대체할 수 있습니다.
  // DynamicJsonDocument doc(200);
  // JSON 입력 문자열.
  char json[] = R"rawliteral({
    "token_type":"bearer",
    "access_token":"c281d73b097",
    "expires_in":43199,
    "refresh_token":"0a0c90af08f",
    "refresh_token_expires_in":5184000,
    "scope":"account_email profile"
  })rawliteral";
  Serial.println(json);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  // Fetch values.
  const char* access_token = doc["access_token"];
  const char* refresh_token = doc["refresh_token"];
  long expires_in = doc["expires_in"];
  // Print values.
  Serial.print("Access token : ");
  Serial.println(access_token);
  Serial.print("Refresh token : ");
  Serial.println(refresh_token);
  Serial.print("Expire time : ");
  Serial.println(expires_in);
}
void loop() {
  // not used in this example
}