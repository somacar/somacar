#define trig1 3
#define echo1 2
#define trig2 4
#define echo2 5
#define trig3 7
#define echo3 8

long duration, distance, right, back, front, left;

void setup() {
  Serial.begin(9600);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);
}

void loop() {
  sonarSensor(trig1, echo1);
  right = distance;
  sonarSensor(trig2, echo2);
  left = distance;
  sonarSensor(trig3, echo3);
  front = distance;

  Serial.print(left);
  Serial.print(" - ");
  Serial.print(front);
  Serial.print(" - ");
  Serial.print(right);
  Serial.println();
}

void sonarSensor(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(100);
  digitalWrite(trig, HIGH);
  delayMicroseconds(100);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;
}

