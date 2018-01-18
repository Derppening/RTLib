#include "core/stm32f1/gpio.h"

#if defined(STM32F1)

#include <cassert>
#include <utility>

#include <libopencm3/stm32/rcc.h>

namespace core {
namespace stm32f1 {

GPIO::GPIO(const Config& config) :
    GPIO(config.pin, config.cnf, config.mode) {}

GPIO::GPIO(Pinout pin, Configuration cnf, Mode mode) :
    pin_(std::move(pin)) {
  // Use external oscillator for RCC
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Initialize the RCC and enable the GPIO
  InitRcc(pin_.first);
  Init(cnf, mode);
}

void GPIO::Init(const Configuration cnf, const Mode mode) const {
  gpio_set_mode(pin_.first, static_cast<uint8_t>(mode), static_cast<uint8_t>(cnf), pin_.second);
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
  gpio_set_mode(pin_.first, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, pin_.second);
}

}  // namespace stm32f1
}  // namespace core

#endif  // defined(STM32F1)
