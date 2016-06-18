int dirPinA1 = 6;
int dirPinA2 = 7;
int speedA = 2;

int dirPinB1 = 4;
int dirPinB2 = 5;
int speedB = 3;

int cameraPin = 8;

int stopPin = 13;
int goPin = 12;
int rightPin = 11;
int leftPin = 10;

int dirAOutPin1 = LOW;
int dirAOutPin2 = LOW;
int dirBOutPin1 = LOW;
int dirBOutPin2 = LOW;

void setup() {
  Serial.begin(9600);
  
  pinMode(dirPinA1, OUTPUT);
  pinMode(dirPinA2, OUTPUT);
  pinMode(speedA, OUTPUT);
  
  pinMode(dirPinB1, OUTPUT);
  pinMode(dirPinB2, OUTPUT);
  pinMode(speedB, OUTPUT);

  pinMode(stopPin, INPUT);
  pinMode(goPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(leftPin, INPUT);
  
  pinMode(cameraPin, OUTPUT);
}

void loop() {
  analogWrite(speedA, 255);
  analogWrite(speedB, 255);

  digitalWrite(cameraPin, HIGH);
  
  if(digitalRead(stopPin) == HIGH) {
    Serial.println("stop");
    dirAOutPin1 = LOW;
    dirAOutPin2 = LOW;
    dirBOutPin1 = LOW;
    dirBOutPin2 = LOW;
  } else if(digitalRead(goPin) == HIGH) {
    Serial.println("front");
    dirAOutPin1 = LOW;
    dirAOutPin2 = LOW;
    dirBOutPin1 = HIGH;
    dirBOutPin2 = LOW;
  } else if(digitalRead(rightPin) == HIGH) {
    Serial.println("right");
    dirAOutPin1 = HIGH;
    dirAOutPin2 = LOW;
    dirBOutPin1 = HIGH;
    dirBOutPin2 = LOW;
  } else if(digitalRead(leftPin) == HIGH) {
    Serial.println("LEFT");
    dirAOutPin1 = LOW;
    dirAOutPin2 = HIGH;
    dirBOutPin1 = HIGH;
    dirBOutPin2 = LOW;
  } 
    digitalWrite(dirPinA1, dirAOutPin1);
    digitalWrite(dirPinA2, dirAOutPin2);
    digitalWrite(dirPinB1, dirBOutPin1);
    digitalWrite(dirPinB2, dirBOutPin2);
    
}
