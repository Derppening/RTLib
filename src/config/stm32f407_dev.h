/**
 * @file src/config/stm32f407_dev.h
 *
 * @brief Pin configurations for STM32F407 development board.
 *
 * Taobao Link: https://m.intl.taobao.com/detail/detail.html?id=524181346955
 *
 * UART Configuration:
 * |          Macro         | TX Pinout | RX Pinout |
 * | :--------------------: | :-------: | :-------: |
 * | @c CORE_UART1_*_PINOUT |    PA9    |    PA10   |
 * | @c CORE_UART2_*_PINOUT |   Unused  |   Unused  |
 * | @c CORE_UART3_*_PINOUT |   Unused  |   Unused  |
 * | @c CORE_UART4_*_PINOUT |   Unused  |   Unused  |
 * | @c CORE_UART6_*_PINOUT |   Unused  |   Unused  |
 *
 * @note Refer to the STM32F407xx Datasheet for alternate pinouts of each UART.
 *
 * Led Configuration:
 * |        Macro       | MCU Pinout | Mainboard Designation | Polarity |
 * | :----------------: | :--------: | :-------------------: | :------: |
 * | @c LIB_LED0_PINOUT |     PA6    |           D2          |  @c true |
 * | @c LIB_LED1_PINOUT |     PA7    |           D3          |  @c true |
 *
 * Button Configuration:
 * |          Macro        | MCU Pinout | Mainboard Designation |   Pull-Up/Pull-Down   |
 * | :-------------------: | :--------: | :-------------------: | :-------------------: |
 * | @c LIB_BUTTON0_PINOUT |     PA0    |          K_UP         |  @c Button::kPullDown |
 * | @c LIB_BUTTON1_PINOUT |     PE4    |           K0          |   @c Button::kPullUp  |
 * | @c LIB_BUTTON2_PINOUT |     PE3    |           K1          |   @c Button::kPullUp  |
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

#ifndef RTLIB_CONFIG_STM32F407_DEV_H_
#define RTLIB_CONFIG_STM32F407_DEV_H_

#if !defined(STM32F407VET6)
#error "This configuration is designed for a STM32F407VET6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F407VET6)

#define DEVICE_SERIES "STM32F4"
#define DEVICE_STRING "STM32F407xx"

#define CORE_UART1_TX_PINOUT {GPIOA, GPIO9}
#define CORE_UART1_RX_PINOUT {GPIOA, GPIO10}
#define CORE_UART2_TX_PINOUT {}
#define CORE_UART2_RX_PINOUT {}
#define CORE_UART3_TX_PINOUT {}
#define CORE_UART3_RX_PINOUT {}
#define CORE_UART4_TX_PINOUT {}
#define CORE_UART4_RX_PINOUT {}
#define CORE_UART6_TX_PINOUT {}
#define CORE_UART6_RX_PINOUT {}

#define LIB_USE_LED 2
#define LIB_LED0_PINOUT {GPIOA, GPIO6}
#define LIB_LED1_PINOUT {GPIOA, GPIO7}

#define LIB_USE_BUTTON 3
#define LIB_BUTTON0_PINOUT {GPIOA, GPIO0}
#define LIB_BUTTON1_PINOUT {GPIOE, GPIO4}
#define LIB_BUTTON2_PINOUT {GPIOE, GPIO3}

#endif  // RTLIB_CONFIG_STM32F407_DEV_H_
