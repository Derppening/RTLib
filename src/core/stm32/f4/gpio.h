/**
 * @file src/core/stm32/f4/gpio.h
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

#if defined(STM32F4)

#include <cstdint>

#include "core/assert.h"
#include "core/stm32/f4/memory_map.h"

namespace rtlib::core::stm32::f4 {

/**
 * @brief GPIO Ports enumeration.
 */
enum struct gpio_port : std::uint32_t {
  gpioa,
  gpiob,
  gpioc,
  gpiod,
  gpioe,
  gpiof,
  gpiog,
  gpioh,
  gpioi,
  gpioj,
  gpiok
};

/**
 * @brief GPIO Mode enumeration.
 *
 * Refer to STM32 RM0090 section 8.4.1 for more information.
 */
enum struct gpio_mode : std::uint8_t {
  input = 0x0,
  gpo,
  altfn,
  analog
};

/**
 * @brief GPIO Output Type enumeration.
 *
 * Refer to STM32 RM0090 section 8.4.2 for more information.
 */
enum struct gpio_otype : std::uint8_t {
  pushpull = 0x0,
  opendrain
};

/**
 * @brief GPIO Output Speed enumeration.
 *
 * Refer to STM32 RM0090 section 8.4.3 for more information.
 */
enum struct gpio_ospeed : std::uint8_t {
  low = 0x0,
  medium,
  high,
  veryhigh
};

/**
 * @brief GPIO Pull-up/Pull-down enumeration.
 *
 * Refer to STM32 RM0090 section 8.4.4 for more information.
 */
enum struct gpio_pullup_pulldown : std::uint8_t {
  none = 0x0,
  pullup,
  pulldown
};

/**
 * @brief GPIO abstraction layer.
 *
 * @tparam PORT GPIO Port.
 * @tparam PIN GPIO Pin. Must be between 0 and 15.
 */
template<gpio_port PORT, std::uint8_t PIN>
class gpio {
 public:
  static_assert(PIN >= 0x0 && PIN <= 0xF);

  gpio() = default;
  gpio(const gpio& other) = delete;
  gpio& operator=(const gpio& other) = delete;
  gpio(gpio&& other) noexcept = default;
  gpio& operator=(gpio&& other) noexcept = default;

  /**
   * @brief Configures the mode of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid mode into this function.
   *
   * @param mode New mode.
   */
  void mode(const gpio_mode mode) const {
    constexpr std::uint32_t bitmask = (1 << 2) - 1;
    const std::uint32_t addr = moder();

    auto v = get_mem(addr);
    v &= ~(bitmask << (PIN * 2));
    if constexpr (RTLIB_ENABLE_VALIDATION) {
      v |= (std::uint32_t(mode) & bitmask) << (PIN * 2);
    } else {
      v |= std::uint32_t(mode) << (PIN * 2);
    }
    set_mem(addr, v);
  }

  /**
   * @brief Configures the output type of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid type into this function.
   *
   * @param otype New output type.
   */
  void otype(const gpio_otype otype) const {
    constexpr std::uint32_t bitmask = (1 << 1) - 1;
    const std::uint32_t addr = otyper();

    auto v = get_mem(addr);
    v &= ~(bitmask << PIN);
    if constexpr (RTLIB_ENABLE_VALIDATION) {
      v |= (std::uint32_t(otype) & bitmask) << PIN;
    } else {
      v |= std::uint32_t(otype) << PIN;
    }
    set_mem(addr, v);
  }

  /**
   * @brief Configures the output speed of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid speed into this function.
   *
   * @param ospeed New output speed.
   */
  void ospeed(const gpio_ospeed ospeed) const {
    constexpr std::uint32_t bitmask = (1 << 2) - 1;
    const std::uint32_t addr = ospeedr();

    auto v = get_mem(addr);
    v &= ~(bitmask << (PIN * 2));
    if constexpr (RTLIB_ENABLE_VALIDATION) {
      v |= (std::uint32_t(ospeed) & bitmask) << (PIN * 2);
    } else {
      v |= std::uint32_t(ospeed) << (PIN * 2);
    }
    set_mem(addr, v);
  }

  /**
   * @brief Configures whether to use the internal pull-up/pull-down resistor of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid pull-up/pull-down state into this function.
   *
   * @param pupd New pull-up/pull-down state.
   */
  void pupd(const gpio_pullup_pulldown pupd) const {
    constexpr std::uint32_t bitmask = (1 << 2) - 1;
    const std::uint32_t addr = pupdr();

    auto v = get_mem(addr);
    v &= ~(bitmask << (PIN * 2));
    if constexpr (RTLIB_ENABLE_VALIDATION) {
      v |= (std::uint32_t(pupd) & bitmask) << (PIN * 2);
    } else {
      v |= std::uint32_t(pupd) << (PIN * 2);
    }
    set_mem(addr, v);
  }

  /**
   * @brief Configures the alternate function to this GPIO pinout.
   *
   * It is undefined behavior to pass an out-of-range alternate function into this function.
   *
   * @param af New alternate function.
   */
  void af(const std::uint8_t af) const {
    constexpr std::uint64_t bitmask = (1 << 4) - 1;
    const std::uint32_t addr = afr();

    auto v = get_mem<std::uint64_t>(addr);
    v &= ~(bitmask << (PIN * 4));
    if constexpr (RTLIB_ENABLE_VALIDATION) {
      v |= (std::uint64_t(af) & bitmask) << (PIN * 4);
    } else {
      v |= std::uint64_t(af) << (PIN * 4);
    }
    set_mem<std::uint64_t>(addr, v);
  }

  /**
   * @brief Sets/Resets the output of this pinout.
   *
   * @param new_state Whether to set or reset this pinout.
   */
  void state(const bool new_state) const {
    const std::uint32_t addr = bsrr();

    auto v = std::uint32_t(1 << (PIN + (16 * int(!new_state))));
    set_mem(addr, v);
  }

  /**
   * @brief Reads the current state of this pinout.
   *
   * @return Whether this pinout is currently digital high or low.
   */
  bool state() const {
    const std::uint32_t addr = _mode == gpio_mode::input ? idr() : odr();

    auto v = get_mem(addr);
    return bool(v & (1 << PIN));
  }

  /**
   * @brief Toggles the output of this pinout.
   *
   * @return The new state of this pinout.
   */
  bool toggle() {
    bool state = !this->state();
    this->state(state);

    return state;
  }

  /**
   * @return Currently managed GPIO port.
   */
  constexpr gpio_port port() const { return PORT; }
  /**
   * @return Currently managed GPIO pin.
   */
  constexpr std::uint8_t pin() const { return PIN; }

 private:
  gpio_mode _mode = gpio_mode::input;

  /**
   * @return The base memory boundary of this GPIO port.
   */
  constexpr std::uint32_t addr() const {
    if constexpr (PORT == gpio_port::gpioa) {
      return memory_map<reg::periph_gpioa>::begin;
    } else if (PORT == gpio_port::gpiob) {
      return memory_map<reg::periph_gpiob>::begin;
    } else if (PORT == gpio_port::gpioc) {
      return memory_map<reg::periph_gpioc>::begin;
    } else if (PORT == gpio_port::gpiod) {
      return memory_map<reg::periph_gpiod>::begin;
    } else if (PORT == gpio_port::gpioe) {
      return memory_map<reg::periph_gpioe>::begin;
    } else if (PORT == gpio_port::gpiof) {
      return memory_map<reg::periph_gpiof>::begin;
    } else if (PORT == gpio_port::gpiog) {
      return memory_map<reg::periph_gpiog>::begin;
    } else if (PORT == gpio_port::gpioh) {
      return memory_map<reg::periph_gpioh>::begin;
    } else if (PORT == gpio_port::gpioi) {
      return memory_map<reg::periph_gpioi>::begin;
    } else if (PORT == gpio_port::gpioj) {
      return memory_map<reg::periph_gpioj>::begin;
    } else if (PORT == gpio_port::gpiok) {
      return memory_map<reg::periph_gpiok>::begin;
    } else {
      return 0;
    }
  }

  /**
   * @return The address of the mode register (MODER) of this port.
   */
  constexpr std::uint32_t moder() const { return addr() + 0x00; }

  /**
   * @return The address of the output type register (OTYPER) of this port.
   */
  constexpr std::uint32_t otyper() const { return addr() + 0x04; }

  /**
   * @return The address of the output speed register (OSPEEDR) of this port.
   */
  constexpr std::uint32_t ospeedr() const { return addr() + 0x08; }

  /**
   * @return The address of the pull-up/pull-down register (PUPDR) of this port.
   */
  constexpr std::uint32_t pupdr() const { return addr() + 0x0C; }

  /**
   * @return The address of the input data register (IDR) of this port.
   */
  constexpr std::uint32_t idr() const { return addr() + 0x10; }

  /**
   * @return The address of the output data register (ODR) of this port.
   */
  constexpr std::uint32_t odr() const { return addr() + 0x14; }

  /**
   * @return The address of the bit set reset register (BSRR) of this GPIO port.
   */
  constexpr std::uint32_t bsrr() const { return addr() + 0x18; }

  /**
   * @return The address of the alternate function register (AFR) of this GPIO port.
   */
  constexpr std::uint32_t afr() const { return addr() + 0x20; }
};

}  // namespace rtlib::core::stm32::f4

#endif  // defined(STM32F4)
