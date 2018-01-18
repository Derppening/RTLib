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
   * @brief Enumeration for different GPIO configurations.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__cnf.html.
   */
  enum struct Configuration : uint8_t {
    /**
     * @brief Use this GPIO as analog input.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_INPUT_ANALOG.
     */
    kInputAnalog = 0x0,
    /**
     * @brief Use this GPIO as floating input.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_INPUT_FLOAT.
     */
    kInputFloat,
    /**
     * @brief Use this GPIO as pull-up/pull-down input.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_INPUT_PULL_UPDOWN.
     */
    kInputPullUpDown,
    /**
     * @brief Use this GPIO as digital push-pull output.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_OUTPUT_PUSHPULL
     */
    kOutputPushPull = 0x0,
    /**
     * @brief Use this GPIO as digital open-drain output.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_OUTPUT_OPENDRAIN.
     */
    kOutputOpenDrain,
    /**
     * @brief Use this GPIO as alternate function push-pull output.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_ALTFN_PUSHPULL.
     */
    kOutputAltFnPushPull,
    /**
     * @brief Use this GPIO as alternate function open-drain output.
     *
     * Equivalent to libopencm3 macro @c GPIO_CNF_ALTFN_OPENDRAIN.
     */
    kOutputAltFnOpenDrain
  };

  /**
   * @brief Enumeration for different GPIO modes.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__mode.html.
   */
  enum struct Mode : uint8_t {
    /**
     * @brief Input Mode.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_INPUT.
     */
    kInput,
    /**
     * @brief Output Mode at 10MHz max.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_OUTPUT_10MHZ.
     */
    kOutput10MHz,
    /**
     * @brief Output Mode at 2MHz max.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_OUTPUT_2MHZ.
     */
    kOutput2MHz,
    /**
     * @brief Output Mode at 50MHz max.
     *
     * Equivalent to libopencm3 macro @c GPIO_MODE_OUTPUT_50MHZ.
     */
    kOutput50MHz
  };

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
     * Defaults to Configuration#kInputFloat.
     */
    Configuration cnf = Configuration::kInputFloat;

    /**
     * @brief GPIO Output Speed.
     *
     * Defaults to Mode#kInput.
     */
    Mode mode = Mode::kInput;
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
   * @param cnf GPIO Configuration
   * @param mode GPIO Mode
   */
  GPIO(Pinout pin, Configuration cnf, Mode mode);

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
 * @param cnf GPIO Configuration
 * @param mode GPIO Mode
 */
  void Init(Configuration cnf, Mode mode) const;

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
