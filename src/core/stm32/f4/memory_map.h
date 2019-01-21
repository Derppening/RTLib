/**
 * @file src/core/stm32/f4/memory_map.h
 *
 * @brief Memory map for STM32F4 devices.
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

#if defined(STM32F4)

#include <cstdint>
#include <type_traits>

#include "core/memory_map.h"

namespace rtlib::core::stm32::f4 {

enum struct reg {
  // Peripheral Registers
  reg_periph,

  // Buses
  bus_apb1,
  bus_apb2,
  bus_ahb1,
  bus_ahb2,
  bus_ahb3,


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
  periph_rtc_bkp,
  periph_wwdg,
  periph_iwdg,
  periph_spi2_i2s2,
  periph_spi3_i2s3,
  periph_i2s3ext,
  periph_usart2,
  periph_usart3,
  periph_uart4,
  periph_uart5,
  periph_i2c1,
  periph_i2c2,
  periph_i2c3,
  periph_can1,
  periph_can2,
  periph_bkp,
  periph_pwr,
  periph_dac,
  periph_uart7,
  periph_uart8,

  // APB2
  periph_tim1,
  periph_tim8,
  periph_usart1,
  periph_usart6,
  periph_adc1_2_3,
  periph_sdio,
  periph_spi1,
  periph_spi4,
  periph_syscfg,
  periph_exti,
  periph_tim9,
  periph_tim10,
  periph_tim11,
  periph_spi5,
  periph_spi6,
  periph_sai1,
  periph_lcd_tft,
  periph_afio,

  // AHB1
  periph_gpioa,
  periph_gpiob,
  periph_gpioc,
  periph_gpiod,
  periph_gpioe,
  periph_gpiof,
  periph_gpiog,
  periph_gpioh,
  periph_gpioi,
  periph_gpioj,
  periph_gpiok,
  periph_crc,
  periph_rcc,
  periph_flash,
  periph_bkpsram,
  periph_dma1,
  periph_dma2,
  periph_eth_mac,
  periph_dma2d,
  periph_usb_otg_hs,

  // AHB2
  periph_usb_otg_fs,
  periph_dcmi,
  periph_cryp,
  periph_hash,
  periph_rng,
  periph_fsmc,

  // AHB3
  periph_fsmc_fmc_cr
};

template<reg R>
struct memory_map;

template<>
struct memory_map<reg::reg_periph> {
  static constexpr std::uint32_t begin = 0x4000'0000;
};

template<>
struct memory_map<reg::bus_apb1> {
  static constexpr std::uint32_t begin = memory_map<reg::reg_periph>::begin + 0x0000'0000;
};

template<>
struct memory_map<reg::bus_apb2> {
  static constexpr std::uint32_t begin = memory_map<reg::reg_periph>::begin + 0x0001'0000;
};

template<>
struct memory_map<reg::bus_ahb1> {
  static constexpr std::uint32_t begin = memory_map<reg::reg_periph>::begin + 0x0002'0000;
};

template<>
struct memory_map<reg::bus_ahb2> {
  static constexpr std::uint32_t begin = memory_map<reg::reg_periph>::begin + 0x1000'0000;
};

template<>
struct memory_map<reg::bus_ahb3> {
  static constexpr std::uint32_t begin = memory_map<reg::reg_periph>::begin + 0x6000'0000;
};

template<>
struct memory_map<reg::periph_gpioa> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_ahb1>::begin + 0x0000;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiob> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_ahb1>::begin + 0x0400;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpioc> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_ahb1>::begin + 0x0800;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiod> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_ahb1>::begin + 0x0C00;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpioe> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_ahb1>::begin + 0x1000;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiof> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_ahb1>::begin + 0x1400;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_rcc> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_ahb1>::begin + 0x3800;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiog> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x1800;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpioh> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x1C00;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpioi> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x2000;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpioj> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x2400;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::periph_gpiok> {
  static constexpr std::uint32_t begin = memory_map<reg::bus_apb2>::begin + 0x2800;
  static constexpr std::size_t size = 0x0400;
};

}  // namespace rtlib::core::stm32::f4

#endif  // defined(STM32F4)
