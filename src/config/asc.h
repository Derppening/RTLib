/**
 * @file src/config/asc.h
 *
 * @brief Pin configurations for Android Server Control board v1.2.
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

#ifndef RTLIB_CONFIG_ASC_H_
#define RTLIB_CONFIG_ASC_H_

constexpr const char kDeviceSeries[] = "STM32F1";
constexpr const char kDeviceString[] = "STM32F105xx";

#if !defined(STM32F105RBT6)
#error "This configuration is designed for a STM32F105RBT6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F105RBT6)

#define LIB_USE_BUTTON 0

#define LIB_USE_LED 3
#define LIB_LED0_PINOUT {GPIOB, GPIO12}
#define LIB_LED1_PINOUT {GPIOB, GPIO13}
#define LIB_LED2_PINOUT {GPIOB, GPIO14}

#endif  // RTLIB_CONFIG_ASC_H_
