int ledPin = 2;
int isHigh = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  Serial.println("Start");
}

void loop() {
  if(isHigh == 1) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void serialEvent() {
  while(Serial.available()) {
    char inChar = (char) Serial.read();

    if(inChar == 'h') {
      isHigh = 1;
      Serial.println("LED On!");
    } else {
      isHigh = 0;
      Serial.println("LED Off!");
    }
  }//end of while 
}
