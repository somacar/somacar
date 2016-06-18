
const int ledPin = 13; // led 핀 설정

void setup() {
   Serial.begin(9600); // 시리얼 통신 시작
}

void loop() {
  if (Serial.available()) { // 시리얼 통신 값 보냄
    r_funclist(Serial.read() - '0'); // int 형으로 받을 수 있게 - '0' 추가
  }
}

void r_funclist(int number) { // 시리얼 값으로 부터 받아서 switch 로 데이터 처리
   switch (number) {
      case 0:
        Serial.println("Received 0 from Raspi");
        break;
      case 1:
        
        Serial.println("Received 1 from Raspi");
        break;
      default:
        Serial.println("The Number is : " + number);
        break;
   }
}
