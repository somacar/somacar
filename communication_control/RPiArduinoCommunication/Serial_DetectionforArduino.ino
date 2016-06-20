const int ledPin = 13;

int dirA1 = 2;
int dirA2 = 3;

int dirB1 = 4;
int dirB2 = 5;

int speedPin1 = 9;
int speedPin2 = 10;

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
   analogWrite(speedPin1, 255);
   analogWrite(speedPin2, 255);
   
   switch (number) {
      case 0: // brake
        digitalWrite(dirA1, LOW);
        digitalWrite(dirA2, LOW);
        digitalWrite(dirB1, LOW);
        digitalWrite(dirB2, LOW);
        
        break;
      case 1: // forward (fast)

        digitalWrite(dirA1, LOW);
        digitalWrite(dirA2, LOW);
        digitalWrite(dirB1, HIGH);
        digitalWrite(dirB2, LOW);
        
        break;
      case 2: // left (fast)

        digitalWrite(dirA1, HIGH);
        digitalWrite(dirA2, LOW);
        digitalWrite(dirB1, HIGH);
        digitalWrite(dirB2, LOW);
        
        break;
      case 3: // right (fast)
        digitalWrite(dirA1, LOW);
        digitalWrite(dirA2, HIGH);
        digitalWrite(dirB1, HIGH);
        digitalWrite(dirB2, LOW);
        
        break;

      default:
        
        break;
     
   }


}
