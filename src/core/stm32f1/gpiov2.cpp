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

#include "core/stm32f1/gpiov2.h"

#if defined(USE_GPIO_V2)
#if defined(STM32F1)

#include <cassert>
#include <utility>

#include <libopencm3/stm32/rcc.h>

#include "core/assert.h"
#include "util/ranges.h"

namespace rtlib::core::stm32f1 {

GPIOv2::~GPIOv2() {
#if defined(DTOR_RESET_PERIPH)
  Release();
#endif  // defined(DTOR_RESET_PERIPH)
}

void GPIOv2::Init(const Configuration cnf, const Mode mode) const {
  gpio_set_mode(std::uint32_t(pin_.first), uint8_t(mode), uint8_t(cnf), std::uint16_t(pin_.second));
}

void GPIOv2::InitRcc() const {
  rcc_periph_clock_enable(rcc_periph_clken(GetRCCRegister(0x18, std::uint8_t(2 + std::uint8_t(pin_.first)))));
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

  Init(Configuration::kInputFloat, Mode::kInput);
}

void GPIOv2::SetPriAltFn(JTAGDisables swj_state, AltFnMaps maps) {
  gpio_primary_remap(uint32_t(swj_state), maps);
}

void GPIOv2::SetSecAltFn(AltFnMaps maps) {
  gpio_secondary_remap(maps);
}

}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)
#endif  // defined(USE_GPIO_V2)
