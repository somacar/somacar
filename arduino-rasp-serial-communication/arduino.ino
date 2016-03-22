
const int ledPin = 13;

void setup() {
   Serial.begin(9600);
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
        break;
      case 1:
        
        Serial.println("Received 1 from Raspi");
        break;
      default:
        Serial.println("The Number is : " + number);
        break;
   }
}
