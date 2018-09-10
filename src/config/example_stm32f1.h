/**
 * @file src/config/example_stm32f1.h
 *
 * @brief Example configuration for STM32F1-series MCUs.
 *
 * This configuration file is an example on how to create your own mainboard configurations with STM32F1-series MCUs.
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

#ifndef RTLIB_CONFIG_EXAMPLE_STM32F1_H_
#define RTLIB_CONFIG_EXAMPLE_STM32F1_H_

// Use !defined(...) to check if the appropriate device is targeted before proceeding
// Replace ... in the brackets to the MCU model of your target device.
#if !defined(STM32F103VCT6)
#error "This configuration is designed for a STM32F103VCT6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F103VCT6)


// You must define two constants 'kDeviceSeries' and 'kDeviceString', which corresponds to the MCU series and MCU model
// respectively. No checks will be done internally, so you are responsible for the correctness of these constant.
#define DEVICE_SERIES "STM32F1"
#define DEVICE_STRING "STM32F103xx"

// If this define is specified, the destructor of objects will also reset the peripheral to the default state. Enable
// this to (potentially) reclaim some space for the user space application.
#define DTOR_RESET_PERIPH

// If this define is specified, the experimental version of GPIO will be enabled. Note that this may increase the binary
// size of the application.
#define USE_GPIO_V2

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

// Other devices may require more than one pinout. These will also be defined here.
// TODO(Derppening): Add example from UART.

#endif  // RTLIB_CONFIG_EXAMPLE_STM32F1_H_
