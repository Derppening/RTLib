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

class gpio {
 public:
  /**
   * @brief GPIO Ports enumeration.
   */
  enum struct ports : std::uint32_t {
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
  enum struct cnfs : std::uint8_t {
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
  enum struct modes : std::uint8_t {
    input,
    output_2mhz,
    output_10mhz,
    output_50mhz
  };

  gpio(ports port, std::uint8_t pin);
  gpio(const gpio& other) = default;
  gpio& operator=(const gpio& other) = delete;
  gpio(gpio&& other) noexcept = default;
  gpio& operator=(gpio&& other) noexcept = delete;

  /**
   * @brief Configures the CNF bits of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid configuration into this function.
   *
   * @param configuration New configuration.
   */
  void cnf(gpio::cnfs configuration);

  /**
   * @return Current GPIO configuration.
   */
  [[nodiscard]] gpio::cnfs cnf() const;

  /**
   * @brief Configures the MODE bits of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid mode into this function.
   *
   * @param mode New mode.
   */
  void mode(gpio::modes mode);

  /**
   * @return Current GPIO mode.
   */
  [[nodiscard]] gpio::modes mode() const;

  /**
   * @brief Sets/Resets the output of this pinout.
   *
   * @param new_state Whether to set or reset this pinout.
   */
  void state(bool new_state);

  /**
   * @brief Reads the current state of this pinout.
   *
   * @return Whether this pinout is currently digital high or low.
   */
  [[nodiscard]] bool state() const;

  /**
   * @brief Toggles the output of this pinout.
   *
   * @return The new state of this pinout.
   */
  bool toggle();

  /**
   * @return Currently managed GPIO port.
   */
  [[nodiscard]] gpio::ports port() const { return gpio::ports(reinterpret_cast<std::uint32_t>(_mmio)); }
  /**
   * @return Currently managed GPIO pin.
   */
  [[nodiscard]] std::uint8_t pin() const { return _pin; }

 private:
  struct mmio {
    std::uint64_t cr;
    std::uint32_t idr;
    std::uint32_t odr;
    std::uint32_t bsrr;
    std::uint32_t brr;
    std::uint32_t lckr;
  };

 volatile mmio* const _mmio;
 const std::uint8_t _pin;
};

}  // namespace rtlib::core::stm32::f1

#endif  // defined(STM32F1)
