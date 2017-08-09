# Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ${TARGET_FLAGS} -Os -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 ${TARGET_FLAGS}")
set(LINKER_FLAGS "${LINKER_FLAGS} -nostartfiles -lc -lnosys --specs=rdimon.specs -Wl,--gc-sections")

# Collect sources and includes

find_file(RTLIB_SRC "src" PATHS "${CMAKE_CURRENT_SOURCE_DIR}/RTLib" "${CMAKE_CURRENT_SOURCE_DIR}")
find_file(RTLIB_INC "inc" PATHS "${CMAKE_CURRENT_SOURCE_DIR}/RTLib" "${CMAKE_CURRENT_SOURCE_DIR}")

if (RTLIB_SRC STREQUAL "RTLIB_SRC-NOTFOUND")
    message(FATAL_ERROR "Could not find RTLib sources")
endif ()

if (RTLIB_INC STREQUAL "RTLIB_INC-NOTFOUND")
    message(FATAL_ERROR "Could not find RTLib includes")
endif ()

file(GLOB_RECURSE RTLIB_SOURCE_FILES "${RTLIB_SRC}/*.c" "${RTLIB_SRC}/*.cpp")
include_directories(${RTLIB_INC})
