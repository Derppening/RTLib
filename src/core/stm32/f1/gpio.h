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

#if defined(STM32F1)

#include <cstdint>

#include "core/assert.h"
#include "core/stm32/f1/memory_map.h"

namespace rtlib::core::stm32::f1 {

/**
 * @brief GPIO Ports enumeration.
 */
enum struct gpio_port : std::uint32_t {
  gpioa = memory_map<reg::periph_gpioa>::begin,
  gpiob = memory_map<reg::periph_gpiob>::begin,
  gpioc = memory_map<reg::periph_gpioc>::begin,
  gpiod = memory_map<reg::periph_gpiod>::begin,
  gpioe = memory_map<reg::periph_gpioe>::begin,
  gpiof = memory_map<reg::periph_gpiof>::begin,
  gpiog = memory_map<reg::periph_gpiog>::begin
};

/**
 * @brief GPIO Configuration enumeration.
 *
 * Refer to STM32 RM0008 section 9.2.1 and 9.2.2 for more information.
 */
enum struct gpio_cnf : std::uint8_t {
  input_analog = 0x0,
  input_float,
  input_pullup_pulldown,

  output_pushpull = 0x0,
  output_opendrain,
  output_altfn_pushpull,
  output_altfn_opendrain
};

/**
 * @brief GPIO Mode enumeration.
 *
 * Refer to STM32 RM0008 section 9.2.1 and 9.2.2 for more information.
 */
enum struct gpio_mode : std::uint8_t {
  input,
  output_2mhz,
  output_10mhz,
  output_50mhz
};

/**
 * @brief GPIO abstraction layer.
 */
class gpio {
 public:
  constexpr gpio(const gpio_port port, const std::uint8_t pin)
      : _addr(std::uint32_t(port)), _pin(pin) {}
  gpio(const gpio& other) = delete;
  gpio& operator=(const gpio& other) = delete;
  gpio(gpio&& other) noexcept = default;
  gpio& operator=(gpio&& other) noexcept = default;

  //<editor-fold desc="Configuration">

  /**
   * @brief Configures the CNF bits of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid configuration into this function.
   *
   * @param configuration New configuration.
   */
  constexpr void cnf(gpio_cnf configuration) {
    const std::uint32_t addr = cr();

    auto v = get_mem<std::uint64_t>(addr);
    v = reset_and_set(v, _pin * 4 + 2, 2, std::uint64_t(configuration));
    set_mem<std::uint64_t>(addr, v);
  }

  /**
   * @return Current GPIO configuration.
   */
  constexpr gpio_cnf cnf() const {
    const std::uint32_t addr = cr();

    auto v = get_mem<std::uint64_t>(addr);
    return gpio_cnf(get_bitmask(v, _pin * 4 + 2, 2));
  }

  /**
   * @brief Configures the MODE bits of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid mode into this function.
   *
   * @param mode New mode.
   */
  constexpr void mode(gpio_mode mode) {
    const std::uint32_t addr = cr();

    auto v = get_mem<std::uint64_t>(addr);
    v = reset_and_set(v, _pin * 4, 2, std::uint64_t(mode));
    set_mem<std::uint64_t>(addr, v);
  }

  /**
   * @return Current GPIO mode.
   */
  constexpr gpio_mode mode() const {
    const std::uint32_t addr = cr();

    auto v = get_mem<std::uint64_t>(addr);
    return gpio_mode(get_bitmask(v, _pin * 4, 2));
  }

  //</editor-fold>

  //<editor-fold desc="I/O">

  /**
   * @brief Sets/Resets the output of this pinout.
   *
   * @param new_state Whether to set or reset this pinout.
   */
  constexpr void state(bool new_state) {
    const std::uint32_t addr = bsrr();

    auto v = std::uint32_t(1) << (_pin + (16 * std::uint8_t(!new_state)));
    set_mem(addr, v);
  }

  /**
   * @brief Reads the current state of this pinout.
   *
   * @return Whether this pinout is currently digital high or low.
   */
  constexpr bool state() const {
    const std::uint32_t addr = mode() == gpio_mode::input ? idr() : odr();

    auto v = get_mem(addr);
    return bool(get_bitmask(v, _pin, 1));
  }

  /**
   * @brief Toggles the output of this pinout.
   *
   * @return The new state of this pinout.
   */
  constexpr bool toggle() {
    bool state = !this->state();
    this->state(state);

    return state;
  }

  //</editor-fold>

  /**
 * @return Currently managed GPIO port.
 */
  constexpr gpio_port port() const { return gpio_port(_addr); }
  /**
   * @return Currently managed GPIO pin.
   */
  constexpr std::uint8_t pin() const { return _pin; }

 private:
  std::uint32_t _addr;
  std::uint8_t _pin;

  /**
 * @return The address of the control register (CR) of this GPIO port.
 */
  constexpr std::uint32_t cr() const { return _addr + 0x00; }

  /**
   * @return The address of the input data register (IDR) of this port.
   */
  constexpr std::uint32_t idr() const { return _addr + 0x08; }

  /**
   * @return The address of the output data register (ODR) of this port.
   */
  constexpr std::uint32_t odr() const { return _addr + 0x0C; }

  /**
   * @return The address of the bit set reset register (BSRR) of this GPIO port.
   */
  constexpr std::uint32_t bsrr() const { return _addr + 0x10; }
};

namespace experimental {
/**
 * @brief Experimental GPIO abstraction layer.
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
   * @brief Configures the CNF bits of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid configuration into this function.
   *
   * @param configuration New configuration.
   */
  void cnf(gpio_cnf configuration) {
    const std::uint32_t addr = cr();

    auto v = get_mem<std::uint64_t>(addr);
    v = reset_and_set(v, PIN * 4 + 2, 2, std::uint64_t(configuration));
    set_mem<std::uint64_t>(addr, v);
  }

  /**
   * @brief Configures the MODE bits of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid mode into this function.
   *
   * @param mode New mode.
   */
  void mode(gpio_mode mode) {
    const std::uint32_t addr = cr();

    auto v = get_mem<std::uint64_t>(addr);
    v = reset_and_set(v, PIN * 4, 2, std::uint64_t(mode));
    set_mem<std::uint64_t>(addr, v);

    _mode = mode;
  }

  /**
   * @brief Sets/Resets the output of this pinout.
   *
   * @param new_state Whether to set or reset this pinout.
   */
  void state(const bool new_state) {
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
    return bool(get_bitmask(v, PIN, 1));
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
    } else {
      return 0;
    }
  }

  /**
   * @return The address of the control register (CR) of this GPIO port.
   */
  constexpr std::uint32_t cr() const { return addr() + 0x00; }

  /**
   * @return The address of the input data register (IDR) of this port.
   */
  constexpr std::uint32_t idr() const { return addr() + 0x08; }

  /**
   * @return The address of the output data register (ODR) of this port.
   */
  constexpr std::uint32_t odr() const { return addr() + 0x0C; }

  /**
   * @return The address of the bit set reset register (BSRR) of this GPIO port.
   */
  constexpr std::uint32_t bsrr() const { return addr() + 0x10; }
};
}  // namespace experimental

}  // namespace rtlib::core::stm32::f1

#endif  // defined(STM32F1)
