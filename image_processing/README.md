# 영상처리
### 개발 환경
- Raspberry Pi 2
- OpenCV (3.1.0)
- OpenCL
- Tesseract (3.03)

###  Raspberry pi에 OpenCV 설치
1. 업데이트
```
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo rpi-update
$ sudo reboot
```
2. dependencies 설치
    - 빌드 packages
    ```
    $ sudo apt-get install build-essential git cmake pkg-config
    ```
    - 이미지 IO packages
    ```
    $ sudo apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev
    ```
    - 영상 IO packages
    ```
    $ sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev
    ```
    - GTK
    ```
    $ sudo apt-get install libgtk2.0-dev
    ```
    - 기타 Operations
    ```
    $ sudo apt-get install libatlas-base-dev gfortran
    ```
    - 파이썬 연동
    ```
    $ sudo apt-get install python2.7-dev python3-dev
    ```
3. tesseract 설치
```
sudo apt-get install tesseract-ocr tesseract-ocr-dev libleptonica-dev
```
4. OpenCV 다운로드 및 빌드
```
$ cd ~
$ git clone https://github.com/Itseez/opencv.git
$ cd opencv
$ git checkout 3.1.0
$ cd ~
$ c
$ cd opencv_contrib
$ git checkout 3.1.0
$ cd ~/opencv
$ mkdir build
$ cd build
$ sudo cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_C_EXAMPLES=OFF \
    -D BUILD_EXAMPLES=ON \
    -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules ..
$ sudo make -j4
$ sudo make install
```