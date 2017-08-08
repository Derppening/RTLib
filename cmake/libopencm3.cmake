# Also painfully written by waicool20 :)

# Utility

function(JOIN VALUES GLUE OUTPUT)
    string(REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
    string(REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
    set(${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

# -----------

# LibOpenCM3 Stuff
set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/libopencm3)
add_custom_target(
        libopencm3 make
        WORKING_DIRECTORY ${LIBOPENCM3_DIR}
)
link_directories(${LIBOPENCM3_DIR}/lib)
include_directories(${LIBOPENCM3_DIR}/include)

# Generate linker information for device, based on libopencm3/mk/genlink-config.mk

if (NOT DEVICE)
    message(FATAL_ERROR "No DEVICE specified for linker script generator")
endif ()


# Check if `awk` program is available
find_program(AWK awk)
if (NOT AWK)
    message(FATAL_ERROR "awk is required to generate the linker script, please install it.")
endif ()

set(GENLINK_SCRIPT "${CMAKE_SOURCE_DIR}/libopencm3/scripts/genlink.awk")
set(DEVICES_DATA "${CMAKE_SOURCE_DIR}/libopencm3/ld/devices.data")
execute_process(
        COMMAND awk "-v" "PAT=${DEVICE}" "-v" "MODE=FAMILY" "-f" "${GENLINK_SCRIPT}" "${DEVICES_DATA}"
        OUTPUT_VARIABLE GENLINK_FAMILY
)
execute_process(
        COMMAND awk "-v" "PAT=${DEVICE}" "-v" "MODE=SUBFAMILY" "-f" "${GENLINK_SCRIPT}" "${DEVICES_DATA}"
        OUTPUT_VARIABLE GENLINK_SUBFAMILY
)
execute_process(
        COMMAND awk "-v" "PAT=${DEVICE}" "-v" "MODE=CPU" "-f" "${GENLINK_SCRIPT}" "${DEVICES_DATA}"
        OUTPUT_VARIABLE GENLINK_CPU
)
execute_process(
        COMMAND awk "-v" "PAT=${DEVICE}" "-v" "MODE=FPU" "-f" "${GENLINK_SCRIPT}" "${DEVICES_DATA}"
        OUTPUT_VARIABLE GENLINK_FPU
)
execute_process(
        COMMAND awk "-v" "PAT=${DEVICE}" "-v" "MODE=CPPFLAGS" "-f" "${GENLINK_SCRIPT}" "${DEVICES_DATA}"
        OUTPUT_VARIABLE GENLINK_CPPFLAGS
)
execute_process(
        COMMAND awk "-v" "PAT=${DEVICE}" "-v" "MODE=DEFS" "-f" "${GENLINK_SCRIPT}" "${DEVICES_DATA}"
        OUTPUT_VARIABLE GENLINK_DEFS
)
message("-------------------------------------")
message(STATUS "Fun Fact: This CMake file was painfully written by waicool20")
message("-------------------------------------")
message(STATUS "Device: ${DEVICE}")
message("---------Device Linker Info----------")
message(STATUS "Family: ${GENLINK_FAMILY}")
message(STATUS "Sub-family: ${GENLINK_SUBFAMILY}")
message(STATUS "CPU: ${GENLINK_CPU}")
message(STATUS "FPU: ${GENLINK_FPU}")
message(STATUS "CPP flags: ${GENLINK_CPPFLAGS}")
message(STATUS "Linker definitions: ${GENLINK_DEFS}")
message("-------------------------------------")

# Generate flags

set(ARCH_FLAGS "-mcpu=${GENLINK_CPU}")

# Check CPU
set(CORTEX_CPU cortex-m0 cortex-m0plus cortex-m3 cortex-m4 cortex-m7)
list(FILTER CORTEX_CPU INCLUDE REGEX ${GENLINK_CPU})
if (GENLINK_CPU STREQUAL CORTEX_CPU)
    list(APPEND ARCH_FLAGS "-mthumb")
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
execute_process(
        COMMAND ${ARM_CXX} ${ARCH_FLAGS} ${GENLINK_DEFS} "-P" "-E" "${LIBOPENCM3_DIR}/ld/linker.ld.S"
        OUTPUT_FILE "${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT}"
)
message(STATUS "Generated linker file: ${LINKER_SCRIPT}")


# ARCH_FLAGS has to be passed as a string here
JOIN("${ARCH_FLAGS}" " " ARCH_FLAGS)
# Set linker flags
set(LINKER_FLAGS "${LINKER_FLAGS} ${LDLIBS} -T${CMAKE_SOURCE_DIR}/${LINKER_SCRIPT} ${ARCH_FLAGS}")
message(STATUS "Current linker flags: ${LINKER_FLAGS}")

# Compiler flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GENLINK_CPPFLAGS} ${ARCH_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GENLINK_CPPFLAGS} ${ARCH_FLAGS}")

message(STATUS "Current C flags: ${CMAKE_C_FLAGS}")
message(STATUS "Current CXX flags: ${CMAKE_CXX_FLAGS}")
message("-------------------------------------")

# Replace `add_executable` with custom macro with same name that adds libopencm3 as a linking target
macro(add_executable _name)
    # invoke built-in add_executable
    _add_executable(${ARGV})
    if (TARGET ${_name})
        set_target_properties(${ARGV0} PROPERTIES LINK_FLAGS ${LINKER_FLAGS})
        target_link_libraries(${_name} "opencm3_${GENLINK_FAMILY}")
        string(REGEX MATCH "^(.*)\\.[^.]*$" dummy "${ARGV0}")
        set(bin ${CMAKE_MATCH_1}.bin)
        set(elf ${ARGV0})
        add_custom_target(
                ${bin} ALL
                COMMAND ${ARM_OBJCOPY} -Obinary ${elf} ${bin}
                WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                DEPENDS ${elf}
        )
    endif ()
endmacro()


