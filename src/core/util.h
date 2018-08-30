/**
 * @file src/core/util.h
 *
 * @brief Utility helpers for core library.
 */

/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2017-2018 Derppening <david.18.19.21@gmail.com>
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

#ifndef RTLIB_CORE_UTIL_H_
#define RTLIB_CORE_UTIL_H_

#include <cstdint>
#include <utility>

namespace rtlib {
namespace core {

/**
 * @brief Type definition for MCU Port.
 */
using Port = unsigned;
/**
 * @brief Type definition for MCU Pin.
 */
using Pin = uint16_t;
/**
 * @brief Type definition for MCU Pinout.
 */
using Pinout = std::pair<Port, Pin>;

/**
   * @brief Constant representing an invalid pinout.
   */
constexpr const Pinout kNullPinout = {Port(-1), Pin(-1)};

/**
 * @brief Compares two c-strings in compile time.
 *
 * @param[in] a A string
 * @param[in] b Another string
 *
 * @return Whether the c-strings are the same
 */
constexpr bool StringCompare(const char* a, const char* b) {
  return *a == *b && (*a == '\0' || StringCompare(a + 1, b + 1));
}

/**
 * @brief Returns the address of a given RCC register.
 *
 * @param base Base address.
 * @param bit Bit offset.
 * @return Address of RCC register.
 */
constexpr std::uint32_t GetRCCRegister(std::uint8_t base, std::uint8_t bit) {
  return std::uint32_t((base << 5) + bit);
}

namespace stm32f1 {}
namespace stm32f4 {}

#if defined(STM32F1)
namespace device = stm32f1;
#elif defined(STM32F4)
namespace device = stm32f4;
#endif
}  // namespace core

namespace lib {}
namespace util {}
}  // namespace rtlib

namespace libcore = rtlib::core;
#if defined(STM32F1)
namespace libdev = libcore::stm32f1;
#elif defined(STM32F4)
namespace libdev = libcore::stm32f4;
#endif
namespace libs = rtlib::lib;
namespace libutil = rtlib::util;

#endif  // RTLIB_CORE_UTIL_H_
