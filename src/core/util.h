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

#pragma once

#include <cstdint>

#if defined(STM32F1)
#include "core/stm32/f1/util.h"
#endif

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
constexpr bool strcmp(const char* a, const char* b) {
  return *a == *b && (*a == '\0' || strcmp(a + 1, b + 1));
}
}  // namespace core

namespace lib {}
namespace util {}
}  // namespace rtlib

namespace libcore = rtlib::core;
namespace libdev = libcore::device;
namespace libs = rtlib::lib;
namespace libutil = rtlib::util;
