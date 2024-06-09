/*
# 0 ~1500 건조한 토양
# 1500~2000 정상 습토
# 2000이상 물속
*/
const int soilSensorPin = 34;

void setup(){
 Serial.begin(115200);
}

void loop(){
 Serial.print("토양 습도 센서 값:");
 Serial.println(analogRead(soilSensorPin));
 delay(1000);
}