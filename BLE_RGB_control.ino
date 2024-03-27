//라이브러리를 가져옵니다.
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
//서비스와 특성에 대한 UUID를 정의합니다.
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define RGB_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define NOTYFY_CHARACTERISTIC_UUID "d501d731-f0f9-4121-a7f6-2c0f959f1583"
//RGB LED에 사용될 핀 번호를 정의합니다
#define RED_PIN   4
#define GREEN_PIN 15
#define BLUE_PIN  2
//BLE 관련 객체를 선언합니다.
BLEServer *pServer; //BLE 서버 객체
BLEService *pService; // BLE 서비스 객체
BLECharacteristic *pRGBCharacteristic = NULL; //BLE 특성 객체
BLECharacteristic* pCharacteristic = NULL; //BLE 특성 객체
//RGB 값을 저장할 변수들을 초기화합니
int redValue = 0;
int greenValue = 0;
int blueValue = 0;
//BLE 특성에 대한 콜백 클래스를 정의합니다.
class MyCallbacks : public BLECharacteristicCallbacks {
  //특성의 값을 읽어와 RGB 값을 파싱하고 LED를 제어합니다.
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue().c_str(); //특성의 값을 문자열로 읽어옵니다.

    // 값이 유효한 경우에만 처리
    if (value.length() > 0) {
      // RGB 값을 파싱하여 각각의 변수에 저장
      int delimiterPos1 = value.indexOf(',');
      int delimiterPos2 = value.lastIndexOf(',');

      if (delimiterPos1 != -1 && delimiterPos2 != -1 \
             && delimiterPos2 < value.length() - 1) {
        redValue = value.substring(0, delimiterPos1).toInt();
        greenValue = value.substring(delimiterPos1 + 1, delimiterPos2).toInt();
        blueValue = value.substring(delimiterPos2 + 1).toInt();

        // RGB 값을 LED의 밝기로 변환하여 제어합니다.
        analogWrite(RED_PIN, 255 - redValue);
        analogWrite(GREEN_PIN, 255 - greenValue);
        analogWrite(BLUE_PIN, 255 - blueValue);
      }
    }
  }
};

void setup() {
  // BLE 초기화하고 이름을 "RGB LED Control"로 설정합니다
  BLEDevice::init("RGB LED Control_seungjun");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  
  // RGB 특성 생성 및 설정
  pRGBCharacteristic = pService->createCharacteristic(
    RGB_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );  
  pRGBCharacteristic->setCallbacks(new MyCallbacks());

  // Notify/Indicate 특성 생성 및 설정
  pCharacteristic = pService->createCharacteristic(
                      NOTYFY_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
    // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // 서비스를 서버에 추가
  pService->start();

  // BLE 광고 시작
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

    // 모든 핀을 HIGH로 설정하여 LED를 꺼진 상태로 초기화합니다.
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void loop() {
  // RGB 값을 16진수 형태로 변환하여 Notify
  char hexValue[8]; 
  sprintf(hexValue, "#%02X%02X%02X", redValue, greenValue, blueValue);
  //Notify 특성의 값을 hexValue로 설정합니다.
  pCharacteristic->setValue(hexValue); 
  pCharacteristic->notify(); //Notify를 통해 값을 전달합니다.

  delay(100);  // 알림 간격을 조절할 수 있습니다.
}
