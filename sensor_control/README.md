# 센서 제어


## 기울임 감지 (Overturn detection)
> 전동자 운행 중 전복될 가능성이 있기 때문에, 가속도 센서 (MPU6050) 를 활용하여 아두이노에서 센서가 많이 기울어졌을 때를 감지할 수 있습니다. 

##### 해당 폴더
- Arduino_Accelerator_Control/

##### 필요한 라이브러리 
- Arduino_Accelerator_Control/data/MPU6050.zip


##### 아두이노 세팅
- Arduino_Accelerator_Control/IMG_3169.JPG

## 적외선 센서 (장애물 탐지)
> 적외선 센서 - 아날로그 신호 값을 읽어 들여 물체가 있다는 것을 인식함. (만약 물체가 있을 시 아두이노 Digital pin 13) - LED 를 on/off 함.

##### 해당 폴더
- /InfraredSensor/

## 초음파 센서 (거리 측정)
> 초음파 센서 - 전동차 운전 중 움푹 파인 쪽을 감지하기 위해 사전에 사고를 예방할 수 있도록 함. 

##### 해당 폴더
- /ultrasonic/
- /ultrasonic_srf05/ (v2) - 정확도 향상

