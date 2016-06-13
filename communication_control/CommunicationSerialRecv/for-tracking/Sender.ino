
int stopPin = 4;
int goPin = 5;
int rightPin = 6;
int leftPin = 7;
 
const int ReceivePin = 8;
int state = 0;
int ReceiveCount = 0; 
int LowReceiveCount = 0;

void setup() {
   Serial.begin(9600);
   Serial.println("still alive inited");
   // SET PIN OUTPUT
   pinMode(stopPin, OUTPUT);
   pinMode(goPin, OUTPUT);
   pinMode(rightPin, OUTPUT);
   pinMode(leftPin, OUTPUT);

   pinMode(ReceivePin, INPUT);
   
}

void loop() {
  
  // Serial Receiver (ttyACM0) for RPi (From tracking Process)
  if (Serial.available()) {
      r_funclist(Serial.read() - '0');
  }
  state = digitalRead(ReceivePin); // Check Arduino Pin 8 for switch Tracking Mode / Manual Mode
  
  if (state == HIGH) { // If Camera is enabled
    ReceiveCount++;
    if (ReceiveCount == 1) {
        Serial.print(8); // Send signal to enable Tracking mode
    }

    if (ReceiveCount > 1 && ReceiveCount > 100) {
       ReceiveCount = 2;
    }
    delay(1);
    
  } else { // If Tracking was disabled
    LowReceiveCount++;
    if (LowReceiveCount == 1) {
      Serial.print(9);  // Send signal to disable Tracking mode
    }

    if (LowReceiveCount > 1 && LowReceiveCount > 100 ) {
      LowReceiveCount = 2;
    }
    delay(100);
  }

//  digitalWrite(stopPin, LOW);
//  digitalWrite(goPin, HIGH);
//  digitalWrite(rightPin, LOW);
//  digitalWrite(leftPin, LOW);
//  delay(2000);
//  digitalWrite(stopPin, HIGH);
//  digitalWrite(goPin, LOW);
//  digitalWrite(rightPin, LOW);
//  digitalWrite(leftPin, LOW);
//  delay(2000);
//  digitalWrite(stopPin, LOW);
//  digitalWrite(goPin, LOW);
//  digitalWrite(rightPin, HIGH);
//  digitalWrite(leftPin, LOW);
//  delay(2000);
//  digitalWrite(stopPin, LOW);
//  digitalWrite(goPin, LOW);
//  digitalWrite(rightPin, LOW);
//  digitalWrite(leftPin, HIGH);
//  delay(2000);  
}

void r_funclist(int number) {

   switch (number) {
      case 0: // brake
        digitalWrite(stopPin, HIGH);  
        digitalWrite(goPin , LOW);
        digitalWrite(leftPin, LOW);
        digitalWrite(rightPin, LOW);      
        Serial.println("brake");
        break;
        
      case 1: // forward (fast)
        digitalWrite(goPin, HIGH);
        digitalWrite(stopPin, LOW);
        digitalWrite(leftPin, LOW);
        digitalWrite(rightPin, LOW); 
        Serial.println("forward");
        break;
        
      case 2: // left (fast)
        digitalWrite(leftPin ,HIGH);
        digitalWrite(goPin, LOW);
        digitalWrite(stopPin, LOW);
        digitalWrite(rightPin, LOW);
        Serial.println("forward");
        break;
        
      case 3: // right (fast)
        digitalWrite(rightPin, HIGH);
        digitalWrite(goPin, LOW);
        digitalWrite(stopPin, LOW);
        digitalWrite(leftPin, LOW);
        Serial.println("right");
        break;
        
      default:        
        break;
     
   }


}
