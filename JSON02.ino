#include <ArduinoJson.h>

void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  while (!Serial) continue;

  delay(3000);

   // JSON 문서 할당
   //
   // 괄호 안의 200은 이 문서에 할당된 RAM입니다.
   // 요구 사항에 맞게 이 값을 변경하는 것을 잊지 마십시오.
   // https://arduinojson.org/v6/assistant를 사용하여 용량을 계산합니다.
  StaticJsonDocument<256> doc;

   // StaticJsonObject는 스택에 메모리를 할당하고,
   // 힙에 할당하는 DynamicJsonDocument로 대체할 수 있습니다.
   //
   // DynamicJsonDocument doc(200);

   // 문서에 값 추가
  doc["object_type"] = "text";
  doc["text"] = "텍스트 영역입니다. 최대 " + String(200)+ "자 표시 가능합니다.";

  doc["link"]["web_url"]  = "https://developers.kakao.com";
  doc["link"]["mobile_web_url"] = "https://developers.kakao.com";

  doc["button_title"] = "바로 확인";

  String jsonString ;
  String jsonPretyString ;

  // 최소화된 JSON을 생성하여 직렬 포트로 보냅니다.
  //
  serializeJson(doc, jsonString);
  Serial.println(jsonString);
  // The above line prints:
  /*
   { "object_type": "text", "text": "텍스트 영역입니다. 최대 200자 표시 가능합니다.", "link": { "web_url": "https://developers.kakao.com", "mobile_web_url": "https://developers.kakao.com" }, "button_title": "바로 확인" }
  */

  // Start a new line
  Serial.println();

  // prettified JSON을 생성하고 직렬 포트로 보냅니다.
  //
  serializeJsonPretty(doc, jsonPretyString);
  Serial.println(jsonPretyString);
  /*
   {
        "object_type": "text",
        "text": "텍스트 영역입니다. 최대 200자 표시 가능합니다.",
        "link": {
            "web_url": "https://developers.kakao.com",
            "mobile_web_url": "https://developers.kakao.com"
        },
        "button_title": "바로 확인"
    }
  */
}

void loop() {
  // not used in this example
}
