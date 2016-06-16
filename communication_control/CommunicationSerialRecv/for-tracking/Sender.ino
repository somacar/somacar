// Set Motor pin
int stopPin = 4;
int goPin = 5;
int rightPin = 6;
int leftPin = 7;
 
// const int ReceivePin = 8;  // Set CameraSet Pin (Tracking On/Off)

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
   pinMode(13, OUTPUT);
   // Set Pinmode
   
}

void loop() {
  
  // Serial Receiver (ttyACM0) for RPi (From tracking Process)
  if (Serial.available()) {
     unsigned char myChar = Serial.read(); // SerialRead with unsigned char
     r_funclist(int(myChar)); // assign to function
  }
  // 이 부분은 GPIO_Receiver.py 에서 처리하기 때문에 주석
  // state = digitalRead(ReceivePin); // Check Arduino Pin 8 for switch Tracking Mode / Manual Mode
  
  // if (state == HIGH) { // If Camera is enabled
  //   ReceiveCount++;
  //   if (ReceiveCount == 1) {
  //       Serial.print(8); // Send signal to enable Tracking mode
  //   }

  //   if (ReceiveCount > 1 && ReceiveCount > 100) {
  //      ReceiveCount = 2;
  //   }
  //   delay(1);
    
  // } else { // If Tracking was disabled
  //   LowReceiveCount++;
  //   if (LowReceiveCount == 1) {
  //     Serial.print(9);  // Send signal to disable Tracking mode
  //   }

  //   if (LowReceiveCount > 1 && LowReceiveCount > 100 ) {
  //     LowReceiveCount = 2;
  //   }
  //   delay(100);
  }
// This is test code 
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
        digitalWrite(13, LOW);
        Serial.println("brake");
        break;
        
      case 1: // forward (fast)
        digitalWrite(goPin, HIGH);
        digitalWrite(stopPin, LOW);
        digitalWrite(leftPin, LOW);
        digitalWrite(rightPin, LOW); 
        digitalWrite(13, LOW);
        Serial.println("forward");
        break;
        
      case 2: // left (fast)
        digitalWrite(leftPin ,HIGH);
        digitalWrite(goPin, LOW);
        digitalWrite(stopPin, LOW);
        digitalWrite(rightPin, LOW);
        digitalWrite(13, LOW);

        Serial.println("left");
        break;
        
      case 3: // right (fast)
        digitalWrite(rightPin, HIGH);
        digitalWrite(goPin, LOW);
        digitalWrite(stopPin, LOW);
        digitalWrite(leftPin, LOW);
        digitalWrite(13, LOW);
        Serial.println("right");
        break;
        
      default:
        Serial.print("Not Detected number : ");
        Serial.println(number);   
        break;
     
   }


}
