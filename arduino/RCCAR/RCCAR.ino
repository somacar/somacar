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

  speed1 = 200;

  BT.begin(9600);

  BT.println("HI");
}

void loop() {
  if (BT.available()) {
    inputWord = (BT.read());
    if (inputWord == 'f') {
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, HIGH);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
      BT.println("front!");
    } else if (inputWord == 'b') {
      digitalWrite(dir1PinA, HIGH);
      digitalWrite(dir2PinA, LOW);
      digitalWrite(dir1PinB, HIGH);
      digitalWrite(dir2PinB, LOW);
      BT.println("rear!");
    } else if (inputWord == 's') {
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, LOW);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, LOW);
      BT.println("stop!");
    }
  }//end of BT.available()
}//end of loop()

