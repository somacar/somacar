cmake_minimum_required(VERSION 3.5)
project(opencv)
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
find_package(OpenCV REQUIRED)
find_package(Tesseract)
if(Tesseract_FOUND)
    message(WARNING "Tesseract:   YES")
    set(HAVE_TESSERACT 1)
else()
    message(WARNING "Tesseract:   NO")
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Tesseract_INCLUDE_DIR})

set(SOURCE_FILES test/main.cpp)
add_executable(opencv ${SOURCE_FILES})

target_link_libraries(opencv ${OpenCV_LIBS} ${Tesseract_LIBS})

#add_executable( opencv test/main.cpp )
#add_executable( opencv qrcode/main.cpp qrcode/FinderPattern.cpp qrcode/FinderPatternTrio.cpp qrcode/qrReader.cpp)
#target_link_libraries( opencv ${OpenCV_LIBS} ${Tesseract_LIBS} )