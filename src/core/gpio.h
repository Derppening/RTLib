#ifndef RTLIB_CORE_GPIO_H_
#define RTLIB_CORE_GPIO_H_

#include <cstdint>
#include <utility>

#include <libopencm3/stm32/gpio.h>

#include "util.h"

/**
 * @brief HAL implementation for GPIO pins.
 *
 * This abstraction layer provides simple Read/Write operations on GPIO pins. One GPIO object will correspond and manage
 * one GPIO pinout from the MCU.
 */
class GPIO {
 public:
  /**
   * @brief Type definition for GPIO Mode.
   */
  using Mode = uint8_t;
  /**
   * @brief Type definition for GPIO Output Speed.
   */
  using Speed = uint8_t;
  /**
   * @brief Type definition for GPIO Internal Pullup.
   */
  using Pullup = uint8_t;
  /**
   * @brief Type definition for GPIO Pin Driver Type.
   */
  using DriverType = uint8_t;

  /**
   * @brief Configuration for GPIO.
   */
  struct Config {
    /**
     * @brief GPIO Pin to enable and initialize.
     *
     * Defaults to @code {GPIOA, GPIO0} @endcode.
     */
    Pinout pin = {};

    /**
     * @brief GPIO Mode.
     *
     * | MCU Series |       Default Value      |
     * | :--------: | :---------------------:  |
     * | STM32F1xx  | @c GPIO_CNF_INPUT_ANALOG |
     * | STM32F4xx  | @c GPIO_MODE_INPUT       |
     */
    Mode mode = 0x0;

    /**
     * @brief GPIO Output Speed.
     *
     * | MCU Series |       Default Value      |
     * | :--------: | :----------------------: |
     * | STM32F1xx  | @c GPIO_MODE_INPUT       |
     * | STM32F4xx  | @c GPIO_MODE_OSPEED_2MHZ |
     */
    Speed speed = 0x0;

    /**
     * @brief Whether to use MCU's internal pull-up/down resistor.
     *
     * | MCU Series |   Default Value   |
     * | :--------: | :---------------: |
     * | STM32F1xx  | No Effect         |
     * | STM32F4xx  | @c GPIO_PUPD_NONE |
     */
    Pullup pullup = 0x0;

    /**
     * @brief GPIO Output Driver Type.
     *
     * | MCU Series |  Default Value   |
     * | :--------: | :--------------: |
     * | STM32F1xx  | No Effect        |
     * | STM32F4xx  | @c GPIO_OTYPE_PP |
     */
    DriverType driver = 0x0;
  };

  /**
   * @brief General constructor for GPIO.
   *
   * This constructor automatically delegates to the appropriate device-specific constructor.
   *
   * @param config Configuration for the GPIO. See GPIO#Config.
   */
  explicit GPIO(const Config& config);

#if defined(STM32F1)
  /**
   * @brief STM32F1xx-specific constructor for GPIO.
   *
   * This constructor is equivalent to @code GPIO::GPIO(const Config&) @endcode. However, it is suggested to use that
   * constructor instead.
   *
   * @param pin MCU pinout
   * @param mode GPIO Mode
   * @param speed GPIO Speed
   */
  GPIO(const Pinout& pin, Mode mode, Speed speed);
#endif  // defined(STM32F1)

#if defined(STM32F4)
  /**
   * @brief STM32F4xx-specific constructor for GPIO.
   *
   * This constructor is equivalent to @code GPIO::GPIO(const Config&) @endcode. However, it is suggested to use that
   * constructor instead.
   *
   * @param pin MCU pinout
   * @param mode GPIO mode
   * @param pullup Internal pull-up mode
   * @param speed GPIO Output Speed
   * @param driver GPIO Output Driver Type
   */
  GPIO(Pinout pin, Mode mode, Pullup pullup, Speed speed = 0x0, DriverType driver = 0x0);
#endif  // defined(STM32F4)

  /**
   * Default trivial destructor.
   */
  ~GPIO() = default;

  /**
   * @brief Move constructor for GPIO.
   *
   * @param other GPIO object to move from
   */
  GPIO(GPIO&& other) noexcept = default;
  /**
   * @brief Move assignment operator for GPIO.
   *
   * @param other GPIO object to move from
   * @return Reference to the moved GPIO.
   */
  GPIO& operator=(GPIO&& other) noexcept = default;

  /**
   * @brief Copy constructor for GPIO.
   *
   * This constructor is deleted because there should only be one object managing each GPIO pin, similar to @c
   * std::unique_ptr.
   */
  GPIO(const GPIO&) = delete;
  /**
   * @brief Copy assignment operator for GPIO.
   *
   * This constructor is deleted because there should only be one object managing each GPIO pin, similar to @c
   * std::unique_ptr.
   */
  GPIO& operator=(const GPIO&) = delete;

  /**
   * @brief Reads the current logic state of GPIO.
   *
   * @return @c true if high value, otherwise @c false
   */
  bool Read() const;
  /**
   * @brief Sets new GPIO state.
   *
   * @param state New state of GPIO, where @c true represents a high value, and @c false represents a low value.
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

 protected:
#if defined(STM32F1)
  /**
   * @brief Initializes this GPIO to the given configuration for STM32F1xx devices.
   *
   * See GPIO#Config for what @p mode and @p speed means.
   *
   * @param mode GPIO Mode
   * @param speed GPIO Speed
   */
  void Init(Mode mode, Speed speed) const;
#endif  // defined(STM32F1)

#if defined(STM32F4)
  /**
   * @brief Initializes this GPIO to the given configuration for STM32F4xx devices.
   *
   * @param mode GPIO Mode
   * @param pullup Internal GPIO pull-up mode
   * @param speed GPIO Output Speed
   * @param driver GPIO Output Driver Type
   */
  void Init(Mode mode, Pullup pullup, Speed speed = 0x0, DriverType driver = 0x0) const;
#endif  // defined(STM32F4)
  /**
   * @brief Initializes an RCC Clock according to which port is initialized.
   *
   * This function is a simple helper function to determine which RCC to initialize, given the port we are working with.
   *
   * @param port The GPIO port which should be initialized
   */
  void InitRcc(Port port) const;

  /**
   * @brief Retrieves the GPIO pinout currently managed by this object.
   *
   * @warning Do not attempt to modify this reference. Doing so may break all functions for this pin.
   *
   * @return The MCU pinout of the currently managed GPIO pin.
   */
  const Pinout& GetPinout() const { return pin_; }

 private:
  /**
   * @brief MCU Pinout of the currently managed GPIO.
   */
  Pinout pin_;
};

#endif  // RTLIB_CORE_GPIO_H_