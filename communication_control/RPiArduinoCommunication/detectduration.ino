int pin_6;
int pin_9;
int pin_10;
int pin_11;

void setup() {
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);
}

void loop() {
  pin_6 = digitalRead(3);
  //delayMicroseconds(10);
  if (pin_6) {
    pin_9 = digitalRead(9);
    pin_10 = digitalRead(10);
    pin_11 = digitalRead(11);
    
    if (!pin_9 == 0) {
      Serial.print("pin 9: ");
      Serial.print(pin_9);
      Serial.println(", forward");
    }

    if (!pin_10 == 0) {
      Serial.print("pin 10:");
      Serial.print(pin_10);
      Serial.println(", left");
    }
    
    if (!pin_11 == 0) {
      Serial.print("pin 11:");
      Serial.print(pin_11);
      Serial.println(", right");
    }
    
    
  } else {
    Serial.print("6 Pin not connected! : ");
    Serial.println(pin_6);
    delay(1000);
  }
  
}


