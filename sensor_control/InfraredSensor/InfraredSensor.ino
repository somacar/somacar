int v_analogPin = A0;
int v_analogValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  v_analogValue = analogRead(v_analogPin);
  Serial.println(v_analogValue);
  delay(200);
}
