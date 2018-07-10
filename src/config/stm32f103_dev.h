/**
 * @file src/config/stm32f103_dev.h
 *
 * @brief Pin configurations for STM32F103 development board.
 *
 * Taobao Link: https://detail.tmall.com/item.htm?id=549940576891&toSite=main
 *
 * Led Configuration:
 * |        Macro       | MCU Pinout | Mainboard Designation | Polarity |
 * | :----------------: | :--------: | :-------------------: | :------: |
 * | @c LIB_LED0_PINOUT |     PB0    |          LED1         |  @c true |
 *
 * Button Configuration:
 * |          Macro        | MCU Pinout | Mainboard Designation | Active High? |
 * | :-------------------: | :--------: | :-------------------: | :----------: |
 * | @c LIB_BUTTON0_PINOUT |     PE6    |          KEY1         |   @c false   |
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

#ifndef RTLIB_CONFIG_STM32F103_DEV_H_
#define RTLIB_CONFIG_STM32F103_DEV_H_

#if !defined(STM32F103VCT6)
#error "This configuration is designed for a STM32F103VCT6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F103VCT6)

#define DEVICE_SERIES "STM32F1"
#define DEVICE_STRING "STM32F103xx"

#define LIB_USE_LED 1
#define LIB_LED0_PINOUT {GPIOB, GPIO0}

#define LIB_USE_BUTTON 1
#define LIB_BUTTON0_PINOUT {GPIOE, GPIO6}

#endif  // RTLIB_CONFIG_STM32F103_DEV_H_
