#include <SoftwareSerial.h>

SoftwareSerial BT(3, 2);

void setup() {
  pinMode(13, OUTPUT);

  BT.begin(9600);

  BT.println("Hello from Arduino");
}

byte a;

void loop() {
  if(BT.available()) {
    a = BT.read();
    BT.write(a);
    if( a == 1 ) {
      BT.write(a);
    } else {
      BT.write(0x23);
    }
      
    
    /*a = (BT.read());
    if(a == '1') {
      digitalWrite(13, HIGH);
      BT.println("LED on");
    }

    if(a == '2') {
      digitalWrite(13, LOW);
      BT.println("LED off");
    }

    if(a =='?') {
      BT.println("Send '1' to turn LED on");
      BT.println("Send '2' to turn LED on");
      
    }
    */
  }//end of BT.available()
}//end of loop()
