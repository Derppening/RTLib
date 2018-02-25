/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2018 Derppening <david.18.19.21@gmail.com>
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

#ifndef RTLIB_CORE_STM32F4_GPIO_H_
#define RTLIB_CORE_STM32F4_GPIO_H_

#if defined(STM32F4)

#include <cstdint>
#include <utility>

#include <libopencm3/stm32/gpio.h>

#include "core/util.h"

namespace rtlib::core::stm32f4 {

/**
 * @brief STM32F4xx-specific HAL implementation for GPIO pins.
 *
 * This abstraction layer provides simple Read/Write operations on GPIO pins. One GPIO object will correspond and manage
 * one GPIO pinout from the MCU.
 */
class GPIO final {
 public:
  /**
   * @brief Type definition for GPIO alternate function selection.
   *
   * See http://libopencm3.org/docs/latest/stm32f4/html/group__gpio__af__num.html.
   */
  using AltFn = uint8_t;

  /**
   * @brief Enumeration for different GPIO modes.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f4/html/group__gpio__mode.html.
   */
  enum struct Mode : uint8_t {
    /**
     * @brief Use this GPIO as digital input.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_INPUT.
     */
    kInput = 0x0,
    /**
     * @brief Use this GPIO as digital output.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_OUTPUT.
     */
    kOutput,
    /**
     * @brief Use this GPIO as an alternate function.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_AF.
     */
    kAF,
    /**
     * @brief Use this GPIO as analog I/O.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_ANALOG.
     */
    kAnalog
  };

  /**
   * @brief Enumeration for different GPIO output speeds.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f4/html/group__gpio__speed.html.
   */
  enum struct Speed : uint8_t {
    /**
     * @brief Output GPIO at 2MHz.
     *
     * Equivalent to libopencm3 macro @c GPIO_OSPEED_2MHZ.
     */
    k2MHz = 0x0,
    /**
     * @brief Output GPIO at 25MHz.
     *
     * Equivalent to libopencm3 macro @c GPIO_OSPEED_25MHZ.
     */
    k25MHz,
    /**
     * @brief Output GPIO at 50MHz.
     *
     * Equivalent to libopencm3 macro @c GPIO_OSPEED_50MHZ.
     */
    k50MHz,
    /**
     * @brief Output GPIO at 100MHz.
     *
     * Equivalent to libopencm3 macro @c GPIO_OSPEED_100MHZ.
     */
    k100MHz
  };

  /**
   * @brief Enumeration for different internal pull-up states.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f4/html/group__gpio__pup.html.
   */
  enum struct Pullup : uint8_t {
    /**
     * @brief Do not use the internal pull-up/pull-down resistor (i.e. floating mode).
     *
     * Equivalent to libopencm3 macro @c GPIO_PUPD_NONE.
     */
    kNone = 0x0,
    /**
     * @brief Use the internal pull-up resistor.
     *
     * Equivalent to libopencm3 macro @c GPIO_PUPD_PULLUP.
     */
    kPullup,
    /**
     * @brief Use the internal pull-down resistor.
     *
     * Equivalent to libopencm3 macro @c GPIO_PUPD_PULLDOWN.
     */
    kPulldown
  };

  /**
   * @brief Enumeration for GPIO output driver modes.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f4/html/group__gpio__output__type.html.
   */
  enum struct DriverType : uint8_t {
    /**
     * @brief Drive GPIO output using push-pull (i.e. actively pushed to VCC/pulled to GND).
     */
    kPushPull = 0x0,
    /**
     * @brief Drive GPIO output using open-drain (i.e. only pull to GND).
     */
    kOpenDrain
  };

  /**
   * @brief Configuration for GPIO.
   */
  struct Config {
    /**
     * @brief GPIO Pin to enable and initialize.
     *
     * Defaults to an indeterminate state.
     */
    Pinout pin = {};

    /**
     * @brief GPIO Mode.
     *
     * Defaults to Mode#kInput.
     */
    Mode mode = Mode::kInput;

    /**
     * @brief GPIO Output Speed.
     *
     * Defaults to Speed#k2MHz.
     */
    Speed speed = Speed::k2MHz;

    /**
     * @brief Whether to use MCU's internal pull-up/down resistor.
     *
     * Defaults to Pullup#kNone.
     */
    Pullup pullup = Pullup::kNone;

    /**
     * @brief GPIO Output Driver Type.
     *
     * Defaults to DriverType#kPushPull.
     */
    DriverType driver = DriverType::kPushPull;

    /**
     * @brief GPIO Alternate Function Selection.
     *
     * Defaults to GPIO_AF0.
     */
    AltFn altfn = GPIO_AF0;
  };

  /**
   * @brief Conversion constructor.
   *
   * @param[in] config Configuration for the GPIO. See GPIO#Config.
   */
  explicit GPIO(const Config& config);

  /**
   * @brief Constructor for internal API use.
   *
   * This constructor is equivalent to @code GPIO::GPIO(const Config&) @endcode. However, it is suggested to use that
   * constructor instead for code readability.
   *
   * @param[in] pin MCU pinout
   * @param[in] mode GPIO mode
   * @param[in] pullup Internal pull-up mode
   * @param[in] speed GPIO Output Speed
   * @param[in] driver GPIO Output Driver Type
   * @param[in] altfn GPIO Alternate Function Select
   */
  GPIO(Pinout pin,
       Mode mode,
       Pullup pullup,
       Speed speed = Speed::k2MHz,
       DriverType driver = DriverType::kPushPull,
       AltFn altfn = 0x0);

  /**
   * Default trivial destructor.
   */
  ~GPIO() = default;

  /**
   * @brief Move constructor.
   *
   * @param[in] other GPIO object to move from
   */
  GPIO(GPIO&& other) noexcept = default;
  /**
   * @brief Move assignment operator.
   *
   * @param[in] other GPIO object to move from
   *
   * @return Reference to the moved GPIO.
   */
  GPIO& operator=(GPIO&& other) noexcept = default;

  /**
   * @brief Copy constructor.
   *
   * This constructor is deleted because there should only be one object managing each GPIO pin, similar to @c
   * std::unique_ptr.
   */
  GPIO(const GPIO&) = delete;
  /**
   * @brief Copy assignment operator.
   *
   * This constructor is deleted because there should only be one object managing each GPIO pin, similar to @c
   * std::unique_ptr.
   */
  GPIO& operator=(const GPIO&) = delete;

  /**
   * @brief Reads the current logic state of the managed GPIO.
   *
   * @return @c true if high value, otherwise @c false
   */
  bool Read() const;
  /**
   * @brief Sets new GPIO state.
   *
   * @param[in] state New state of GPIO, where @c true represents a high value, and @c false represents a low value.
   */
  void Set(bool state) const;
  /**
   * @brief Toggles GPIO state. Logic High -> Logic Low and vice versa.
   */
  void Toggle() const;

  /**
   * @brief Resets the GPIO to its original configuration, i.e. when the RESET button is first pushed.
   */
  void Reset() const;

 private:
  /**
   * @brief Initializes this GPIO to the given configuration for STM32F4xx devices.
   *
   * @param[in] mode GPIO Mode
   * @param[in] pullup Internal GPIO pull-up mode
   * @param[in] speed GPIO Output Speed
   * @param[in] driver GPIO Output Driver Type
   */
  void Init(Mode mode, Pullup pullup, Speed speed = Speed::k2MHz, DriverType driver = DriverType::kPushPull) const;
  /**
   * @brief Initializes an RCC Clock according to which port is initialized.
   *
   * This function is a simple helper function to determine which RCC to initialize, given the port we are working with.
   *
   * @param[in] port The GPIO port which should be initialized
   */
  constexpr void InitRcc(Port port) const;

  /**
   * @brief Sets this GPIO to be used as an alternate function.
   *
   * Most pins have alternate functions associated with them. This function allows those functions to be used.
   *
   * @param[in] altfn Alternate function number
   */
  void SetAltFn(uint8_t altfn) const;

  /**
   * @brief Retrieves the GPIO pinout currently managed by this object.
   *
   * @warning Do not attempt to modify this reference. Doing so may break all functions for this GPIO object.
   *
   * @return The MCU pinout of the currently managed GPIO pin.
   */
  const Pinout& GetPinout() const { return pin_; }

  /**
   * @brief MCU Pinout of the currently managed GPIO.
   */
  Pinout pin_;
};

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)

#endif  // RTLIB_CORE_STM32F4_GPIO_H_
