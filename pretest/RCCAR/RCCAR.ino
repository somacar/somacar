#include <SoftwareSerial.h>

//front = f
//rear = b 
//right = r
//left = l
//stop = s

SoftwareSerial BT(10, 11);

int dir1PinA = 4;
int dir2PinA = 5;
int dir1PinB = 6;
int dir2PinB = 7;
int speedPinA = 3;
int speedPinB = 9;
int speed1;
int dir;

String inputString = "";
char inputWord = 's';

void setup() {

  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);

  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);

  speed1 = 255;

  BT.begin(9600);

  BT.println("HI");
}

void loop() {
  if (BT.available()) {
    inputWord = (BT.read());
    if (inputWord == 'r') {
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, HIGH);
      BT.println("right!");
    } else if (inputWord == 'l') {
      digitalWrite(dir1PinA, HIGH);
      digitalWrite(dir2PinA, LOW);
      BT.println("left!");
    } else if (inputWord == 's') {
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, LOW);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, LOW);
      BT.println("stop!");
    }
    if (inputWord == 'b') {
      digitalWrite(dir1PinB, HIGH);
      digitalWrite(dir2PinB, LOW);
      BT.println("rear!");
    } else if (inputWord == 'f') {
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
      BT.println("front!");
    }
  }//end of BT.available()
}//end of loop()

