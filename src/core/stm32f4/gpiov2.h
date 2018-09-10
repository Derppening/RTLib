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

#ifndef RTLIB_CORE_STM32F4_GPIOV2_H_
#define RTLIB_CORE_STM32F4_GPIOV2_H_

#include "config/config.h"

#if defined(USE_GPIO_V2)
#if defined(STM32F4)

#include <cstdint>
#include <utility>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "core/util.h"

namespace rtlib::core::stm32f4 {

/**
 * @brief STM32F4xx-specific HAL implementation for GPIO pins.
 *
 * This abstraction layer provides simple Read/Write operations on GPIO pins. One GPIO object will correspond and manage
 * one GPIO pinout from the MCU.
 */
class GPIOv2 final {
 public:
  enum struct Port : std::uint8_t {
    kGPIOA = 0,
    kGPIOB,
    kGPIOC,
    kGPIOD,
    kGPIOE,
    kGPIOF,
    kGPIOG,
    kGPIOI,
    kGPIOJ,
    kGPIOK
  };

  enum struct Pin : std::uint8_t {
    kGPIO0 = 0,
    kGPIO1,
    kGPIO2,
    kGPIO3,
    kGPIO4,
    kGPIO5,
    kGPIO6,
    kGPIO7,
    kGPIO8,
    kGPIO9,
    kGPIO10,
    kGPIO11,
    kGPIO12,
    kGPIO13,
    kGPIO14,
    kGPIO15
  };

  /**
   * @brief Type definition of a MCU GPIO pinout.
   */
  using Pinout = std::pair<Port, Pin>;
  
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
    std::pair<Port, Pin> pin = kUnbindedPinout;

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
   * @brief Default constructor.
   *
   * Initializes this object to an "unbinded" state, i.e. this object does not manage any GPIO.
   */
  constexpr GPIOv2() : pin_(kUnbindedPinout) {}

  /**
   * @brief Conversion constructor.
   *
   * @param[in] config Configuration for the GPIO. See GPIO#Config.
   */
  constexpr explicit GPIOv2(const Config& config) :
      pin_(config.pin) {
    // Use external oscillator for RCC
    rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    // Initialize the RCC and enable the GPIO
    InitRcc();
    Init(config.mode, config.pullup, config.speed, config.driver);

    // Set the alternate function
    SetAltFn(config.altfn);
  }

  /**
   * Default destructor.
   */
  ~GPIOv2();

  /**
   * @brief Move constructor.
   *
   * @param[in] other GPIO object to move from
   */
  constexpr GPIOv2(GPIOv2&& other) noexcept :
      pin_(std::move(other.pin_)) {
    other.pin_ = kUnbindedPinout;
  }
  /**
   * @brief Move assignment operator.
   *
   * @param[in] other GPIO object to move from
   *
   * @return Reference to the moved GPIO.
   */
  constexpr GPIOv2& operator=(GPIOv2&& other) noexcept {
    if (this != &other) {
      pin_ = std::move(other.pin_);
      other.pin_ = kUnbindedPinout;
    }

    return *this;
  }

  /**
   * @brief Copy constructor.
   *
   * This constructor is deleted because there should only be one object managing each GPIO pin, similar to @c
   * std::unique_ptr.
   */
  GPIOv2(const GPIOv2&) = delete;
  /**
   * @brief Copy assignment operator.
   *
   * This constructor is deleted because there should only be one object managing each GPIO pin, similar to @c
   * std::unique_ptr.
   */
  GPIOv2& operator=(const GPIOv2&) = delete;

  /**
 * @return Whether this object is managing a GPIO pinout.
 */
  constexpr bool IsBinded() const { return pin_ != kUnbindedPinout; }

  /**
   * @brief Reads the current logic state of the managed GPIO.
   *
   * @return @c true if high value, otherwise @c false
   *
   * If validation is enabled, this function will assert if invoked on a non-owning object.
   */
  bool Read() const;
  /**
   * @brief Sets new GPIO state.
   *
   * @param[in] state New state of GPIO, where @c true represents a high value, and @c false represents a low value.
   *
   * If validation is enabled, this function will assert if invoked on a non-owning object.
   */
  void Set(bool state) const;
  /**
   * @brief Toggles GPIO state. Logic High -> Logic Low and vice versa.
   *
   * If validation is enabled, this function will assert if invoked on a non-owning object.
   */
  void Toggle() const;

  /**
   * @brief Releases the ownership of the GPIO from this object.
   *
   * This function implicitly invokes GPIO#Reset, and releases the ownership of the currently managed GPIO.
   *
   * This function is null-safe, i.e. if this object is not bound to any pinout, there are no effects.
   */
  void Release();

 private:
  static constexpr const std::pair<Port, Pin> kUnbindedPinout{Port(0xFF), Pin(0xFF)};

  /**
   * @brief Initializes this GPIO to the given configuration for STM32F4xx devices.
   *
   * @param[in] mode GPIO Mode
   * @param[in] pullup Internal GPIO pull-up mode
   * @param[in] speed GPIO Output Speed
   * @param[in] driver GPIO Output Driver Type
   */
  void Init(Mode mode, Pullup pullup, Speed speed, DriverType driver) const;
  /**
   * @brief Initializes an RCC Clock according to which port is initialized.
   *
   * This function is a simple helper function to determine which RCC to initialize, given the port we are working with.
   *
   * @param[in] port The GPIO port which should be initialized
   */
  void InitRcc() const;

  /**
   * @brief Resets the GPIO to its original configuration, i.e. when the RESET button is first pushed.
   *
   * This function is null-safe, i.e. if this object is not bound to any pinout, there are no effects.
   */
  void Reset() const;

  /**
   * @brief Sets this GPIO to be used as an alternate function.
   *
   * Most pins have alternate functions associated with them. This function allows those functions to be used.
   *
   * @param[in] altfn Alternate function number
   */
  void SetAltFn(uint8_t altfn) const;
  
  constexpr std::uint32_t GetPortMemMapping() const { return PERIPH_BASE_AHB1 + std::uint32_t(0x0400 * std::uint8_t(pin_.first)); }
  constexpr std::uint16_t GetPinMemMapping() const { return std::uint16_t(1 << std::uint8_t(pin_.second)); }

  /**
   * @brief MCU Pinout of the currently managed GPIO.
   */
  std::pair<Port, Pin> pin_;
};

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)
#endif  // defined(USE_GPIO_V2)

#endif  // RTLIB_CORE_STM32F4_GPIOV2_H_
