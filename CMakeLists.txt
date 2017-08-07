cmake_minimum_required(VERSION 3.8)
project(libopencm3-stm32)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(libopencm3-stm32 ${SOURCE_FILES})
