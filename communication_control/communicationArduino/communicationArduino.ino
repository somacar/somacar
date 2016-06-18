#include <SoftwareSerial.h>

SoftwareSerial BT(3, 2);

int commuPinFront = 8;
int commuPinRear = 9;
int commuPinRight = 10;
int commuPinLeft = 11;

int trackingPinStop = 16;
int trackingPinFront = 17;
int trackingPinRight = 18;
int trackingPinLeft = 19;

int trackingModePin = 14;
boolean isTracking = false;

int isCommuPin = 12;

int isComm;

int isFront;
int isRear;
int isRight;
int isLeft;

String inputString = "";

boolean stringComplete = false;

byte a;

void setup() {
    Serial.begin(9600);
    BT.begin(9600);
    BT.println("Hello from Arduino");
    
    pinMode(commuPinFront, OUTPUT);
    pinMode(commuPinRear, OUTPUT);
    pinMode(commuPinRight, OUTPUT);
    pinMode(commuPinLeft, OUTPUT);
    
    pinMode(isCommuPin, OUTPUT);

    pinMode(trackingPinFront, INPUT);
    pinMode(trackingPinRight, INPUT);
    pinMode(trackingPinLeft, INPUT);
    pinMode(trackingPinStop, INPUT);

    pinMode(trackingModePin, OUTPUT);
    
    isComm = LOW;

    isFront = LOW;
    isRear = LOW;

    isRight = LOW;
    isLeft = LOW;
}

void loop() {

  if(isTracking) {
    if(digitalRead(trackingPinStop) == HIGH) {
      Serial.println("Test1");
      isFront = LOW;
      isRight = LOW;
      isLeft = LOW;
    } else {
      Serial.println("Test2");
      isFront = digitalRead(trackingPinFront);
      isRight = digitalRead(trackingPinRight);
      isLeft = digitalRead(trackingPinLeft);
    }
  }
  
  if(BT.available()) {
    a = (byte) BT.read();
    BT.println(a);

    switch(a) {
      //stop
      case 0x00 :
        isFront = LOW;
        isRear = LOW;
        isRight = LOW;
        isLeft = LOW;
        break;
        
      //원격제어
      case 0xf1 :
        isComm = HIGH;
        break;
      case 0xf2 : 
        isComm = LOW;
        break;
        
      //트래킹
      case 0x51 :
        digitalWrite(trackingModePin, HIGH);
        isTracking = true;
        break;

      case 0x52 :
        digitalWrite(trackingModePin, LOW);
        isTracking = false;
        break;
        
      //frontDown 
      case 0x01 :
        isFront = HIGH; 
        break;
      //frontUp
      case 0x02 :
        isFront = LOW;
        break;
        
      //rearDown
      case 0x11 :
        isRear = HIGH;
        break;
      //rearup
      case 0x12 :
        isRear = LOW;
        break;

      //rightDown
      case 0x21 :
        isRight = HIGH;
        break;
      //rightUp
      case 0x22 :
        isRight = LOW;
        break;

      //leftDown
      case 0x31 :
        isLeft = HIGH;
        break;
      //leftUp
      case 0x32 :
        isLeft = LOW;
        break;       
    }

  }
  
  digitalWrite(isCommuPin, isComm);
  digitalWrite(commuPinFront, isFront);
  digitalWrite(commuPinRear, isRear);
  digitalWrite(commuPinRight, isRight);
  digitalWrite(commuPinLeft, isLeft);
}


