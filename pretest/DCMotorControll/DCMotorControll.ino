int dirPinA1 = 4;
int dirPinA2 = 5;
int speedA = 9;

int dirPinB1 = 6;
int dirPinB2 = 7;
int speedB = 10;

void setup() {
  pinMode(dirPinA1, OUTPUT);
  pinMode(dirPinA2, OUTPUT);
  pinMode(speedA, OUTPUT);
  
  pinMode(dirPinB1, OUTPUT);
  pinMode(dirPinB2, OUTPUT);
  pinMode(speedB, OUTPUT);
}

void loop() {
  //OUT1,2 모터는 최대속도
  //OUT3,4 모터는 절반이하 속도
  analogWrite(speedA, 255);
  analogWrite(speedB, 100);

  //OUT1,2 모터 작동, OUT 3,4 모터 미작동
  digitalWrite(dirPinA1, HIGH);
  digitalWrite(dirPinA2, LOW);
  digitalWrite(dirPinB1, LOW);
  digitalWrite(dirPinB2, LOW);

  delay(2000);

  //OUT1,2 모터 미작동, OUT 3,4 모터 작동
  digitalWrite(dirPinA1, LOW);
  digitalWrite(dirPinA2, LOW);
  digitalWrite(dirPinB1, HIGH);
  digitalWrite(dirPinB2, LOW);

  delay(2000);
}
