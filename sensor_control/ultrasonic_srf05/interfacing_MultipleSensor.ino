#define ECHOPIN 13                            // Pin to receive echo pulse
#define TRIGPIN 6                            // Pin to send trigger pulse

#define TRIGPIN2 9
#define ECHOPIN2 12

long duration, distance, Sensor1, Sensor2;


void setup(){
  Serial.begin(9600);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
}


void loop() {
  SonarSensor(TRIGPIN, ECHOPIN);
  Sensor1 = distance; 
  SonarSensor(TRIGPIN2, ECHOPIN2);
  Sensor2 = distance;
  Serial.print("Sensor 1 : ");
  Serial.print(Sensor1);
  Serial.print("/ Sensor 2 : ") ;
  Serial.println(Sensor2);
  

  if (Sensor1 > 20 || Sensor2 < 20 ) {
    if (Sensor1 > 20) {
      Serial.println("Sensor1 is 20cm exceed - (bottom detection)");
    }

    if (Sensor2 < 20) {
      Serial.println("Sensor2 is distance < 20, short -  (Forward Detection)");
    }
  }
  delay(300);
}



void SonarSensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
}

