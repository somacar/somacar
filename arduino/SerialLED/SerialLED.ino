int ledPin = 2;
String inputString = "";
int isHigh = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
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
    if(inChar == '\n' || inChar == '\r') {
      if(inputString.equals("high")) {
        isHigh = 1;
        Serial.println("LED on!");
      } else {
        isHigh = 0;
        Serial.println("LED off!");
      }
    }else {
        inputString += inChar;
      }
  }//end of while 
}

