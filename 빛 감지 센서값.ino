int sensorPin = 34;
int value = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  value = analogRead(sensorPin);

  Serial.println(value);

  delay(500);
}