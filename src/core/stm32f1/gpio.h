#ifndef RTLIB_CORE_STM32F1_GPIO_H_
#define RTLIB_CORE_STM32F1_GPIO_H_

#if defined(STM32F1)

#include <cstdint>
#include <utility>

#include <libopencm3/stm32/gpio.h>

#include "core/util.h"

namespace core {
namespace stm32f1 {

/**
 * @brief STM32F1xx-specific HAL implementation for GPIO pins.
 *
 * This abstraction layer provides simple Read/Write operations on GPIO pins. One GPIO object will correspond and manage
 * one GPIO pinout from the MCU.
 */
class GPIO final {
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
     * Defaults to @c GPIO_CNF_INPUT_ANALOG.
     */
    Mode mode = GPIO_CNF_INPUT_ANALOG;

    /**
     * @brief GPIO Output Speed.
     *
     * Defaults to @c GPIO_MODE_INPUT.
     */
    Speed speed = GPIO_MODE_INPUT;
  };

  /**
   * @brief Conversion constructor.
   *
   * @param config Configuration for the GPIO. See GPIO#Config.
   */
  explicit GPIO(const Config& config);

  /**
   * @brief Constructor for internal API use.
   *
   * This constructor is equivalent to @code GPIO::GPIO(const Config&) @endcode. However, it is suggested to use that
   * constructor instead for code readability.
   *
   * @param pin MCU pinout
   * @param mode GPIO Mode
   * @param speed GPIO Speed
   */
  GPIO(const Pinout& pin, Mode mode, Speed speed);

  /**
   * Default trivial destructor.
   */
  ~GPIO() = default;

  /**
   * @brief Move constructor.
   *
   * @param other GPIO object to move from
   */
  GPIO(GPIO&& other) noexcept = default;
  /**
   * @brief Move assignment operator.
   *
   * @param other GPIO object to move from
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

 private:
  /**
 * @brief Initializes this GPIO to the given configuration for STM32F1xx devices.
 *
 * See GPIO#Config for what @p mode and @p speed means.
 *
 * @param mode GPIO Mode
 * @param speed GPIO Speed
 */
  void Init(Mode mode, Speed speed) const;

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
  /**
   * @brief MCU Pinout of the currently managed GPIO.
   */
  Pinout pin_;
};

}  // namespace stm32f1
}  // namespace core

#endif  // defined(STM32F1)

#endif  // RTLIB_CORE_STM32F1_GPIO_H_
