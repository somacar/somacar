#include <Wire.h>
#define SLAVE_ADDRESS 0x04 // RPi Address for I2C Communication
#define I2C_PIN 8

int state = 0;
int ReceiveCount = 0;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(I2C_PIN, INPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // Init data
  state = 0;
  ReceiveCount = 0;
  Serial.println("Ready!");
  
}
  
  
  
void loop() {
  
  state = digitalRead(I2C_PIN);
  Serial.print("I2CPin Status:");
  Serial.println(state);
  if (state == HIGH) {
    ReceiveCount++;
    if (ReceiveCount == 1) { // Only one time send to I2C
      Wire.write(5); // send to RPi
      digitalWrite(13, HIGH); // set the LED on
      Serial.println("Sent(just one time print)");
    }

    if (ReceiveCount > 1 && ReceiveCount > 100) {
      ReceiveCount = 2;
    }
    
  } else {
    digitalWrite(13, LOW); // set the LED on
    ReceiveCount = 0;
  }
  
  delay(100);
}

