#include "core/gpio.h"

#include <cassert>

#include <libopencm3/stm32/rcc.h>

GPIO::GPIO(const Config& config) :
    GPIO(config.pin, config.mode, config.speed) {}

GPIO::GPIO(const Pinout& pin, Mode mode, Speed speed) :
    pin_(pin) {
  // Use external oscillator for RCC
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Initialize the RCC and enable the GPIO
  InitRcc(pin_.first);
  Init(mode, speed);
}

void GPIO::Init(const Mode mode, const Speed speed) const {
  gpio_set_mode(pin_.first, speed, mode, pin_.second);
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
