const int ledPin = 13;

void setup() {
   Serial.begin(9600);
   Serial.println("still alive inited");
   pinMode(4, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    r_funclist(Serial.read() - '0');
  }
}

void r_funclist(int number) {
   switch (number) {
      case 0:
        Serial.println("Received 0 from Raspi");
        digitalWrite(4, HIGH);
        delay(1000);
        digitalWrite(4, LOW);
        break;
      case 1:
        Serial.println("Received 1 from Raspi");
        digitalWrite(4, HIGH);
        delay(1000);
        digitalWrite(4, LOW);
        break;
      case 2:
        Serial.println("Received 2 from Raspi");
        digitalWrite(4, HIGH);
        delay(1000);
        digitalWrite(4, LOW);
        break;
      default:
        Serial.println("The Number is : " + number);
        break;
   }
}
