## 와이파이 연결하기
### 1. 라즈베리파이 환경 세팅
>우선 라즈베리파이에서 와이파이 세팅을 위한 환경을 구성해주도록 한다. iptime N100 Mini 제품을 라즈베리파이에 연결하도록 하자.
```
cd wifiConnectorforSetup
sudo ./setup.sh
```
>혹시나 네트워크 드라이버가 안 잡히는 문제가 있으면 upgrade.sh 도 같이 실행.

### 2. 라즈베리파이와 아두이노 연결
>라즈베리파이와 아두이노는 시리얼 방식으로 통신하기 때문에, USB 로 연결 해준다.

### 3. 아두이노에서 블루투스 모듈 (HC-06) 연결
>Digital 신호 10, 11 와 GND, VCC(3.7v) 를 연결.
Arduino Digital 포트 10번 핀 같은 경우 HC-06 에서 TXD 로 연결해주고
Arduino Digital 포트 11번 핀 같은 경우 RXD 로 연결 해준다.

### 4. Python 데몬 실행
>현재 Python 파일 실행 방식으로 되어 있기 때문에, screen 을 사용해서 데몬으로 작동 시킬 수 있도록 변경할 예정이다. 변경이 끝나는대로 Readme 에도 반영할 예정이다. Python 버젼은 3 버젼 이상을 권장하며, RPi2 에는 기본적으로 3.2 버젼이 설치되어 있다.
```
sudo python3.2 wifi-serial.py &
```
>관리` 권한이 반드시 필요하다 ㅠㅠ

### 4.1 부팅 시 자동 시작
>라즈베리파이 부팅 시 자동으로 파일이 실행되게 설정 하려면 vim 에디터로 /etc/rc.local 에 들어가서 명령어를 입력해주면 된다.
```
...
... (생략)
#sudo python3.2 /home/pi/Desktop/somacar/arduino-raspberry-wifi-connect/wifi-serial.py &
sudo screen -S rasp python3.2 /home/pi/Desktop/somacar/arduino-raspberry-wifi-connect/wifi-serial.py
exit 0
```
> 저장하고 sudo reboot 로 재부팅을 해준 후 파이썬 파일이 정상적으로 실행되는지 확인해본다.


### 5. 안드로이드 휴대폰에서 블루투스 연결 (권장)
>다른 휴대폰에서 블루투스 연결을 시도해 봤으나, 안드로이드가 제일 안정적으로 작동하는 편이기 때문에 테스트 시 안드로이드 환경에서 테스트 하는게 좋다.
```
ap_set_   <enter> //입력한다. 
- RECV : ap set process(^=fin)
701_        <enter> //글자를 한꺼번에 보내면 오류가 발생할 수 있기 때문에 AP정보, PW정보를 따로 전송한다.
27141212a^  <enter> // 패스워드를 입력하고 끝에 ^ 를 꼭 입력해 주어야 한다. 
- RECV : [BT] Wi-Fi Connected
```
>이 예제는 AP의 이름이 701이고, 패스워드가 27141212a 일 때의 예시로 설명해서 작성하였다.

### 6. 와이파이 연결상태 확인
>블루투스로 AP정보를 보내고 와이파이가 연결되었는지 유무를 판단할 수 있게 두가지 종류로 블루투스로 보내서 준다.
```
[BT] Wi-Fi Connected
```
>라즈베리파이가 와이파이 연결을 정상적으로 했을 때 나오는 메세지.
```
[BT] Failed to Connect
```
>정상적으로 연결하지 못 하였을 때 나오는 메세지이다. (감지하는데 다소 시간이 걸린다.)

