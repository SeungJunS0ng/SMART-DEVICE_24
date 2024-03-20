// LED 핀 번호
const int ledPin = 4; // 4는 GPTO4에 해당
void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // LED 밝기 증가
  for(int i = 0; i <= 255; i++) {
    // PWM으로 LED 밝기 변경
    analogWrite(ledPin, i);
    delay(15);
  }
  // LED 밝기 감소
  for(int i = 255; i >= 0 ; i++) {
    // PWM으로 LED 밝기 변경
    analogWrite(ledPin, i);
    delay(15);
  }                  
}
