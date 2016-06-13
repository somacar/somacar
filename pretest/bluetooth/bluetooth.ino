#include <SoftwareSerial.h>

SoftwareSerial BT(11, 10); //Tx, Rx
int ledPin = 2;
char inChar;

void setup() {
  pinMode(ledPin, OUTPUT);

  BT.begin(9600);
  BT.println("Hello from Arduino");
}

void loop() {
  if(BT.available()) {
    inChar = BT.read();
    BT.write(inChar);
    if( inChar == '1' ) {
      BT.write(inChar);
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
      BT.write(inChar);
    }
  }//end of BT.available()
}//end of loop()
