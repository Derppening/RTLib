/**
 * @file src/config/config.h
 *
 * @brief Helper file for selecting device configurations.
 *
 * This file selects which device configuration to use according to the definition given in @c CMakeLists.txt. If such
 * configuration is not, this file will throw an error indicating such.
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

#ifndef RTLIB_CONFIG_CONFIG_H_
#define RTLIB_CONFIG_CONFIG_H_

#if defined(EXAMPLE_STM32F1)
#include "example_stm32f1.h"
#elif defined(EXAMPLE_STM32F4)
#include "example_stm32f4.h"
#elif defined(MAINBOARD_VER4_2)
#include "mainboard_ver4_2.h"
#elif defined(STM32F103_DEV)
#include "config/stm32f103_dev.h"
#elif defined(STM32F407_DEV)
#include "config/stm32f407_dev.h"
#elif defined(ASC)
#include "config/asc.h"
#elif defined(UPS)
#include "config/ups.h"
#else
#error "No known configuration is specified. Please specify a target mainboard in CMakeLists.txt"
#endif

#endif  // RTLIB_CONFIG_CONFIG_H_
