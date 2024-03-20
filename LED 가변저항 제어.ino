// 가변저항 모듈은 GPIO 34(아날로그 ADC!_CH6)에 연결
const int potPin = 34;
// LED 모듈은 GPIO 4에 연결
const int ledPin = 4; 
// 가변저항 값을 저장하기 위한 변수
int potValue = 0; 

void setup() {
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 가변저항 값을 읽음
  potValue = analogRead(potPin);
  analogWrite(ledPin, map(potValue, 0, 4095, 0, 255));
  delay(100);
  }                  
