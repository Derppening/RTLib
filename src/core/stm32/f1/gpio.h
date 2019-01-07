/**
 * @file src/core/stm32/f1/gpio.h
 *
 * @brief Header-only implementation for GPIO.
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

#include "core/assert.h"
#include "core/stm32/f1/memory_map.h"

namespace rtlib::core::stm32::f1 {

/**
 * @brief GPIO Ports enumeration.
 */
enum struct gpio_port_t : std::uint32_t {
  GPIOA,
  GPIOB,
  GPIOC,
  GPIOD,
  GPIOE,
  GPIOF,
  GPIOG
};

/**
 * @brief GPIO Configuration enumeration.
 *
 * Refer to STM32 RM0008 section 9.2.1 for more information.
 */
enum struct gpio_cnf_t : std::uint8_t {
  INPUT_ANALOG = 0x0,
  INPUT_FLOAT,
  INPUT_PULLUP_PULLDOWN,

  OUTPUT_PUSHPULL = 0x0,
  OUTPUT_OPENDRAIN,
  OUTPUT_ALTFN_PUSHPULL,
  OUTPUT_ALTFN_OPENDRAIN
};

/**
 * @brief GPIO Mode enumeration.
 *
 * Refer to STM32 RM0008 section 9.2.1 for more information.
 */
enum struct gpio_mode_t : std::uint8_t {
  INPUT,
  OUTPUT_2MHZ,
  OUTPUT_10MHZ,
  OUTPUT_50MHZ
};

/**
 * @brief GPIO abstraction layer.
 *
 * @tparam PORT GPIO Port.
 * @tparam PIN GPIO Pin. Must be between 0 and 15.
 */
template<gpio_port_t PORT, std::uint8_t PIN>
class gpio {
 public:
  static_assert(PIN >= 0x0 && PIN <= 0xF);

  gpio() = default;
  gpio(const gpio& other) = delete;
  gpio& operator=(const gpio& other) = delete;
  gpio(gpio&& other) noexcept = default;
  gpio& operator=(gpio&& other) noexcept = default;

  /**
   * @brief Configures the CNF bits of this GPIO pinout.
   *
   * @param configuration New configuration.
   */
  void cnf(gpio_cnf_t configuration) const {
    std::uint32_t addr = cr();
    if (addr == 0) {
      return;
    }

    auto v = get_mem<std::uint64_t>(addr);
    v &= ~(std::uint64_t(0b1100) << (PIN * 4));
    v |= std::uint64_t(std::uint64_t(configuration) << (PIN * 4 + 2));
    set_mem<std::uint64_t>(addr, v);
  }

  /**
   * @brief Configures the MODE bits of this GPIO pinout.
   *
   * @param mode New mode.
   */
  void mode(gpio_mode_t mode) const {
    std::uint32_t addr = cr();
    if (addr == 0) {
      return;
    }

    auto v = get_mem<std::uint64_t>(addr);
    v &= ~(std::uint64_t(0b0011) << (PIN * 4));
    v |= std::uint64_t(mode) << (PIN * 4);
    set_mem<std::uint64_t>(addr, v);

    _mode = mode;
  }

  /**
   * @brief Sets/Resets the output of this pinout.
   *
   * @param new_state Whether to set or reset this pinout.
   */
  void state(const bool new_state) {
    std::uint32_t addr = bsrr();
    if (addr == 0) {
      return;
    }

    auto v = std::uint32_t(1 << (PIN + (16 * int(!new_state))));
    set_mem(addr, v);
  }

  /**
   * @brief Reads the current state of this pinout.
   *
   * @return Whether this pinout is currently digital high or low.
   */
  bool state() {
    std::uint32_t addr = _mode == gpio_mode_t::INPUT ? idr() : odr();

    auto v = get_mem(addr);
    return bool(v & (1 << PIN));
  }

  /**
   * @return Currently managed GPIO port.
   */
  constexpr gpio_port_t port() const { return PORT; }
  /**
   * @return Currently managed GPIO pin.
   */
  constexpr std::uint8_t pin() const { return PIN; }

 private:
  gpio_mode_t _mode = gpio_mode_t::INPUT;

  /**
   * @return The base memory boundary of this GPIO port.
   */
  constexpr std::uint32_t addr() const {
    if constexpr (PORT == gpio_port_t::GPIOA) {
      return memory_map<reg::PERIPH_GPIOA>::begin;
    } else if (PORT == gpio_port_t::GPIOB) {
      return memory_map<reg::PERIPH_GPIOB>::begin;
    } else if (PORT == gpio_port_t::GPIOC) {
      return memory_map<reg::PERIPH_GPIOC>::begin;
    } else if (PORT == gpio_port_t::GPIOD) {
      return memory_map<reg::PERIPH_GPIOD>::begin;
    } else if (PORT == gpio_port_t::GPIOE) {
      return memory_map<reg::PERIPH_GPIOE>::begin;
    } else if (PORT == gpio_port_t::GPIOF) {
      return memory_map<reg::PERIPH_GPIOF>::begin;
    } else if (PORT == gpio_port_t::GPIOG) {
      return memory_map<reg::PERIPH_GPIOG>::begin;
    } else {
      return 0;
    }
  }

  /**
   * @return The address of the control register (CR) of this GPIO port.
   */
  constexpr std::uint32_t cr() const {
    constexpr std::uint32_t offset = 0x00;
    return addr() + offset;
  }

  /**
   * @return The address of the input data register (IDR) of this port.
   */
  constexpr std::uint32_t idr() const {
    constexpr std::uint32_t offset = 0x08;
    return addr() + offset;
  }

  /**
   * @return The address of the output data register (ODR) of this port.
   */
  constexpr std::uint32_t odr() const {
    constexpr std::uint32_t offset = 0x0C;
    return addr() + offset;
  }

  /**
   * @return The address of the bit set reset register (BSRR) of this GPIO port.
   */
  constexpr std::uint32_t bsrr() const {
    constexpr std::uint32_t offset = 0x10;
    return addr() + offset;
  }
};

}  // namespace rtlib::core::stm32::f1
