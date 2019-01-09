#[[

This file is part of RTLib.

Copyright (C) 2017 waicool20 <waicool20@gmail.com>
Copyright (C) 2018-2019 Derppening <david.18.19.21@gmail.com>

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

# Get Build Information
execute_process(COMMAND make --version
                RESULT_VARIABLE MAKE_RESULT
                OUTPUT_VARIABLE MAKE_OUTPUT)
if (MAKE_RESULT EQUAL 0)
    string(REGEX REPLACE "GNU Make ([0-9]\\.[0-9]\\.*[0-9]*).+" "\\1" MAKE_VERSION ${MAKE_OUTPUT})
else()
    set(MAKE_VERSION "Not detected")
endif()

message("-----------System Information-----------")
message(STATUS "Host            : ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "GNU Make        : ${MAKE_VERSION}")
message(STATUS "CC              : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "CXX             : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Collect sources and includes
find_file(RTLIB_SRC "src" PATHS "${CMAKE_CURRENT_SOURCE_DIR}/RTLib" "${CMAKE_CURRENT_SOURCE_DIR}")
if (RTLIB_SRC STREQUAL "RTLIB_SRC-NOTFOUND")
    message(FATAL_ERROR "Could not find RTLib sources. Tried:
    - ${CMAKE_CURRENT_SOURCE_DIR}
    - ${CMAKE_CURRENT_SOURCE_DIR}/RTLib")
endif ()

file(GLOB_RECURSE RTLIB_HEADER_FILES "src/*.h")
file(GLOB_RECURSE RTLIB_SOURCE_FILES "${RTLIB_SRC}/*.c" "${RTLIB_SRC}/*.cpp")
include_directories(${RTLIB_SRC})

message("-----------Build Information------------")
message(STATUS "RTLib Path      : ${RTLIB_SRC}")
message(STATUS "RTLib Version   : ${PROJECT_VERSION}")
message(STATUS "Build Type      : ${CMAKE_BUILD_TYPE}")

# Additional Flags
set(ADDITIONAL_C_FLAGS "-fmessage-length=0 -fno-strict-aliasing -ffunction-sections -fdata-sections -fsigned-char")
set(ADDITIONAL_CXX_FLAGS "-fno-exceptions -fno-rtti")
set(ADDITIONAL_LINKER_FLAGS "-nostartfiles -lc -lnosys --specs=rdimon.specs -Wl,-Map,${CMAKE_PROJECT_NAME}_${CMAKE_BUILD_TYPE}.map,--cref")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra ${TARGET_SPECIFIC_FLAGS} -D${MAINBOARD_CONFIG} ${ADDITIONAL_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra ${TARGET_SPECIFIC_FLAGS} -D${MAINBOARD_CONFIG} ${ADDITIONAL_CXX_FLAGS}")
set(LINKER_FLAGS "${LINKER_FLAGS} ${ADDITIONAL_LINKER_FLAGS}")

message("------------Additional Flags------------")
message(STATUS "C   : ${ADDITIONAL_C_FLAGS}")
message(STATUS "CXX : ${ADDITIONAL_CXX_FLAGS}")
message(STATUS "LD  : ${ADDITIONAL_LINKER_FLAGS}")

# Build-dependent flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Og -ggdb")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Og -ggdb")

# Automatically enable validations if building for Debug
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(RTLIB_VALIDATION true)
else()
    set(RTLIB_VALIDATION false)
endif()
add_definitions(-DRTLIB_ENABLE_VALIDATION=${RTLIB_VALIDATION})

# Generate header for RTLib version
configure_file(cmake/template/version.h.in ${RTLIB_SRC}/version.h)

# Include additional compilation flags
include(cmake/compile_flags.cmake)

# Collect sources into SOURCE_FILES variable and include your headers' directory
file(GLOB_RECURSE SOURCE_FILES "src/*.c" "src/*.cpp")
include_directories(src)

# Specify output executable
add_library(${PROJECT_NAME} ${RTLIB_HEADER_FILES} ${RTLIB_SOURCE_FILES})
add_dependencies(${PROJECT_NAME} libopencm3)
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        LINK_FLAGS ${LINKER_FLAGS}
        OUTPUT_NAME "${PROJECT_NAME}_${CMAKE_BUILD_TYPE}")
target_link_libraries(${PROJECT_NAME} "opencm3_${GENLINK_FAMILY}")
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${LIBOPENCM3_DIR}/include>)

# Output elf file size
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${ARM_SIZE} lib${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.a
)
