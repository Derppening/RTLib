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

#if defined(STM32F1)
#define CORE_NS core::stm32f1
#elif defined(STM32F4)
#define CORE_NS core::stm32f4
#endif

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

#endif  // RTLIB_CORE_UTIL_H_
