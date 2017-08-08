#[[

Just use the `CMakeLists-template.txt` if you want a ready-to-go boilerplate file,
this file is commented and mostly identical, only some slight differences in the
`include` paths and `add_executable` sources. I think at least.

]]
cmake_minimum_required(VERSION 3.8)
project(RTLib)

# Include an error in case the user forgets to specify ARM as a toolchain
if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=rtlib/cmake/arm-toolchain.cmake or use your own toolchain file")
endif ()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Choose target device here
set(DEVICE STM32F103VCT6)

# Include libopencm3
include(cmake/libopencm3.cmake)

# Include rtlib
include(cmake/rtlib.cmake)

# Collect sources into SOURCE_FILES variable and include your headers directory
file(GLOB_RECURSE SOURCE_FILES "src/*.c" "src/*.cpp")
include_directories(inc)

# Specify output executable
add_executable(${PROJECT_NAME}.elf ${RTLIB_SOURCE_FILES})
