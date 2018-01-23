/**
 * @file src/core/gpio.h
 *
 * @brief Helper file for selecting which GPIO class to enable.
 *
 * This file selects which GPIO class to enable according to the @c DEVICE set in @c CMakeLists.txt.
 */

/*
 * This file is part of RTLib.
 *
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

#ifndef RTLIB_CORE_GPIO_H_
#define RTLIB_CORE_GPIO_H_

#include "core/util.h"

#if defined(STM32F1)
#include "core/stm32f1/gpio.h"
#elif defined(STM32F4)
#include "core/stm32f4/gpio.h"
#endif

#endif  // RTLIB_CORE_GPIO_H_
