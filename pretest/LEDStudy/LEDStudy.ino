int LedPin2 = 2;

void setup() {
  pinMode(LedPin2, OUTPUT);
}

void loop() {
  digitalWrite(LedPin2,HIGH);
  delay(1000);
  digitalWrite(LedPin2,LOW);
  delay(1000);
}
