/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2017-2018 Derppening <david.18.19.21@gmail.com>
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

#include "config/config.h"

#if defined(LIB_USE_LED) && LIB_USE_LED > 0

#include "lib/led.h"
#include "core/assert.h"

using libcore::Pinout;
using libdev::GPIO;

namespace {

constexpr Pinout GetConfigPinout(const uint8_t id) {
  Assert(id < LIB_USE_LED, __FILE__, __LINE__, __func__, "Invalid LED ID");
  switch (id) {
    default:
      // not handled, since assert will catch this error
      return {};
#if LIB_USE_LED > 0
    case 0:
      return LIB_LED0_PINOUT;
#endif  // LIB_USE_LED > 0
#if LIB_USE_LED > 1
    case 1:
      return LIB_LED1_PINOUT;
#endif  // LIB_USE_LED > 1
#if LIB_USE_LED > 2
    case 2:
      return LIB_LED2_PINOUT;
#endif  // LIB_USE_LED > 2
  }
}

}  // namespace

namespace rtlib::lib {

Led::Led(const Config& config) :
#if defined(STM32F1)
    gpio_(GetConfigPinout(config.id), GPIO::Configuration::kOutputPushPull, GPIO::Mode::kOutput50MHz),
#elif defined(STM32F4)
    gpio_(GetConfigPinout(config.id), GPIO::Mode::kOutput, GPIO::Pullup::kNone, GPIO::Speed::k50MHz),
#endif
    polarity_(config.polarity) {
  SetEnable(false);
}

void Led::SetEnable(const bool flag) {
  gpio_.Set(polarity_ ^ flag);
}

void Led::Switch() {
  gpio_.Toggle();
}

}  // namespace rtlib::lib

#elif !defined(LIB_USE_LED)
#error "LIB_USE_LED macro not found. (Did you define it in your board configuration?)"
#endif
