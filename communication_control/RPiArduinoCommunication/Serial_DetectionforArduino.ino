const int ledPin = 13;

void setup() {
   Serial.begin(9600);
   Serial.println("still alive inited");
//   pinMode(4, OUTPUT);
//   pinMode(11, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    r_funclist(Serial.read() - '0');
  }
}

void r_funclist(int number) {
   switch (number) {
      case 0: // brake

        digitalWrite(3, HIGH);
        
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(11, LOW);
        break;
      case 1: // forward (fast)

        digitalWrite(4, HIGH);

        digitalWrite(3, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(11, LOW);
        break;
      case 2: // left (fast)

        digitalWrite(5, HIGH);

        digitalWrite(4, LOW);
        digitalWrite(3, LOW);
        digitalWrite(6, LOW);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(11, LOW);
        
        break;
      case 3: // right (fast)
        digitalWrite(6, HIGH);

        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(3, LOW);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(11, LOW);
        break;
      
      case 4: // forward (slow)

        digitalWrite(9, HIGH);


        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(3, LOW);
        digitalWrite(8, LOW);
        digitalWrite(11, LOW);
        digitalWrite(6, LOW);
        break;
      case 5: // left (slow)
        digitalWrite(11, HIGH);
        

        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(3, LOW);
        digitalWrite(8, LOW);
        digitalWrite(6, LOW);
        digitalWrite(9, LOW);
        break;
      case 6: // right(slow)
        digitalWrite(8, HIGH);

        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(3, LOW);
        digitalWrite(6, LOW);
        digitalWrite(9, LOW);
        digitalWrite(11,LOW);
        
        break;
        
      default:

        
        break;
   }


}
