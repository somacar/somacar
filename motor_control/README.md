# 모터 제어

### Requirements (H/W)
- arduino 5v
- Dc 모터 2개
- 전선 12개
- 모터 드라이브 L298N
- 외부전력 전지

### Requirements (S/W)
- Arduino Sketch


### DC Motor
![DC Motor](http://i.imgur.com/diL2HvP.png "DC motor")
> DC 모터, + - 부분에 각각 전지를 연결해주면 모터가 회전하고, - + 로 연결하게 된다면 반대 방향으로 회전합니다. 

### Motor driver
![DC Motor](http://i.imgur.com/hXqRLyo.png "DC motor")
>L298N 모듈은 모터를 2개 까지 제어할 수 있습니다. 각 제어핀은 ENA1, IN1, IN2으로 모터A를 제어할 수 있고 ENA2, IN3, IN4로 모터B를 제어 할 수 있습니다.
입력전압은 12V까지 버틸 수 있습니다. 5V는 모듈 입력 전압입니다. 따라서, 12V에는 외부 전력의 VCC를 연결하고 5V에는 아두이노 5V전력을 연결하면 됩니다. 또한 두 VCC 전부 다시 GND에 연결하여 주면 됩니다.
DC모터는 (+), (-) 부분은 바꾸면서 방향을 설정합니다.
IN1과 IN2를 각각 HIGH, LOW로 두게되면 회전을 하게 되고, 반대로 각각 LOW, HIGH를 주게되면 반대방향으로 회전하게 됩니다.
물론, 둘다 HIGH나 LOW로 두게되면 회전하지 않습니다.


### 회로 구성
>저는 아래와 같이 연결하였습니다. 전지 전력은 1.5V 직렬 연결 3개를 이용하여 4.5V를 전압을 사용하였습니다.

```
12V ------ 외부전지 (+)
5V  ------ 아두이노 5V
GND ------ 외부전지 (-), 아두이노 GND
ENA1 ------ 9 PIN
IN1 -------- 4 PIN
IN2 -------- 5 PIN
ENA2 ------- 10PIN
IN3 -------- 6 PIN
IN4 -------- 7PIN
```
>IN1,2,3,4는 어떻게 연결해도 상관없습니다. 다만, 방향이 바뀔 뿐이니 잘 모르곘으면 테스트해보면서 방향 설정하시면됩니다.
주의하실 점은 ENA입니다. ENA는 아두이노 디지털 핀에서 PWM이 가능한 부분에 연결하여야합니다. 즉, ~이 붙어있는 핀에 연결하셔야합니다.


### Source code 

```
int dirPinA1 = 4;
int dirPinA2 = 5;
int speedA = 9;

int dirPinB1 = 6;
int dirPinB2 = 7;
int speedB = 10;

void setup() {
  pinMode(dirPinA1, OUTPUT);
  pinMode(dirPinA2, OUTPUT);
  pinMode(speedA, OUTPUT);
  
  pinMode(dirPinB1, OUTPUT);
  pinMode(dirPinB2, OUTPUT);
  pinMode(speedB, OUTPUT);
}

void loop() {
  //OUT1,2 모터는 최대속도
  //OUT3,4 모터는 절반이하 속도
  analogWrite(speedA, 255);
  analogWrite(speedB, 100);

  //OUT1,2 모터 작동, OUT 3,4 모터 미작동
  digitalWrite(dirPinA1, HIGH);
  digitalWrite(dirPinA2, LOW);
  digitalWrite(dirPinB1, LOW);
  digitalWrite(dirPinB2, LOW);

  delay(2000);

  //OUT1,2 모터 미작동, OUT 3,4 모터 작동
  digitalWrite(dirPinA1, LOW);
  digitalWrite(dirPinA2, LOW);
  digitalWrite(dirPinB1, HIGH);
  digitalWrite(dirPinB2, LOW);

  delay(2000);
}

```

### Source 코드 설명
> 소스코드는 간단합니다. 모터 A와 모터 B가 각 2초마다 회전합니다. 모터 B의 경우에는 속도를 절반이하로 낮추었습니다.
육안 상으로 잘 확인안되지만, 소리들어보면 확실히 모터A가 속도가 빠릅니다.

### Tutorial video
```
http://serviceapi.nmv.naver.com/flash/convertIframeTag.nhn?vid=3449B3D98FD7CF79452C185158158FA1F14E&outKey=V127524c590e924815e5a3c95afb8083dbc8422fdd0f95f89765d3c95afb8083dbc84&width=544&height=306
```
