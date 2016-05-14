int dir1PinA = 4;
int dir2PinA = 5;
int dir1PinB = 6;
int dir2PinB = 7; 
int speedPinA = 8;
int speedPinB = 9;
int speed1;
int dir;
String inputString = "";

void setup() {
  Serial.begin(9600);
  
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);

  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  
  speed1 = 200;
  dir = 0;
}

void loop() {
  analogWrite(speedPinA, speed1);
  analogWrite(speedPinB, speed1);
  if(dir == 1) {
    digitalWrite(dir1PinA, LOW);
    digitalWrite(dir2PinA, HIGH);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, HIGH);    
  } else if(dir == 2) {
    digitalWrite(dir1PinA, HIGH);
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
  } else if(dir == 0) {
    digitalWrite(dir1PinA, LOW);
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, LOW);
  }
}

void serialEvent() {
  while(Serial.available()) {
    char inChar = (char) Serial.read();
    if(inChar == '\n' || inChar == '\r') {
      
      if(inputString.equals("front")) {
          dir = 1;
          inputString = "";
          Serial.println("Front");
      } else if (inputString.equals("rear")) {
          dir = 2;
          inputString = "";
          Serial.println("Rear");
      } else if (inputString.equals("stop")) {
          dir = 0;
          inputString = "";
          Serial.println("Stop");
      }
      //구문끝
    }else {
        inputString += inChar;
      }
  }//end of while
}

