#include "core/gpio.h"

#include <cassert>
#include <utility>

#include <libopencm3/stm32/rcc.h>

GPIO::GPIO(const Config& config) :
#if defined(STM32F1)
    GPIO(config.pin, config.mode, config.speed) {}
#elif defined(STM32F4)
    GPIO(config.pin, config.mode, config.pullup, config.speed, config.driver) {}
#endif

#if defined(STM32F1)
GPIO::GPIO(const Pinout& pin, Mode mode, Speed speed) :
    pin_(pin) {
  // Use external oscillator for RCC
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Initialize the RCC and enable the GPIO
  InitRcc(pin_.first);
  Init(mode, speed);
}
#endif  // defined(STM32F1)

#if defined(STM32F4)
GPIO::GPIO(Pinout pin, Mode mode, Pullup pullup, Speed speed, DriverType driver) :
    pin_(std::move(pin)) {
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

  InitRcc(pin_.first);
  Init(mode, pullup, speed, driver);
}
#endif  // defined(STM32F4)

#if defined(STM32F1)
void GPIO::Init(const Mode mode, const Speed speed) const {
  gpio_set_mode(pin_.first, speed, mode, pin_.second);
}
#endif  // defined(STM32F1)

#if defined(STM32F4)
void GPIO::Init(const Mode mode, const Pullup pullup, const Speed speed, const DriverType driver) const {
  gpio_mode_setup(pin_.first, mode, pullup, pin_.second);
  gpio_set_output_options(pin_.first, driver, speed, pin_.second);
}
#endif  // defined(STM32F4)

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
#if defined(STM32F4)
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
#endif  // defined(STM32F4)
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
#if defined(STM32F1)
  gpio_set_mode(pin_.first, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, pin_.second);
#elif defined(STM32F4)
  gpio_mode_setup(pin_.first, GPIO_MODE_INPUT, GPIO_PUPD_NONE, pin_.second);
#endif
}
