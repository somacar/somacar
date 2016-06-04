int pin_9;
int pin_10;
int pin_11;

void setup() {
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  Serial.begin(9600);
}

void loop() {
  pin_9 = digitalRead(9);
  pin_10 = digitalRead(10);
  pin_11 = digitalRead(11);

  Serial.print("pin 9: ");
  Serial.print(pin_9);
  Serial.print(" / pin 10: ");
  Serial.print(pin_10);
  Serial.print(" / pin 11: ");
  Serial.println(pin_11);
  delay(300);
}
