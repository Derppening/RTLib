#[[

Just use the `CMakeLists-template.txt` if you want a ready-to-go boilerplate file,
this file is commented and mostly identical, only some slight differences in the
`include` paths and `add_executable` sources. I think at least.

]]
cmake_minimum_required(VERSION 3.9)
project(RTLib)

# Include an error in case the user forgets to specify ARM as a toolchain
if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=rtlib/cmake/arm-toolchain.cmake or use your own toolchain file")
endif ()

if ((NOT ${CMAKE_C_COMPILER_ID} STREQUAL "GNU") OR
    (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR
    (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 6.3.1))
    message(WARNING "This project is targeted for ARM GCC versions 6.3.1 or above. Some features may not be supported by your compiler.")
endif()

# Set the C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Verbose logging
set(LOG_VERBOSE 0)

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
file(GLOB_RECURSE INC_FILES "inc/*.h" "src/*.tpp")
include_directories(inc)

# Specify output executable
add_executable(${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.elf main.cpp ${INC_FILES} ${RTLIB_SOURCE_FILES})

add_custom_command(
        TARGET ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.elf
        POST_BUILD
        COMMAND ${ARM_SIZE} ${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.elf
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
