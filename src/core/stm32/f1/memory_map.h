/**
 * @file src/core/stm32/f1/memory_map.h
 *
 * @brief Memory map for STM32F1 devices.
 */

/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2019 Derppening <david.18.19.21@gmail.com>
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

#pragma once

#if defined(STM32F1)

#include <cstdint>
#include <type_traits>

#include "core/memory_map.h"

namespace rtlib::core::stm32::f1 {

enum struct reg {
  // Peripheral Registers
  reg_periph,

  // Buses
  bus_apb1,
  bus_apb2,
  bus_ahb,

  // APB1
  periph_tim2,
  periph_tim3,
  periph_tim4,
  periph_tim5,
  periph_tim6,
  periph_tim7,
  periph_tim12,
  periph_tim13,
  periph_tim14,
  periph_rtc,
  periph_wwdg,
  periph_iwdg,
  periph_spi2,
  periph_spi3,
  periph_usart2,
  periph_usart3,
  periph_uart4,
  periph_uart5,
  periph_i2c1,
  periph_i2c2,
  periph_usb_fs,
  periph_shr_usb_can_sram,
  periph_bx_can1,
  periph_bx_can2,
  periph_bkp,
  periph_pwr,
  periph_dac,

  // APB2
  periph_afio,
  periph_exti,
  periph_gpioa,
  periph_gpiob,
  periph_gpioc,
  periph_gpiod,
  periph_gpioe,
  periph_gpiof,
  periph_gpiog,
  periph_adc1,
  periph_adc2,
  periph_tim1,
  periph_spi1,
  periph_tim8,
  periph_usart1,
  periph_adc3,
  periph_tim9,
  periph_tim10,
  periph_tim11,

  // AHB
  periph_sdio,
  periph_dma1,
  periph_dma2,
  periph_rcc,
  periph_flash,
  periph_crc,
  periph_eth,
  periph_usb_otg_fs,
  periph_fsmc
};

template<reg R>
struct memory_map;

template<>
struct memory_map<reg::reg_periph> {
  static constexpr std::uint32_t begin = 0x4000'0000;
};

template<>
struct memory_map<reg::bus_apb1> {
  static constexpr std::uint32_t begin = memory_map<reg::reg_periph>::begin + 0x0000;
};

template<>
struct memory_map<reg::bus_apb2> {
  static constexpr std::uint32_t begin = memory_map<reg::reg_periph>::begin + 0x1'0000;
};

template<>
struct memory_map<reg::bus_ahb> {
  static constexpr std::uint32_t begin = memory_map<reg::reg_periph>::begin + 0x1'8000;
};

template<>
struct memory_map<reg::periph_gpioa> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x0800;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiob> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x0C00;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpioc> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x1000;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiod> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x1400;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpioe> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x1800;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiof> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x1C00;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiog> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x2000;
  static constexpr std::size_t size = 0x0400;
};

}  // namespace rtlib::core::stm32::f1

#endif  // defined(STM32F1)
