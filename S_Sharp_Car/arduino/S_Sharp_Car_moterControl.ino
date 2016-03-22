
int dir1PinA = 4;
int dir2PinA = 5;
int dir1PinB = 6;
int dir2PinB = 7;
int speedPinA = 3;
int speedPinB = 9;
int speed1;
int dir;

void setup() {

  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);

  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);

}

void loop() {
       digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, HIGH);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);

}
