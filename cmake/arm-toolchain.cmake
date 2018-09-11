#[[

This file is part of RTLib.

Copyright (C) 2017 waicool20 <waicool20@gmail.com>

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

# Use ARM-GCC for crosscompiling
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(ARM_PREFIX arm-none-eabi)

find_program(ARM_CC ${ARM_PREFIX}-gcc)
find_program(ARM_CXX ${ARM_PREFIX}-g++)
find_program(ARM_LD ${ARM_PREFIX}-gcc)
find_program(ARM_AR ${ARM_PREFIX}-ar)
find_program(ARM_AS ${ARM_PREFIX}-as)
find_program(ARM_OBJCOPY ${ARM_PREFIX}-objcopy)
find_program(ARM_OBJDUMP ${ARM_PREFIX}-objdump)
find_program(ARM_GDB ${ARM_PREFIX}-gdb)
find_program(ARM_SIZE ${ARM_PREFIX}-size)


SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_C_COMPILER ${ARM_CC})
set(CMAKE_CXX_COMPILER ${ARM_CXX})
