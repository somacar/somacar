int inputPin = 8; // 아두이노 8번 핀으로 설정하고 그 핀에 전기가 흘러가게 끔 함
//  목적 : 다른 아두이노로 전기 신호가 흐르게끔 함

void setup() {
  pinMode(8, OUTPUT); // 출력 모드로 설정
  Serial.begin(9600);           // start serial for output
}

void loop() {
  digitalWrite(8, HIGH); // 8번 핀에다가 신호 줌
  delay(100); // 딜레이 100 
  Serial.println("on!"); 
  digitalWrite(8, LOW); // 신호 끔
  delay(100);
  Serial.println("off!");

}

# Master Arduino
