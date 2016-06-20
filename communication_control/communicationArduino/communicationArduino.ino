#include <SoftwareSerial.h>

//bluetooth pin 설정
//Tx ==3, Rx == 2
SoftwareSerial BT(3, 2);

//bluetooth 통신 pin 설정
int commuPinFront = 8;
int commuPinRear = 9;
int commuPinRight = 10;
int commuPinLeft = 11;
int isCommuPin = 12;

//tracking pin 설정
int trackingPinStop = 16;
int trackingPinFront = 17;
int trackingPinRight = 18;
int trackingPinLeft = 19;
int trackingModePin = 14;

//현재 모드 상태
//isTacking -> HIGH -> 트래킹모드
//isComm -> HIGH -> 원격제어모드
boolean isTracking = false;
boolean isComm = false;

//모터제어 Pin
int isFront;
int isRear;
int isRight;
int isLeft;

//통신버퍼변수
byte bufferByte;

void setup() {
    //초기 통신 설정
    Serial.begin(9600);
    BT.begin(9600);
    BT.println("Hello from Arduino");

    //모터와통신핀 OUTPUT설정
    pinMode(commuPinFront, OUTPUT);
    pinMode(commuPinRear, OUTPUT);
    pinMode(commuPinRight, OUTPUT);
    pinMode(commuPinLeft, OUTPUT);

    //트래킹 통신 INPUT설정
    pinMode(trackingPinFront, INPUT);
    pinMode(trackingPinRight, INPUT);
    pinMode(trackingPinLeft, INPUT);
    pinMode(trackingPinStop, INPUT);

    //모드 pin OUTPUT 설정
    pinMode(trackingModePin, OUTPUT);
    pinMode(isCommuPin, OUTPUT);

    //초기 모터 제어 설정
    isFront = LOW;
    isRear = LOW;
    isRight = LOW;
    isLeft = LOW;
}

void loop() {
  //트래킹모드 일때 설정
  if(isTracking) {
    if(digitalRead(trackingPinStop) == HIGH) {
      isFront = LOW;
      isRight = LOW;
      isLeft = LOW;
    } else {
      
      if( digitalRead(trackingPinFront) == HIGH) {
        isFront = HIGH;
        isRight = LOW;
        isLeft = LOW;
      } else if( digitalRead(trackingPinRight) == HIGH ) {
        isFront = HIGH;
        isRight = HIGH;
        isLeft = LOW;
      } else if( digitalRead(trackingPinLeft) == HIGH ) {
        isFront = HIGH;
        isRight = LOW;
        isLeft = HIGH;
      }
      
    }
  }// end of tracking Mode
  
  //받은 통신값 설정
  if(BT.available()) {
    bluetoothCommunicationFunc();
  }
  
  digitalWrite(isCommuPin, isComm);
  digitalWrite(commuPinFront, isFront);
  digitalWrite(commuPinRear, isRear);
  digitalWrite(commuPinRight, isRight);
  digitalWrite(commuPinLeft, isLeft);
}

void bluetoothCommunicationFunc() {
  bufferByte = (byte) BT.read();
    BT.println(bufferByte);

    switch(bufferByte) {
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

