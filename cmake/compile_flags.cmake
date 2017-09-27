set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(COMMON_WARN_FLAGS "-pedantic -Wall -Wextra \
-Wcast-align -Wcast-qual -Wdisabled-optimization -Wdouble-promotion -Wduplicated-cond -Wfloat-equal -Wformat=2 \
-Winit-self -Winline -Winvalid-pch -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs \
-Wmissing-format-attribute -Wno-unused -Wnull-dereference -Wodr -Wpointer-arith -Wredundant-decls -Wshadow \
-Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wswitch-enum -Wwrite-strings -Wundef -Wuninitialized \
-Wunreachable-code")

set(C_WARN_FLAGS "${COMMON_WARN_FLAGS} -Wbad-function-cast -Wnested-externs -Wmissing-prototypes -Wold-style-definition \
-Wstrict-prototypes")
set(CXX_WARN_FLAGS "${COMMON_WARN_FLAGS} -Wctor-dtor-privacy -Wnoexcept -Wold-style-cast -Woverloaded-virtual \
-Wsign-promo -Wstrict-null-sentinel -Wuseless-cast -Wzero-as-null-pointer-constant")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARN_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARN_FLAGS}")

if (LOG_VERBOSE)
    message(STATUS "Current C warning flags: ${C_WARN_FLAGS}")
    message(STATUS "Current CXX warning flags: ${CXX_WARN_FLAGS}")
    message("-------------------------------------")
endif ()
