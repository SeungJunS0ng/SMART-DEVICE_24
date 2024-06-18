#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Your_SSID";          // 사용하는 WiFi 네트워크 이름 (SSID)
const char* password = "Your_Password";  // 사용하는 WiFi 네트워크 비밀번호

const String rest_api_key = "REST API KEY";
String access_token = "Access token";
String refresh_token = "Refresh token";

#define MsgSendInterval 3600  // 60 * 60 초, 즉 한시간 간격으로 전송
long timeout = 3600;          //시간을 초로 나타냄
int sensorValue = 0;
int sensorPin = 34;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nConnected to WiFi : ");
  Serial.println(WiFi.localIP());
}
void loop() {
  if (timeout++ > MsgSendInterval)  // 1시간(60 * 60)에 1번씩 전송
  {
    if (isAccessTokenExpired() == true) {
      if (update_access_token() == false) {
        Serial.println("Access token update failed");
      }
    }
    sensorValue = analogRead(sensorPin);
    send_message();
    timeout = 0;
  }

  delay(1000);
}


// str문자열에서 start_string와 end_string사이의 문자열을 추출하는 함수
/*
String extract_string(String str, String start_string, String end_string)
{
  int index1 = str.indexOf(start_string) + start_string.length();
  int index2 = str.indexOf(end_string, index1);
  String value = str.substring(index1, index2);
  return value;
}
*/

bool isAccessTokenExpired() {
  HTTPClient http;
  bool returnVal = true;

  StaticJsonDocument<100> doc;
  /*
curl -v -X GET "https://kapi.kakao.com/v1/user/access_token_info" \
  -H "Authorization: Bearer ${ACCESS_TOKEN}"
*/
  if (!http.begin("https://kapi.kakao.com/v1/user/access_token_info")) {
    Serial.println("\nfailed to begin http\n");
  }
  http.addHeader("Authorization", "Bearer " + access_token);

  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, payload);
      long expire_time = doc["expires_in"];
      Serial.println(expire_time);
      if (expire_time > 0) {
        returnVal = false;
      } else {
        returnVal = true;
      }
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n",
                  http.errorToString(httpCode).c_str());
  }
  http.end();
  return returnVal;
}

void send_message() {
  HTTPClient http;

  StaticJsonDocument<300> doc;

  String url = "https://kapi.kakao.com/v2/api/talk/memo/default/send";
  if (!http.begin(url)) {
    Serial.println("\nfailed to begin http\n");
  }

  http.addHeader("Authorization", "Bearer " + access_token);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int http_code;
  /* 카카오톡 default 템플릿
   template_object={
        "object_type": "text",
        "text": "텍스트 영역입니다. 최대 200자 표시 가능합니다.",
        "link": {
            "web_url": "https://developers.kakao.com",
            "mobile_web_url": "https://developers.kakao.com"
        },
        "button_title": "바로 확인"
    }
    */
  doc["object_type"] = "text";
  doc["text"] = "토양 센서 값 :" + String(sensorValue);
  //doc["link"]["web_url"] = "https://www.naver.com";
  JsonObject obj = doc.createNestedObject();
  doc["link"] = obj;

  String data;
  // Generate the minified JSON and send it to the Serial port.
  //
  serializeJson(doc, data);
  Serial.println(data);

  http_code = http.POST("template_object=" + data);
  Serial.print("HTTP Response code: ");
  Serial.println(http_code);

  String response;
  if (http_code > 0) {
    response = http.getString();
    Serial.println(response);
  }

  http.end();
}

/*
curl -v -X POST "https://kauth.kakao.com/oauth/token" \
 -H "Content-Type: application/x-www-form-urlencoded" \
 -d "grant_type=refresh_token" \
 -d "client_id=${REST_API_KEY}" \
 -d "refresh_token=${USER_REFRESH_TOKEN}"
*/
bool update_access_token() {
  HTTPClient http;
  bool retVal = false;
  StaticJsonDocument<400> doc;

  String url = "https://kauth.kakao.com/oauth/token";
  String nrefresh_token = "";
  if (!http.begin(url)) {
    Serial.println("\nfailed to begin http\n");
  }
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int http_code;
  String data = "grant_type=refresh_token&client_id=" + rest_api_key + "&refresh_token=" + refresh_token;
  Serial.println(data);
  http_code = http.POST(data);
  Serial.print("HTTP Response code: ");
  Serial.println(http_code);

  String response;
  if (http_code > 0) {
    response = http.getString();
    Serial.println(response);

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, response);

    const char* atoken = doc["access_token"];
    const char* rtoken = doc["refresh_token"];

    Serial.print("Access token : ");
    Serial.println(atoken);
    Serial.print("New refresh token : ");
    Serial.println(rtoken);

    access_token = atoken;
    //만료 1개월전부터 갱신되므로 data가 없을 수도 있음
    if (rtoken != NULL) {
      refresh_token = rtoken;
    }

    retVal = true;
  } else {
    retVal = false;
  }
  http.end();
  return retVal;
}
