void setup() { //setup() 함수는 초기 설정을 담당합니다.
  
  pinMode(LED_BUILTIN, OUTPUT); //pinMode(LED_BUILTIN, OUTPUT); 는 내장된 LED를 출력 모드로 설정합니다. 이는 내장된 LED가 켜지거나 꺼지도록 하기 위해 필요합니다.
}

void loop() { //loop() 함수는 반복적으로 실행되는 부분입니다.
  digitalWrite(LED_BUILTIN, HIGH); //digitalWrite(LED_BUILTIN, HIGH);는 내장된 LED를 켭니다.
  delay(1000);                      //delay(1000);은 1초 동안 기다립니다.
  digitalWrite(LED_BUILTIN, LOW);  
  delay(1000);                      
