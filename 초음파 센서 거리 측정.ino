const int trigPin = 12;  // 초음파 센서의 트리거 핀
const int echoPin = 14;  // 초음파 센서의 에코 핀

long duration;          // 음파의 왕복 시간(마이크로초)
float distanceCm;       // 거리(cm)
float distanceInch;     // 거리(인치)

void setup() {
  Serial.begin(115200); // 시리얼 통신 시작 (통신 속도: 115200bps)
  pinMode(trigPin, OUTPUT);  // 트리거 핀을 출력으로 설정
  pinMode(echoPin, INPUT);   // 에코 핀을 입력으로 설정
}

void loop() {
  digitalWrite(trigPin, LOW);           // 트리거 핀 LOW로 초기화
  delayMicroseconds(2);                 // 2 마이크로초 대기
  digitalWrite(trigPin, HIGH);          // 트리거 핀 HIGH로 설정하여 초음파 송신
  delayMicroseconds(10);                // 10 마이크로초 동안 
  digitalWrite(trigPin, LOW);           // 초음파 송신 종료
  
  duration = pulseIn(echoPin, HIGH);     // 에코 핀에서 초음파의 왕복 시간 측정
  
  distanceCm = duration * 0.034/2; // 거리 계산
  
  distanceInch = distanceCm * 0.393701; // 인치로 변환
  
  Serial.print("Distance: ");
  Serial.print(distanceCm);              // 거리 (cm)
  Serial.print("(cm)  ");
  Serial.print(distanceInch);            // 거리 (인치)
  Serial.println("(inch)");
  
  delay(1000);                           // 1초 대기
}