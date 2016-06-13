int state = 0;
int ReceiveCount = 0;


void setup() {
   pinMode(8, INPUT);
   Serial.begin(9600);
   Serial.print(9);
}

void loop() {
  state = digitalRead(8);
//
  if (state == HIGH) {
    ReceiveCount++;
    if (ReceiveCount == 1) {
        Serial.print('8'); // request to RPi enable daemon
    }

    if (ReceiveCount > 1 && ReceiveCount > 100) {
       ReceiveCount = 2;
    }
    Serial.println(state);
    delay(1);
  } else {
    //Serial.print("nothing");
   //
   Serial.print(9);
    delay(100);
    ReceiveCount = 0; // request to RPi disable daemon
  }
}

