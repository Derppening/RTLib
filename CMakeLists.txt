#[[

One day, I'll have to rewrite `CMakeLists-template.txt`. Until then...
# TODO(Derppening): Add a repo as an example on how to use RTLib.

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
project(RTLib
        VERSION 0.2.0)

# Include an error in case the user forgets to specify ARM as a toolchain
if (NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=rtlib/cmake/arm-toolchain.cmake or use\
    your own toolchain file")
endif ()

if ((NOT ${CMAKE_C_COMPILER_ID} STREQUAL "GNU") OR
    (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR
    (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 8.1.0))
    message(WARNING "This project is only tested for ARM GCC versions 8.1.0 or above. Some features may not be\
    supported by your compiler. You are using:
     ${CMAKE_CXX_COMPILER_ID} C, version ${CMAKE_C_COMPILER_VERSION}
     ${CMAKE_CXX_COMPILER_ID} CXX, version ${CMAKE_CXX_COMPILER_VERSION}")
endif()

# Set the C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Verbose logging
set(LOG_VERBOSE false)

# Choose target device here
set(DEVICE STM32F407VET6)

# Choose target mainboard pin configuration here
set(MAINBOARD_CONFIG STM32F407_DEV)

# Include libopencm3
include(cmake/libopencm3.cmake)

# Include rtlib
include(cmake/rtlib.cmake)

# Include doxygen for dank docs
include(cmake/doxygen.cmake)

# Include version logging if specified
if (LOG_VERBOSE)
    include(cmake/logging.cmake)
endif()

message("----------------------------------------")

# Determine if fmt is built as a subproject (using add_subdirectory)
# or if it is the master project.
#
# Taken from fmtlib.
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # Export variables to our parent
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_SPECIFIC_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TARGET_SPECIFIC_FLAGS}" PARENT_SCOPE)
    set(RTLIB_LINKER_FLAGS "${LINKER_FLAGS} -Wl,--gc-sections" PARENT_SCOPE)
    set(LIBOPENCM3_LIB_PATH ${LIBOPENCM3_DIR}/lib PARENT_SCOPE)
endif ()
