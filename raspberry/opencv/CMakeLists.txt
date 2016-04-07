cmake_minimum_required(VERSION 3.5)
project(opencv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(opencv ${SOURCE_FILES})

target_link_libraries(opencv ${OpenCV_LIBS})