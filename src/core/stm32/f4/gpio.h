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
 * @brief GPIO abstraction layer.
 */
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
    gpiog = memory_map<reg::periph_gpiog>::begin,
    gpioh = memory_map<reg::periph_gpioh>::begin,
    gpioi = memory_map<reg::periph_gpioi>::begin,
    gpioj = memory_map<reg::periph_gpioj>::begin,
    gpiok = memory_map<reg::periph_gpiok>::begin
  };

  /**
   * @brief GPIO Mode enumeration.
   *
   * Refer to STM32 RM0090 section 8.4.1 for more information.
   */
  enum struct modes : std::uint8_t {
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
  enum struct otypes : std::uint8_t {
    pushpull = 0x0,
    opendrain
  };

/**
 * @brief GPIO Output Speed enumeration.
 *
 * Refer to STM32 RM0090 section 8.4.3 for more information.
 */
  enum struct ospeeds : std::uint8_t {
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
  enum struct pupds : std::uint8_t {
    none = 0x0,
    pullup,
    pulldown
  };

  gpio(ports port, std::uint8_t pin);

  /**
   * @brief Configures the mode of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid mode into this function.
   *
   * @param mode New mode.
   */
  void mode(modes mode);

  /**
   * @return Current GPIO mode.
   */
  [[nodiscard]] modes mode() const;

  /**
   * @brief Configures the output type of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid type into this function.
   *
   * @param otype New output type.
   */
  void otype(otypes otype);

  /**
   * @return Current GPIO output type.
   */
  [[nodiscard]] otypes otype() const;

  /**
   * @brief Configures the output speed of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid speed into this function.
   *
   * @param ospeed New output speed.
   */
  void ospeed(ospeeds ospeed);

  /**
   * @return Current GPIO output speed.
   */
  [[nodiscard]] ospeeds ospeed() const;

  /**
   * @brief Configures whether to use the internal pull-up/pull-down resistor of this GPIO pinout.
   *
   * It is undefined behavior to pass an invalid pull-up/pull-down state into this function.
   *
   * @param pupd New pull-up/pull-down state.
   */
  void pupd(pupds pupd);

  /**
   * @return Current GPIO pull-up/pull-down.
   */
  [[nodiscard]] pupds pupd() const;

  /**
   * @brief Configures the alternate function to this GPIO pinout.
   *
   * It is undefined behavior to pass an out-of-range alternate function into this function.
   *
   * @param af New alternate function.
   */
  void af(std::uint8_t af);

  /**
   * @return Current GPIO alternate function.
   */
  [[nodiscard]] std::uint8_t af() const;

  /**
   * @brief Sets/Resets the output of this pinout.
   *
   * @param new_state Whether to set or reset this pinout.
   */
  void state(bool new_state) const;

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
  [[nodiscard]] ports port() const { return ports(reinterpret_cast<std::uint32_t>(_mmio)); }
  /**
   * @return Currently managed GPIO pin.
   */
  [[nodiscard]] std::uint8_t pin() const { return _pin; }

 private:
  struct mmio {
    std::uint32_t moder;
    std::uint32_t otyper;
    std::uint32_t ospeedr;
    std::uint32_t pupdr;
    std::uint32_t idr;
    std::uint32_t odr;
    std::uint32_t bsrr;
    std::uint32_t lckr;
    std::uint64_t afr;
  };

  volatile mmio* const _mmio;
  const std::uint8_t _pin;
};

}  // namespace rtlib::core::stm32::f4

#endif  // defined(STM32F4)
