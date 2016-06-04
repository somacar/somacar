# 영상처리
### 개발 환경
- Raspberry Pi 2
- OpenCV (3.1.0)
- OpenCL (1.2)
- Tesseract (3.03)

###  Raspberry pi에 OpenCV 설치
- 업데이트
```
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo rpi-update
$ sudo reboot
```
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
- tesseract 설치
```
$ sudo apt-get install tesseract-ocr tesseract-ocr-dev libleptonica-dev
```
- OpenCV 다운로드 및 빌드
```
$ cd ~
$ git clone https://github.com/Itseez/opencv.git
$ cd opencv
$ git checkout 3.1.0
$ cd ~
$ git clone https://github.com/itseez/opencv_contrib
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
- 프로젝트 빌드
```
$ cd image_processing
$ mkdir build
$ cd build
$ cmake CMAKE_BUILD_TYPE=Release ..
$ make
```

### 전체 구성
| | |
| ------------- |:-------------:|
|**project**|카메라를 통해서 영상을 읽어서 특정 직사각형을 찾았을 경우에 텍스트 인식을 통해 물체를 추적|
|**test**|OpenCV 다양한 예제들|
|CMakeLists.txt|OpenCL과 Tesseract 연동|
|FindTesseract.cmake|Tesseract 연동|

### 세부 설명
- **project** > main.cpp
    - 특정 직사각형을 찾았을 때 tesseract-ocr 사용
    ``` cpp
    if (target.find_square(&sqr)) ...
    ```
- **project** > ocr.cpp
    - (이미지 축소 여부, 영역, 그룹)
    ``` cpp
    OCRTess::OCRTess(bool downsize, int region, int group)
    ```
    - 흑백 처리를 통해 텍스트 인식
    ``` cpp
    void OCRTess::detectAndRecog()
    ```
- **project** > target.cpp
    - (이미지, 색 구분 여부)
    ``` cpp
    void Target::init(UMat f, bool color)
    ```
    - 꼭지점이 4개 && 최소 25px * 25px && 
    ``` cpp
    bool Target::is_square(vector<Point> c, Rect *rect)
    ```
    - 조건을 모두 충족한 직사각형을 찾으면 투시 보정을 위해 기하학적 변환
    ``` cpp
    bool Target::find_square(UMat *sqr)
    ```
- CMakeLists.txt
    - OpenCL
    ``` cmake
    set(OPENCV_OPENCL_RUNTIME)
    set(OPENCV_OPENCL_DEVICE :GPU:0)
    ```
    - Tesseract
    ``` cmake
    find_package(Tesseract)
    if (Tesseract_FOUND)
        set(HAVE_TESSERACT true)
        add_definitions(-DHAVE_TESSERACT=true)
        message(WARNING "Tesseract:   YES")
    else ()
        message(WARNING "Tesseract:   NO")
    endif ()
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    include_directories(${Tesseract_INCLUDE_DIR})
    ... 
    target_link_libraries(opencv ${OpenCV_LIBS} ${Tesseract_LIBS})
    ```
