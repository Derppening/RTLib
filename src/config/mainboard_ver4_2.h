/**
 * @file src/config/mainboard_ver4_2.h
 *
 * @brief Pin configurations for Mainboard Version 4.2, designed by Chan Tsz Ho.
 *
 * Led Configuration:
 * |        Macro       | MCU Pinout | Mainboard Designation | Polarity |
 * | :----------------: | :--------: | :-------------------: | :------: |
 * | @c LIB_LED0_PINOUT |    PB12    |           D1          | @c false |
 * | @c LIB_LED1_PINOUT |    PB13    |           D2          | @c false |
 * | @c LIB_LED2_PINOUT |    PB14    |           D3          | @c false |
 *
 * Button Configuration:
 * |          Macro        | MCU Pinout | Mainboard Designation | Active High? |
 * | :-------------------: | :--------: | :-------------------: | :----------: |
 * | @c LIB_BUTTON0_PINOUT |     PB6    |        Button_1       |    @c false  |
 * | @c LIB_BUTTON1_PINOUT |     PB7    |        Button_2       |    @c false  |
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

#ifndef RTLIB_CONFIG_MAINBOARD_VER4_2_H_
#define RTLIB_CONFIG_MAINBOARD_VER4_2_H_

#if !defined(STM32F103VCT6)
#error "This configuration is designed for a STM32F103VCT6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F103VCT6)

constexpr const char kDeviceSeries[] = "STM32F1";
constexpr const char kDeviceString[] = "STM32F103xx";

#define LIB_USE_LED 3
#define LIB_LED0_PINOUT {GPIOB, GPIO12}
#define LIB_LED1_PINOUT {GPIOB, GPIO13}
#define LIB_LED2_PINOUT {GPIOB, GPIO14}

#define LIB_USE_BUTTON 2
#define LIB_BUTTON0_PINOUT {GPIOB, GPIO6}
#define LIB_BUTTON1_PINOUT {GPIOB, GPIO7}

#endif  // RTLIB_CONFIG_MAINBOARD_VER4_2_H_
