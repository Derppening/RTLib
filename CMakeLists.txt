cmake_minimum_required(VERSION 3.8)
project(libopencm3-stm32)

IF(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=/PATH/TO/TOOLCHAIN_FILE")
ENDIF(NOT CMAKE_TOOLCHAIN_FILE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Setup libopencm3
set(DEVICE STM32F103VCT6)
include(cmake/libopencm3.cmake)

# Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ${TARGET_FLAGS} -Os -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 ${TARGET_FLAGS}")
set(LINKER_FLAGS "${LINKER_FLAGS} -nostartfiles -lc -lnosys --specs=rdimon.specs -Wl,--gc-sections")

# Collect sources
file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.cpp)

# Specify output file
add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES})


