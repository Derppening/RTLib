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
#include "util/ranges.h"

namespace rtlib::core::stm32f4 {

GPIO::GPIO(const Config& config) :
    pin_(AssertPin(config.pin, __FILE__, __LINE__, __func__)) {
  // Use external oscillator for RCC
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

  // Initialize the RCC and enable the GPIO
  InitRcc();
  Init(config.mode, config.pullup, config.speed, config.driver);

  // Set the alternate function
  SetAltFn(config.altfn);
}

GPIO::~GPIO() {
  Release();
}

void GPIO::Init(const Mode mode, const Pullup pullup, const Speed speed, const DriverType driver) const {
  gpio_mode_setup(pin_.first, uint8_t(mode), uint8_t(pullup), pin_.second);
  gpio_set_output_options(pin_.first, uint8_t(driver), uint8_t(speed), pin_.second);
}

constexpr void GPIO::InitRcc() const {
  switch (pin_.first) {
    case GPIOA:
    case GPIOB:
    case GPIOC:
    case GPIOD:
    case GPIOE:
    case GPIOF:
    case GPIOG:
    case GPIOH:
    case GPIOI:
    case GPIOJ:
    case GPIOK:
      rcc_periph_clock_enable(rcc_periph_clken(GetRCCRegister(0x30, std::uint8_t((pin_.first - GPIO_PORT_A_BASE) / 0x0400))));
      break;
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid GPIO Port");
      break;
  }
}

bool GPIO::Read() const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  return bool(gpio_get(pin_.first, pin_.second));
}

void GPIO::Set(const bool state) const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  if (state) {
    gpio_set(pin_.first, pin_.second);
  } else {
    gpio_clear(pin_.first, pin_.second);
  }
}

void GPIO::Toggle() const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  gpio_toggle(pin_.first, pin_.second);
}

void GPIO::Release() {
  if (!IsBinded()) {
    return;
  }

  Reset();

  pin_ = kNullPinout;
}

void GPIO::Reset() const {
  if (!IsBinded()) {
    return;
  }

  if (pin_.first == GPIOA) {
    switch (pin_.second) {
      case GPIO15:
        Init(Mode::kAF, Pullup::kPullup, Speed::k2MHz, DriverType::kPushPull);
        break;
      case GPIO14:
        Init(Mode::kAF, Pullup::kPulldown, Speed::k2MHz, DriverType::kPushPull);
        break;
      case GPIO13:
        Init(Mode::kAF, Pullup::kPullup, Speed::k100MHz, DriverType::kPushPull);
        break;
      default:
        Init(Mode::kInput, Pullup::kNone, Speed::k2MHz, DriverType::kPushPull);
        break;
    }
  } else if (pin_.first == GPIOB) {
    switch (pin_.second) {
      case GPIO4:
        Init(Mode::kAF, Pullup::kPullup, Speed::k2MHz, DriverType::kPushPull);
        break;
      case GPIO3:
        Init(Mode::kAF, Pullup::kNone, Speed::k100MHz, DriverType::kPushPull);
        break;
      default:
        Init(Mode::kInput, Pullup::kNone, Speed::k2MHz, DriverType::kPushPull);
        break;
    }
  } else {
    Init(Mode::kInput, Pullup::kNone, Speed::k2MHz, DriverType::kPushPull);
  }
}

void GPIO::SetAltFn(uint8_t altfn) const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(libutil::in_range<std::uint8_t>(altfn, 0, 15), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  gpio_set_af(pin_.first, altfn, pin_.second);
}

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)
