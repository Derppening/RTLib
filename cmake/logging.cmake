#[[

This file is part of RTLib.

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

message("-------------VERBOSE LOGGING------------")

# From cmake/rtlib.cmake
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message("-------------Full Flag List-------------")
message(STATUS "C   : ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${BUILD_TYPE}}")
message(STATUS "CXX : ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BUILD_TYPE}}")
message(STATUS "LD  : ${LINKER_FLAGS}")

# From cmake/compile_flags.cmake
message("-------------Warning Flags--------------")
message(STATUS "C   : ${C_WARN_FLAGS}")
message(STATUS "CXX : ${CXX_WARN_FLAGS}")
