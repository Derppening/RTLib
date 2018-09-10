/**
 * @file src/config/example_stm32f4.h
 *
 * @brief Example configuration for STM32F4-series MCUs.
 *
 * This configuration file is an example on how to create your own mainboard configurations with STM32F4-series MCUs.
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

#ifndef RTLIB_CONFIG_EXAMPLE_STM32F4_H_
#define RTLIB_CONFIG_EXAMPLE_STM32F4_H_

// Use !defined(...) to check if the appropriate device is targeted before proceeding
// Replace ... in the brackets to the MCU model of your target device.
#if !defined(STM32F407VET6)
#error "This configuration is designed for a STM32F407VET6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F407VET6)


// You must define two constants 'kDeviceSeries' and 'kDeviceString', which corresponds to the MCU series and MCU model
// respectively. No checks will be done internally, so you are responsible for the correctness of these constant.
#define DEVICE_SERIES "STM32F4"
#define DEVICE_STRING "STM32F407xx"

// If this define is specified, the destructor of objects will also reset the peripheral to the default state. Enable
// this to (potentially) reclaim some space for the user space application.
#define DTOR_RESET_PERIPH

// If a pin is unused or not designated, leave the curly braces empty ("{}").

// For UART, you must define all following constants.
#define CORE_UART1_TX_PINOUT {}
#define CORE_UART1_RX_PINOUT {}
#define CORE_UART2_TX_PINOUT {}
#define CORE_UART2_RX_PINOUT {}
#define CORE_UART3_TX_PINOUT {}
#define CORE_UART3_RX_PINOUT {}
#define CORE_UART4_TX_PINOUT {}
#define CORE_UART4_RX_PINOUT {}
#define CORE_UART6_TX_PINOUT {}
#define CORE_UART6_RX_PINOUT {}

// You must define all LIB_USE_*. If your board does not have the feature, or you do not intend to expose the API to
// other users, set the macro to 0. A compilation error will occur if a user tries to include a library which is
// disabled in this file.
#define LIB_USE_BUTTON 0
#define LIB_USE_LED 1

// To use wrapper classes for any devices under lib, define all pin outputs here.
// The format of the macro is
// #define LIB_[DEVICE][x]_PINOUT {[GPIO], [GPIO_PIN]}
// where [DEVICE] is the device under lib, [x] is the ID of the device, [GPIO] is the GPIO, and [GPIO_PIN] is the pin.
// All devices IDs in this file start from 0, and corresponds to 0 in Config structs.
#define LIB_LED0_PINOUT {GPIOB, GPIO0}

#endif  // RTLIB_CONFIG_EXAMPLE_STM32F4_H_
