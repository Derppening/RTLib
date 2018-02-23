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

#include "core/stm32f4/gpio.h"

#if defined(STM32F4)

#include <cassert>
#include <utility>

#include <libopencm3/stm32/rcc.h>

#include "core/assert.h"

namespace rtlib::core::stm32f4 {

GPIO::GPIO(const Config& config) :
    GPIO(config.pin, config.mode, config.pullup, config.speed, config.driver, config.altfn) {}

GPIO::GPIO(Pinout pin, Mode mode, Pullup pullup, Speed speed, DriverType driver, AltFn altfn) :
    pin_(AssertPin(std::move(pin), __FILE__, __LINE__, __func__)) {
  // Use external oscillator for RCC
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

  // Initialize the RCC and enable the GPIO
  InitRcc(pin_.first);
  Init(mode, pullup, speed, driver);

  // Set the alternate function if user specifies one
  if (mode == Mode::kAF) {
    SetAltFn(altfn);
  }
}

void GPIO::Init(const Mode mode, const Pullup pullup, const Speed speed, const DriverType driver) const {
  gpio_mode_setup(pin_.first, static_cast<uint8_t>(mode), static_cast<uint8_t>(pullup), pin_.second);
  gpio_set_output_options(pin_.first, static_cast<uint8_t>(driver), static_cast<uint8_t>(speed), pin_.second);
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

void GPIO::SetAltFn(uint8_t altfn) {
  // Guard users from invalid configs
  if (altfn > 0xF) {
    return;
  }

  gpio_set_af(pin_.first, altfn, pin_.second);
}

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)
