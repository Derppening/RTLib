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

# Generate linker information for device, based on libopencm3/mk/genlink-config.mk
if (NOT DEVICE)
    message(FATAL_ERROR "No DEVICE specified for linker script generator")
endif ()


find_program(PYTHON python)
if (NOT PYTHON)
    message(FATAL_ERROR "python is required to generate the linker script, please install it.")
endif ()

set(GENLINK_SCRIPT "${LIBOPENCM3_DIR}/scripts/genlink.py")
set(DEVICES_DATA "${LIBOPENCM3_DIR}/ld/devices.data")
execute_process(
        COMMAND "${PYTHON}" "${GENLINK_SCRIPT}" "${DEVICES_DATA}" "${DEVICE}" "FAMILY"
        OUTPUT_VARIABLE GENLINK_FAMILY
)
execute_process(
        COMMAND "${PYTHON}" "${GENLINK_SCRIPT}" "${DEVICES_DATA}" "${DEVICE}" "USBFAMILY"
        OUTPUT_VARIABLE GENLINK_SUBFAMILY
)
execute_process(
        COMMAND "${PYTHON}" "${GENLINK_SCRIPT}" "${DEVICES_DATA}" "${DEVICE}" "CPU"
        OUTPUT_VARIABLE GENLINK_CPU
)
execute_process(
        COMMAND "${PYTHON}" "${GENLINK_SCRIPT}" "${DEVICES_DATA}" "${DEVICE}" "FPU"
        OUTPUT_VARIABLE GENLINK_FPU
)
execute_process(
        COMMAND "${PYTHON}" "${GENLINK_SCRIPT}" "${DEVICES_DATA}" "${DEVICE}" "CPPFLAGS"
        OUTPUT_VARIABLE GENLINK_CPPFLAGS
)
execute_process(
        COMMAND "${PYTHON}" "${GENLINK_SCRIPT}" "${DEVICES_DATA}" "${DEVICE}" "DEFS"
        OUTPUT_VARIABLE GENLINK_DEFS
)

message("----------------------------------------")
message(STATUS "Fun Fact: This CMake file was painfully written by waicool20 and Derppening")
message("----------------------------------------")
message(STATUS "Device                  : ${DEVICE}")
message("-----------Device Linker Info-----------")
message(STATUS "Family                  : ${GENLINK_FAMILY}")
message(STATUS "Sub-family              : ${GENLINK_SUBFAMILY}")
message(STATUS "CPU                     : ${GENLINK_CPU}")
message(STATUS "FPU                     : ${GENLINK_FPU}")
message(STATUS "CPP Flags               : ${GENLINK_CPPFLAGS}")
message(STATUS "Linker Defs             : ${GENLINK_DEFS}")

# Generate flags
set(ARCH_FLAGS "-mcpu=${GENLINK_CPU}")

# Check CPU
set(CORTEX_CPU cortex-m0 cortex-m0plus cortex-m3 cortex-m4 cortex-m7)
list(FILTER CORTEX_CPU INCLUDE REGEX ${GENLINK_CPU})
if (GENLINK_CPU STREQUAL CORTEX_CPU)
    list(APPEND ARCH_FLAGS "-mthumb -mthumb-interwork")
endif ()

# Check FPU
if (GENLINK_FPU STREQUAL "soft")
    list(APPEND ARCH_FLAGS "-msoft-float")
elseif (GENLINK_FPU STREQUAL "hard-fpv4-sp-d16")
    list(APPEND ARCH_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
elseif (GENLINK_FPU STREQUAL "hard-fpv5-sp-d16")
    list(APPEND ARCH_FLAGS "-mfloat-abi=hard -mfpu=fpv5-sp-d16")
else ()
    message(WARNING "No match for the FPU flags")
endif ()

# Check family
if (NOT GENLINK_FAMILY)
    message(WARNING "${DEVICE} not found in ${DEVICES_DATA}")
endif ()

# Linker stuff
set(LINKER_SCRIPT "generated.${DEVICE}.ld")

if (EXISTS "${LIBOPENCM3_DIR}/lib/libopencm3_${GENLINK_FAMILY}.ld")
    set(LDLIBS "${LDLIBS} -lopencm3_${GENLINK_FAMILY}")
else ()
    if (EXISTS "${LIBOPENCM3_DIR}/lib/libopencm3_${GENLINK_SUBFAMILY}.ld")
        set(LDLIBS "${LDLIBS} -lopencm3_${GENLINK_SUBFAMILY}")
    else ()
        message(FATAL_ERROR "${LIBOPENCM3_DIR}/lib/libopencm3_${GENLINK_FAMILY}: A library variant for the selected device does not exist.")
    endif ()
endif ()

message(STATUS "Linker File Destination : .../${LINKER_SCRIPT}")

message("-----------Target Specific Info---------")

# ARCH_FLAGS and GENLINK_DEFS has to be passed as a list here
string(REPLACE " " ";" GENLINK_DEFS ${GENLINK_DEFS})
# Get rid of any spaces and turn the thing into a list
JOIN("${ARCH_FLAGS}" " " ARCH_FLAGS)
string(REPLACE " " ";" ARCH_FLAGS ${ARCH_FLAGS})

add_custom_target(
        libopencm3-genlink
        COMMAND ${ARM_CXX} ${ARCH_FLAGS} ${GENLINK_DEFS} "-P" "-E" "${LIBOPENCM3_DIR}/ld/linker.ld.S" > "${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
