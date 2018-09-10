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

#include "core/stm32f4/gpiov2.h"

#if defined(USE_GPIO_V2)
#if defined(STM32F4)

#include <cassert>
#include <utility>

#include <libopencm3/stm32/rcc.h>

#include "core/assert.h"
#include "util/ranges.h"

namespace rtlib::core::stm32f4 {

GPIOv2::~GPIOv2() {
#if defined(DTOR_RESET_PERIPH)
  Release();
#endif  // defined(DTOR_RESET_PERIPH)
}

void GPIOv2::Init(const Mode mode, const Pullup pullup, const Speed speed, const DriverType driver) const {
  gpio_mode_setup(GetPortMemMapping(), uint8_t(mode), uint8_t(pullup), GetPinMemMapping());
  gpio_set_output_options(GetPortMemMapping(), uint8_t(driver), uint8_t(speed), GetPinMemMapping());
}

void GPIOv2::InitRcc() const {
  rcc_periph_clock_enable(rcc_periph_clken(GetRCCRegister(0x30, std::uint8_t(pin_.first))));
}

bool GPIOv2::Read() const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  return bool(gpio_get(GetPortMemMapping(), GetPinMemMapping()));
}

void GPIOv2::Set(const bool state) const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  if (state) {
    gpio_set(GetPortMemMapping(), GetPinMemMapping());
  } else {
    gpio_clear(GetPortMemMapping(), GetPinMemMapping());
  }
}

void GPIOv2::Toggle() const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(IsBinded(), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  gpio_toggle(GetPortMemMapping(), GetPinMemMapping());
}

void GPIOv2::Release() {
  Reset();

  pin_ = kUnbindedPinout;
}

void GPIOv2::Reset() const {
  if (!IsBinded()) {
    return;
  }

  switch (pin_.first) {
    case Port::kGPIOA:
      switch (pin_.second) {
        case Pin::kGPIO15:
          Init(Mode::kAF, Pullup::kPullup, Speed::k2MHz, DriverType::kPushPull);
          break;
        case Pin::kGPIO14:
          Init(Mode::kAF, Pullup::kPulldown, Speed::k2MHz, DriverType::kPushPull);
          break;
        case Pin::kGPIO13:
          Init(Mode::kAF, Pullup::kPullup, Speed::k100MHz, DriverType::kPushPull);
          break;
        default:
          Init(Mode::kInput, Pullup::kNone, Speed::k2MHz, DriverType::kPushPull);
          break;
      }
      break;
    case Port::kGPIOB:
      switch (pin_.second) {
        case Pin::kGPIO4:
          Init(Mode::kAF, Pullup::kPullup, Speed::k2MHz, DriverType::kPushPull);
          break;
        case Pin::kGPIO3:
          Init(Mode::kAF, Pullup::kNone, Speed::k100MHz, DriverType::kPushPull);
          break;
        default:
          Init(Mode::kInput, Pullup::kNone, Speed::k2MHz, DriverType::kPushPull);
          break;
      }
      break;
    default:
      Init(Mode::kInput, Pullup::kNone, Speed::k2MHz, DriverType::kPushPull);
  }
}

void GPIOv2::SetAltFn(uint8_t altfn) const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(libutil::in_range<std::uint8_t>(altfn, 0, 15), __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  gpio_set_af(GetPortMemMapping(), altfn, GetPinMemMapping());
}

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)
#endif  // defined(USE_GPIO_V2)
