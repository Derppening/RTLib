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

#include <cstdint>
#include <type_traits>

namespace rtlib::core::stm32::f1 {

/**
 * @brief Loads data from the specified address.
 *
 * @tparam T Length of data to retrieve. Must be an integral type.
 * @param base_addr Address to load from.
 * @return Data from the specified address, represented as T.
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr volatile T const get_mem(const std::uint32_t base_addr) {
  return *reinterpret_cast<volatile T* const>(base_addr);
}

/**
 * @brief Stores data into the registered address.
 *
 * @tparam T Length of data to retrieve. Must be an integral type.
 * @param base_addr Address to store into.
 * @param val Data to store.
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr void set_mem(const std::uint32_t base_addr, const T val) {
  *reinterpret_cast<volatile T* const>(base_addr) = val;
}

enum struct reg {
  // Peripheral Registers
  REG_PERIPH,

  // Buses
  BUS_APB1,
  BUS_APB2,
  BUS_AHB,

  // APB1
  PERIPH_TIM2,
  PERIPH_TIM3,
  PERIPH_TIM4,
  PERIPH_TIM5,
  PERIPH_TIM6,
  PERIPH_TIM7,
  PERIPH_TIM12,
  PERIPH_TIM13,
  PERIPH_TIM14,
  PERIPH_RTC,
  PERIPH_WWDG,
  PERIPH_IWDG,
  PERIPH_SPI2,
  PERIPH_SPI3,
  PERIPH_USART2,
  PERIPH_USART3,
  PERIPH_UART4,
  PERIPH_UART5,
  PERIPH_I2C1,
  PERIPH_I2C2,
  PERIPH_USB_FS,
  PERIPH_SHR_USB_CAN_SRAM,
  PERIPH_BX_CAN1,
  PERIPH_BX_CAN2,
  PERIPH_BKP,
  PERIPH_PWR,
  PERIPH_DAC,

  // APB2
  PERIPH_AFIO,
  PERIPH_EXTI,
  PERIPH_GPIOA,
  PERIPH_GPIOB,
  PERIPH_GPIOC,
  PERIPH_GPIOD,
  PERIPH_GPIOE,
  PERIPH_GPIOF,
  PERIPH_GPIOG,
  PERIPH_ADC1,
  PERIPH_ADC2,
  PERIPH_TIM1,
  PERIPH_SPI1,
  PERIPH_TIM8,
  PERIPH_USART1,
  PERIPH_ADC3,
  PERIPH_TIM9,
  PERIPH_TIM10,
  PERIPH_TIM11,

  // AHB
  PERIPH_SDIO,
  PERIPH_DMA1,
  PERIPH_DMA2,
  PERIPH_RCC,
  PERIPH_FLASH,
  PERIPH_CRC,
  PERIPH_ETH,
  PERIPH_USB_OTG_FS,
  PERIPH_FSMC
};

template<reg R>
struct memory_map;

template<>
struct memory_map<reg::REG_PERIPH> {
  static constexpr std::uint32_t begin = 0x4000'0000;
};

template<>
struct memory_map<reg::BUS_APB1> {
  static constexpr std::uint32_t begin = memory_map<reg::REG_PERIPH>::begin + 0x0000;
};

template<>
struct memory_map<reg::BUS_APB2> {
  static constexpr std::uint32_t begin = memory_map<reg::REG_PERIPH>::begin + 0x1'0000;
};

template<>
struct memory_map<reg::BUS_AHB> {
  static constexpr std::uint32_t begin = memory_map<reg::REG_PERIPH>::begin + 0x1'8000;
};

template<>
struct memory_map<reg::PERIPH_GPIOA> {
  static constexpr std::uint32_t begin = memory_map<reg::BUS_APB2>::begin + 0x0800;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::PERIPH_GPIOB> {
  static constexpr std::uint32_t begin = memory_map<reg::BUS_APB2>::begin + 0x0C00;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::PERIPH_GPIOC> {
  static constexpr std::uint32_t begin = memory_map<reg::BUS_APB2>::begin + 0x1000;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::PERIPH_GPIOD> {
  static constexpr std::uint32_t begin = memory_map<reg::BUS_APB2>::begin + 0x1400;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::PERIPH_GPIOE> {
  static constexpr std::uint32_t begin = memory_map<reg::BUS_APB2>::begin + 0x1800;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::PERIPH_GPIOF> {
  static constexpr std::uint32_t begin = memory_map<reg::BUS_APB2>::begin + 0x1C00;
  static constexpr std::size_t size = 0x0400;
};

template<>
struct memory_map<reg::PERIPH_GPIOG> {
  static constexpr std::uint32_t begin = memory_map<reg::BUS_APB2>::begin + 0x2000;
  static constexpr std::size_t size = 0x0400;
};

}  // namespace rtlib::core::stm32::f1
