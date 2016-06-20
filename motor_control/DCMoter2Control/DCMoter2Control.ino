int dir1PinB = 5;
int dir2PinB = 4;
int speedOutPinB = 2;


int dir1PinA = 7;
int dir2PinA = 6;
int speedOutPinA = 3;

int speedIntputPin = 11;

int frontPin = 12;
int rearPin = 13;

int commuPinFront = 19;
int commuPinRear = 18;
int commuPinRight = 17;
int commuPinLeft = 16;

int isCommu = 14;

int speed1;
int speed2;

int val;

void setup() {
    Serial.begin(9600);
    Serial.println("Start");

    pinMode(commuPinFront, INPUT);
    pinMode(commuPinRear, INPUT);
    pinMode(commuPinRight, INPUT);
    pinMode(commuPinLeft, INPUT);
    
    pinMode(isCommu, INPUT);
    
    //
    pinMode(dir1PinB, OUTPUT);
    pinMode(dir2PinB, OUTPUT);
    pinMode(speedOutPinB, OUTPUT);

    pinMode(dir1PinA, OUTPUT);
    pinMode(dir2PinA, OUTPUT);
    pinMode(speedOutPinA, OUTPUT);

    pinMode(speedIntputPin, INPUT);
    
    pinMode(frontPin, INPUT);
    pinMode(rearPin, INPUT);
    
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, LOW);

    speed1 = 255;
    speed2 = 255;
}

void loop() {
  analogWrite(speedOutPinA, 175);

 if(digitalRead(8) == LOW) {
  
  Serial.println("ultra LOW");
  if(digitalRead(isCommu) == HIGH) {
      analogWrite(speedOutPinB, 255);
      
    if(digitalRead(commuPinFront) == HIGH) {
        frontDir();
      } else if(digitalRead(commuPinRear) == HIGH) {
        rearDir();
      } else {
        stopDirRear();
      }
      
      if(digitalRead(commuPinRight) == HIGH) {
        rightDir();
      } else if(digitalRead(commuPinLeft) == HIGH) {
        leftDir();
      } else {
        stopDirFront();
      }
      
  } else {
    if(digitalRead(speedIntputPin) == HIGH) {
      analogWrite(speedOutPinB, 140);
    } else {
      analogWrite(speedOutPinB, 255);
    }
    
    if(digitalRead(frontPin) == digitalRead(rearPin)) {
      stopDirRear();
    } else if(digitalRead(frontPin) == HIGH) {
      frontDir();
    } else if(digitalRead(rearPin) == HIGH) {
      rearDir();
    } else {
      stopDirRear();
    }
    
  }
  } else {
      Serial.println("ultra HIGH");
      stopDirRear();    
  }
  
}

void frontDir() {
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
}

void rearDir() {
  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
}

void stopDirRear() {
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, LOW);
}

void rightDir() {
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);
}

void leftDir() {
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
}

void stopDirFront() {
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, LOW);
}

