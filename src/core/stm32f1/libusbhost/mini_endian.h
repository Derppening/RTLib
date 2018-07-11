/*
 * This file is part of RTLib, ported from the libusbhost library
 * (https://github.com/libusbhost/libusbhost).
 *
 * Copyright (C) 2015 Amir Hammad <amir.hammad@hotmail.com>
 * Copyright (C) 2018 Derppening <david.18.19.21@gmail.com>
 *
 * RTLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RTLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RTLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RTLIB_CORE_STM32F1_LIBUSBHOST_MINI_ENDIAN_H_
#define RTLIB_CORE_STM32F1_LIBUSBHOST_MINI_ENDIAN_H_

#include <machine/endian.h>

/* Mini endian  BEGIN */
# if BYTE_ORDER == LITTLE_ENDIAN
#  define htobe16(x) byteswap_16 (x)
#  define htole16(x) (x)
#  define be16toh(x) byteswap_16 (x)
#  define le16toh(x) (x)

#  define htobe32(x) byteswap_32 (x)
#  define htole32(x) (x)
#  define be32toh(x) byteswap_32 (x)
#  define le32toh(x) (x)

# else
#  define htobe16(x) (x)
#  define htole16(x) byteswap_16 (x)
#  define be16toh(x) (x)
#  define le16toh(x) byteswap_16 (x)

#  define htobe32(x) (x)
#  define htole32(x) byteswap_32 (x)
#  define be32toh(x) (x)
#  define le32toh(x) byteswap_32 (x)

#  define htobe64(x) (x)
#  define htole64(x) byteswap_64 (x)
#  define be64toh(x) (x)
#  define le64toh(x) byteswap_64 (x)
# endif

/**
 * @brief Swaps the endianness of a 16-bit integer.
 *
 * @param val Input value.
 * @return A 16-bit integer with the opposite endianness.
 */
static inline uint16_t byteswap_16(uint16_t val) {
  union {
    uint16_t num;
    uint8_t arr[2];
  } in, ret;
  in.num = val;

  ret.arr[0] = in.arr[1];
  ret.arr[1] = in.arr[0];
  return ret.num;
}

/**
 * @brief Swaps the endianness of a 32-bit integer.
 *
 * @param val Input value.
 * @return A 32-bit integer with the opposite endianness.
 */
static inline uint32_t byteswap_32(uint32_t val) {
  union {
    uint32_t num;
    uint8_t arr[4];
  } in, ret;
  in.num = val;

  ret.arr[0] = in.arr[3];
  ret.arr[1] = in.arr[2];
  ret.arr[2] = in.arr[1];
  ret.arr[3] = in.arr[0];
  return ret.num;
}

#endif  // RTLIB_CORE_STM32F1_LIBUSBHOST_MINI_ENDIAN_H_
