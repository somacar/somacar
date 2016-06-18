// 아두이노와 아두이노 끼리 Pin 통신
int ledPin = 13; // 아두이노 led 인덱스 설정
int output = 10; // 다른 아두이노로부터 받을 핀 번호 설정

void setup() {
  // put your setup code here, to run once:
  pinMode(10, INPUT); // 입력 모드로 설정
  pinMode(13, OUTPUT); // 출력 모드 ( led )
}

void loop() {
  state = digitalRead(10); // 10 번 핀 상태를 계속 읽어들임
  if (state == HIGH) { // 만약 신호가 들어오면
    digitalWrite(13, HIGH); // LED 불을 켬
  } else {
    digitalWrite(13, LOW); // LED 끔
  }
   
}

