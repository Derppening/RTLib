#ifndef RTLIB_LIB_BUTTON_H_
#define RTLIB_LIB_BUTTON_H_

#include <cstdint>

#include "config/config.h"
#include "core/gpio.h"

static_assert(LIB_USE_BUTTON > 0, "Button library is disabled in your configuration.");

/**
 * @brief HAL implementation for buttons.
 * 
 * This abstraction layer provides standard state-reading for onboard buttons. One button object is designed to manage
 * one button on the mainboard.
 */
class Button {
 public:
#if defined(STM32F4)
  /**
   * @brief Enumeration for different internal pull-up/pull-down states for STM32F4xx.
   */
  enum PullupState : uint8_t {
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
    kPullUp = 0x1,
    /**
     * @brief Use the internal pull-down resistor.
     * 
     * Equivalent to libopencm3 macro @c GPIO_PUPD_PULLDOWN.
     */
    kPullDown = 0x2
  };
#endif  // defined(STM32F4)

  /**
   * @brief Configuration for button.
   */
  struct Config {
    /**
     * @brief ID of the button.
     * 
     * See your device configuration header file to see which id corresponds to which hardware button.
     */
    uint8_t id;
    /**
     * @brief Specifies default state of the button.
     *
     * For STM32F1xx, specifies if the button is active high.
     *
     * For STM32F4xx, specifies whether to use the internal pull-up/pull-down resistor. You may use the
     * Button#PullupState enumeration or libopencm3 macros.
     */
    uint8_t pullup;
  };

  /**
   * @brief Default constructor for Button.
   *
   * @param config Button configuration
   */
  explicit Button(const Config& config);
  
  /**
   * @brief Default trivial destructor.
   */
  ~Button() = default;

  /**
   * @brief Move constructor for Button.
   *
   * @param other Button object to move from
   */
  Button(Button&& other) noexcept = default;
  /**
   * @brief Move assignment operator for Button.
   *
   * @param other Button object to move from
   * @return Reference to the moved Button.
   */
  Button& operator=(Button&& other) noexcept = default;

  /**
   * @brief Copy constructor for Button.
   *
   * This constructor is deleted because there should only be one object managing each button, similar to @c
   * std::unique_ptr.
   */
  Button(const Button&) = delete;
  /**
   * @brief Copy assignment operator for Button.
   *
   * This constructor is deleted because there should only be one object managing each button, similar to @c
   * std::unique_ptr.
   */
  Button& operator=(const Button&) = delete;

  /**
   * @brief Reads the state of the button.
   *
   * @note Button will always be in low state at idle after constructor is calButton. If it isn't, your
   * Button#Config#pullup may be incorrect; Set it to another value or consult a hardware senior if that is the case.
   *
   * @return @c true if button is pressed, @c false otherwise.
   */
  bool Read();

 protected:
  /**
   * @return GPIO object which manages the pin of the button
   */
  GPIO* GetGpio() { return &gpio_; }

 private:
  GPIO gpio_;
  bool polarity_;
};

#endif  // RTLIB_LIB_BUTTON_H_
