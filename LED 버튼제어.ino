const int buttonPin = 33; // 버튼 핀의 번호
const int ledPin = 4; // 4는 GPTO4에 해당
int buttonState = 0; // 버튼 상태를 읽기 위한 변수

void setup() {
  // LED 핀을 출력으로 초기화
  pinMode(ledPin, OUTPUT);
  // 푸시버튼 핀을 입력으로 초기화
  pinMode(buttonPin, INPUT);
}

void loop() {
  // 버튼의 상태를 읽습니다.
  buttonState = digitalRead(buttonPin);
  // 버튼이 눌렸는지 확인합니다. 그렇다면 buttonState는 Low입니다.
  if(buttonState == LOW) {
    //LED 켜기
    digitalWrite(ledPin, HIGH);
  } else {
    // LED 끄기
    digitalWrite(ledPin, LOW);
  }                  
}
