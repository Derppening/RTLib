/**
 * @file src/config/ups.h
 *
 * @brief Pin configurations for BM v1.0.
 *
 * This board is a proprietary design; there are currently no means of obtaining such boards.
 *
 * Led Configuration:
 * |        Macro       | MCU Pinout | Mainboard Designation | Polarity |
 * | :----------------: | :--------: | :-------------------: | :------: |
 * | @c LIB_LED0_PINOUT |    PC13    |           D1          | @c false |
 * | @c LIB_LED1_PINOUT |    PC14    |           D2          | @c false |
 * | @c LIB_LED2_PINOUT |    PC15    |           D3          | @c false |
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

#ifndef RTLIB_CONFIG_UPS_H_
#define RTLIB_CONFIG_UPS_H_

#define DEVICE_SERIES "STM32F1"
#define DEVICE_STRING "STM32F105xx"

#if !defined(STM32F105RBT6)
#error "This configuration is designed for a STM32F105RBT6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F105RBT6)

#define LIB_USE_BUTTON 0

#define LIB_USE_LED 3
#define LIB_LED0_PINOUT {GPIOC, GPIO13}
#define LIB_LED1_PINOUT {GPIOC, GPIO14}
#define LIB_LED2_PINOUT {GPIOC, GPIO15}

#endif  // RTLIB_CONFIG_UPS_H_
