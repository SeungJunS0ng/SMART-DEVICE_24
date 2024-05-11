const int buzzerPin = 2; //led 핀 번호 설절
 
void setup (){
  pinMode (buzzerPin,OUTPUT );//buzzerPin 을 출력으로 설정
}
 
void loop (){
  digitalWrite (buzzerPin, HIGH ); //buzzerPin 에 HIGH 값 쓰기
  delay (500 ); // 0.5 초 기다리기
  digitalWrite (buzzerPin, LOW ); //buzzerPin 에 LOW 값 쓰기
  delay (1000 ); // 1 초 기다리기
}