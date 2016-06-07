const int ledPin = 13;

void setup() {
   Serial.begin(9600);
   Serial.println("still alive inited");
   pinMode(4, OUTPUT);
   pinMode(11, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    r_funclist(Serial.read() - '0');
  }
}

void r_funclist(int number) {
   switch (number) {
      case 0: // brake
        Serial.println("Received 0 from Raspi");
        digitalWrite(3, HIGH);
        delay(200);
        digitalWrite(3, LOW);
        break;
      case 1: // forward (fast)
        Serial.println("Received 1 from Raspi");
        digitalWrite(4, HIGH);
        delay(200);
        digitalWrite(4, LOW);
        break;
      case 2: // left (fast)
        Serial.println("Received 2 from Raspi");
        digitalWrite(5, HIGH);
        delay(200);
        digitalWrite(5, LOW);
        break;
      case 3: // right (fast)
        Serial.println("Received 3 from raspi");
        digitalWrite(6, HIGH);
        delay(200);
        digitalWrite(6, LOW);
        break;
      
      case 4: // forward (slow)
        Serial.println("forward slow");
        digitalWrite(9, HIGH);
        delay(200);
        digitalWrite(9, LOW);
        break;
      case 5: // left (slow)
        Serial.println("left slow");
        digitalWrite(11, HIGH);
        delay(200);
        digitalWrite(11, LOW);
        break;
      case 6: // right(slow)
        Serial.println("right slow");
        digitalWrite(8, HIGH);
        delay(200);
        digitalWrite(8, LOW);
        break;
        
      default:
        Serial.println("The Number is : " + number);
        break;
   }
}
