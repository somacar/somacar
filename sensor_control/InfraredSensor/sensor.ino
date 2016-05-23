int ledPin = 13;
int sensor = A0;   
int val = 0;

void setup()
{
  pinMode(sensor, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  val = analogRead(A0);   // read the input pin
  Serial.println(val);
  delay(100);
  
  if (val > 680) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
}
