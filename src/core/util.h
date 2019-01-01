/**
 * @file src/core/util.h
 *
 * @brief Utility helpers for core library.
 */

/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2017-2019 Derppening <david.18.19.21@gmail.com>
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

#include "stm32/utils/rcc.h"

namespace rtlib {
namespace core {

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

[[deprecated("Replaced by rtlib::core::stm32::utils::rcc_get_reg.")]]
constexpr std::uint32_t GetRCCRegister(std::uint8_t base, std::uint8_t bit) {
  return rtlib::core::stm32::utils::rcc_get_reg(base, bit);
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
