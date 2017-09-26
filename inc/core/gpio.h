#ifndef RTLIB_GPIO_H_
#define RTLIB_GPIO_H_

#include <cstdint>
#include <utility>

#include <libopencm3/stm32/gpio.h>

/**
 * @brief HAL implementation for GPIO pins.
 *
 * This abstraction layer provides simple Read/Write operations on GPIO pins. One GPIO object will correspond and manage
 * one GPIO pinout from the MCU.
 */
class GPIO {
 public:
  /**
   * @brief Type definition for GPIO Port.
   */
  using Port = unsigned;
  /**
   * @brief Type definition for GPIO Pin.
   */
  using Pin = uint16_t;
  /**
   * @brief Type definition for GPIO Mode.
   */
  using Mode = uint8_t;
  /**
   * @brief Type definition for GPIO Output Speed.
   */
  using Speed = uint8_t;
  /**
   * @brief Type definition for MCU Pinout.
   */
  using Pinout = std::pair<Port, Pin>;

  struct Config {
    /**
     * @brief GPIO Pin to enable and initialize.
     */
    Pinout pin;
    /**
     * @brief GPIO Mode.
     *
     * See http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__cnf.html#gafdb5d21e4c813c645906c8b7e9472daf for
     * a list of available configurations.
     */
    Mode mode;
    /**
     * @brief GPIO Output Speed.
     *
     * See http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__mode.html#gaf40bec3146810028a84b628d37d3b391 for
     * a list of available pin modes.
     */
    Speed speed = GPIO_MODE_OUTPUT_50_MHZ;
  };

  /**
   * @brief Default constructor for GPIO.
   *
   * @param config Configuration for the GPIO. See GPIO#Config.
   */
  explicit GPIO(const Config& config);

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
   * std::unique_ptr
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
  /**
   * @brief Initializes this GPIO to the given configuration.
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

 private:
  /**
   * @brief MCU Pinout of the currently managed GPIO.
   */
  Pinout pin_;
};

#endif  // RTLIB_GPIO_H_