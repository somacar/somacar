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
  }
}

// 시리얼로부터 받은 값들 넘겨 줌.
// 값에 따라 다른 디지털 신호를 주는 식으로 하여 브레이크, 직진/왼쪽 오른쪽을 줌.
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
        Serial.print("Not Detected number : "); //디버그 용도
        Serial.println(number);   
        break;
     
   }


}
