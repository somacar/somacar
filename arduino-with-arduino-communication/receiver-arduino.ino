int ledPin = 13;
int output = 10;

int state = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(10, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  state = digitalRead(10);
  if (state == HIGH) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
   
}

