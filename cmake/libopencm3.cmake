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

# LibOpenCM3 Stuff
find_file(LIBOPENCM3_DIR "libopencm3" "${CMAKE_CURRENT_SOURCE_DIR}" PATH_SUFFXIES "RTLib")
if (LIBOPENCM3_DIR STREQUAL "LIBOPENCM3_DIR-NOTFOUND")
    message(FATAL_ERROR "Could not locate libopencm3 directory")
endif ()

add_custom_target(
        libopencm3 make
        WORKING_DIRECTORY ${LIBOPENCM3_DIR}
)
link_directories(${LIBOPENCM3_DIR}/lib)
include_directories(SYSTEM ${LIBOPENCM3_DIR}/include)


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
message(STATUS "Device      : ${DEVICE}")
message("-----------Device Linker Info-----------")
message(STATUS "Family      : ${GENLINK_FAMILY}")
message(STATUS "Sub-family  : ${GENLINK_SUBFAMILY}")
message(STATUS "CPU         : ${GENLINK_CPU}")
message(STATUS "FPU         : ${GENLINK_FPU}")
message(STATUS "CPP Flags   : ${GENLINK_CPPFLAGS}")
message(STATUS "Linker Defs : ${GENLINK_DEFS}")

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

if (EXISTS "${LIBOPENCM3_DIR}/lib/libopencm3_${GENLINK_FAMILY}.a")
    set(LDLIBS "${LDLIBS} -lopencm3_${GENLINK_FAMILY}")
else ()
    if (EXISTS "${LIBOPENCM3_DIR}/lib/libopencm3_${GENLINK_SUBFAMILY}.a")
        set(LDLIBS "${LDLIBS} -lopencm3_${GENLINK_SUBFAMILY}")
    else ()
        message(WARNING "${LIBOPENCM3_DIR}/lib/libopencm3_${GENLINK_FAMILY}. A library variant for the selected device does not exist.")
    endif ()
endif ()

# ARCH_FLAGS and GENLINK_DEFS has to be passed as a list here
string(REPLACE " " ";" GENLINK_DEFS ${GENLINK_DEFS})
# Get rid of any spaces and turn the thing into a list
JOIN("${ARCH_FLAGS}" " " ARCH_FLAGS)
string(REPLACE " " ";" ARCH_FLAGS ${ARCH_FLAGS})
# ------------------
execute_process(
        COMMAND ${ARM_CXX} ${ARCH_FLAGS} ${GENLINK_DEFS} "-P" "-E" "${LIBOPENCM3_DIR}/ld/linker.ld.S"
        OUTPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT}"
)
message("-----------Target Specific Info---------")
message(STATUS "Generated Linker File   : .../${LINKER_SCRIPT}")


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

# Replace `add_executable` with custom macro with same name that adds libopencm3 as a linking target
macro(add_executable _name)
    # invoke built-in add_executable
    if ("${ARGV1}" STREQUAL "")
        message(FATAL_ERROR "No source files added to executable")
    endif ()
    _add_executable(${ARGV})

    if (TARGET ${_name} AND NOT ${_name} MATCHES ".*Doxygen.*")
        # Set target properties
        set_target_properties(${ARGV0} PROPERTIES LINK_FLAGS ${LINKER_FLAGS})
        set_target_properties(${PROJECT_NAME}.elf PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_${CMAKE_BUILD_TYPE}.elf")
        target_link_libraries(${_name} "opencm3_${GENLINK_FAMILY}")
        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "../build/RTLib_${CMAKE_BUILD_TYPE}.bin")

        # Set output file locations
        string(REGEX MATCH "^(.*)\\.[^.]*$" dummy "${ARGV0}")
        set(bin ${CMAKE_MATCH_1}.bin)
        set(elf ${ARGV0})
        set(bin_out ${CMAKE_MATCH_1}_${CMAKE_BUILD_TYPE}.bin)
        get_target_property(elf_in ${PROJECT_NAME}.elf OUTPUT_NAME)

        # Add target
        add_custom_target(
                ${bin} ALL
                COMMAND ${ARM_OBJCOPY} -Obinary ${elf_in} ${bin_out}
                WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                DEPENDS ${elf}
        )

        message("------------Output Locations------------")
        message(STATUS "BIN: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${bin_out}")
        message(STATUS "ELF: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${elf_in}")
    endif ()
endmacro()


