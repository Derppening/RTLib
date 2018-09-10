#[[

This file is part of RTLib.

Copyright (C) 2017 waicool20 <waicool20@gmail.com>
Copyright (C) 2018 Derppening <david.18.19.21@gmail.com>

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

# Originally painfully written by waicool20 :)

# Utility

function(JOIN VALUES GLUE OUTPUT)
    string(REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
    string(REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
    set(${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

# -----------

# Find libopencm3 first
find_file(LIBOPENCM3_DIR "libopencm3" "${CMAKE_CURRENT_SOURCE_DIR}" PATH_SUFFXIES "RTLib")
if (LIBOPENCM3_DIR STREQUAL "LIBOPENCM3_DIR-NOTFOUND")
    message(FATAL_ERROR "Could not locate libopencm3 directory")
endif ()

# include the genlink target
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libopencm3-genlink.cmake)

# ARCH_FLAGS has to be passed as a string here
JOIN("${ARCH_FLAGS}" " " ARCH_FLAGS)
# Set linker flags
set(LINKER_FLAGS "${LINKER_FLAGS} ${LDLIBS} -T${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT} ${ARCH_FLAGS}")
message(STATUS "Linker Flags            : ${LINKER_FLAGS}")

# Compiler flags
set(TARGET_SPECIFIC_FLAGS "${GENLINK_CPPFLAGS} ${ARCH_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_SPECIFIC_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TARGET_SPECIFIC_FLAGS}")

message(STATUS "Target Specific Flags   : ${TARGET_SPECIFIC_FLAGS}")

# Add targets to build libopencm3
add_custom_target(
        libopencm3 ${CMAKE_MAKE_PROGRAM} -j${CMAKE_JOB_POOL_COMPILE}
        DEPENDS libopencm3-genlink
        WORKING_DIRECTORY ${LIBOPENCM3_DIR}
)
add_custom_target(
        libopencm3-clean ${CMAKE_MAKE_PROGRAM} clean -j${CMAKE_JOB_POOL_COMPILE}
        WORKING_DIRECTORY ${LIBOPENCM3_DIR}
)
link_directories(${LIBOPENCM3_DIR}/lib)
include_directories(SYSTEM ${LIBOPENCM3_DIR}/include)
