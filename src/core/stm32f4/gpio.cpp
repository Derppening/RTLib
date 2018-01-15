#include "core/stm32f4/gpio.h"

#if defined(STM32F4)

#include <cassert>
#include <utility>

#include <libopencm3/stm32/rcc.h>

namespace core {
namespace stm32f4 {

GPIO::GPIO(const Config& config) :
    GPIO(config.pin, config.mode, config.pullup, config.speed, config.driver) {}

GPIO::GPIO(Pinout pin, Mode mode, Pullup pullup, Speed speed, DriverType driver) :
    pin_(std::move(pin)) {
  // Use external oscillator for RCC
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

  // Initialize the RCC and enable the GPIO
  InitRcc(pin_.first);
  Init(mode, pullup, speed, driver);
}

void GPIO::Init(const Mode mode, const Pullup pullup, const Speed speed, const DriverType driver) const {
  gpio_mode_setup(pin_.first, mode, pullup, pin_.second);
  gpio_set_output_options(pin_.first, driver, speed, pin_.second);
}

void GPIO::InitRcc(const Port port) const {
  switch (port) {
    case GPIOA:
      rcc_periph_clock_enable(RCC_GPIOA);
      break;
    case GPIOB:
      rcc_periph_clock_enable(RCC_GPIOB);
      break;
    case GPIOC:
      rcc_periph_clock_enable(RCC_GPIOC);
      break;
    case GPIOD:
      rcc_periph_clock_enable(RCC_GPIOD);
      break;
    case GPIOE:
      rcc_periph_clock_enable(RCC_GPIOE);
      break;
    case GPIOF:
      rcc_periph_clock_enable(RCC_GPIOF);
      break;
    case GPIOG:
      rcc_periph_clock_enable(RCC_GPIOG);
      break;
    case GPIOH:
      rcc_periph_clock_enable(RCC_GPIOH);
      break;
    case GPIOI:
      rcc_periph_clock_enable(RCC_GPIOI);
      break;
    case GPIOJ:
      rcc_periph_clock_enable(RCC_GPIOJ);
      break;
    case GPIOK:
      rcc_periph_clock_enable(RCC_GPIOK);
      break;
    default:
      assert(false);
      break;
  }
}

bool GPIO::Read() const {
  return static_cast<bool>(gpio_get(pin_.first, pin_.second));
}

void GPIO::Set(const bool state) const {
  if (state) {
    gpio_set(pin_.first, pin_.second);
  } else {
    gpio_clear(pin_.first, pin_.second);
  }
}

void GPIO::Toggle() const {
  gpio_toggle(pin_.first, pin_.second);
}

void GPIO::Reset() const {
  gpio_mode_setup(pin_.first, GPIO_MODE_INPUT, GPIO_PUPD_NONE, pin_.second);
}

}  // namespace f4
}  // namespace stm32

#endif  // defined(STM32F4)
