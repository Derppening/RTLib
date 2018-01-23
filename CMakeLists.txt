#[[

One day, I'll have to rewrite `CMakeLists-template.txt`. Until then...
# TODO(Derppening): Rewrite CMakeLists-template.txt.

]]

#[[

This file is part of RTLib.

Copyright (C) 2017 waicool20 <waicool20@gmail.com>
Copyright (C) 2017-2018 Derppening <david.18.19.21@gmail.com>

RTLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

RTLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with RTLib.  If not, see <http://www.gnu.org/licenses/>.

]]

cmake_minimum_required(VERSION 3.9)
project(RTLib)

# Include an error in case the user forgets to specify ARM as a toolchain
if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=rtlib/cmake/arm-toolchain.cmake or use\
    your own toolchain file")
endif ()

if ((NOT ${CMAKE_C_COMPILER_ID} STREQUAL "GNU") OR
    (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR
    (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 7.2.1))
    message(WARNING "This project is targeted for ARM GCC versions 7.2.1 or above. Some features may not be supported\
     by your compiler. You are using:
     ${CMAKE_CXX_COMPILER_ID} C, version ${CMAKE_C_COMPILER_VERSION}
     ${CMAKE_CXX_COMPILER_ID} CXX, version ${CMAKE_CXX_COMPILER_VERSION}")
endif()

# Set the C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Verbose logging
set(LOG_VERBOSE false)

# Choose target device here
set(DEVICE STM32F407VET6)

# Choose target mainboard pin configuration here
add_definitions(-DSTM32F407_DEV)

# Include libopencm3
include(cmake/libopencm3.cmake)

# Include rtlib
include(cmake/rtlib.cmake)

# Include doxygen for dank docs
include(cmake/doxygen.cmake)

# Include additional compilation flags
include(cmake/compile_flags.cmake)

# Collect sources into SOURCE_FILES variable and include your headers directory
file(GLOB_RECURSE SOURCE_FILES "src/*.c" "src/*.cpp")
file(GLOB_RECURSE INC_FILES "src/*.h" "src/*.tpp")
include_directories(src)

# Specify output executable
add_executable(${PROJECT_NAME}.elf main.cpp ${INC_FILES} ${RTLIB_SOURCE_FILES})

# Output elf file size
add_custom_command(
        TARGET ${PROJECT_NAME}.elf
        POST_BUILD
        COMMAND ${ARM_SIZE} ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.elf
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

message("----------------------------------------")
