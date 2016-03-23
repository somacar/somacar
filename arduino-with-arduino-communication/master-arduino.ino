int inputPin = 8;

void setup() {
  pinMode(8, OUTPUT);
  Serial.begin(9600);           // start serial for output
}

void loop() {
  digitalWrite(8, HIGH);
  delay(100);
  Serial.println("on!");
  digitalWrite(8, LOW);
  delay(100);
  Serial.println("off!");

}

