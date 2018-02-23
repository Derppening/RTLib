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

#if defined(LIB_USE_BUTTON) && LIB_USE_BUTTON > 0

#include "lib/button.h"

#include <cassert>

using CORE_NS::Pinout;
using DEVICE_NS::GPIO;

namespace {

inline CORE_NS::Pinout GetConfigPinout(const uint8_t id) {
  assert(id < LIB_USE_BUTTON);
  switch (id) {
    default:
      // not handled, since assert will catch this error
      assert(false);
      break;
#if LIB_USE_BUTTON > 0
    case 0:
      return LIB_BUTTON0_PINOUT;
#endif  // LIB_USE_BUTTON > 0
#if LIB_USE_BUTTON > 1
    case 1:
      return LIB_BUTTON1_PINOUT;
#endif  // LIB_USE_BUTTON > 1
#if LIB_USE_BUTTON > 2
    case 2:
      return LIB_BUTTON2_PINOUT;
#endif  // LIB_USE_BUTTON > 2
  }
}

}  // namespace

namespace rtlib::lib {

Button::Button(const Config& config) :
#if defined(STM32F1)
    gpio_(GetConfigPinout(config.id), GPIO::Configuration::kInputPullUpDown, GPIO::Mode::kInput),
#elif defined(STM32F4)
    gpio_(GetConfigPinout(config.id),
          GPIO::Mode::kInput,
          GPIO::Pullup(config.pullup),
          GPIO::Speed::k50MHz),
#endif
    polarity_(config.pullup == 0x1) {
}

bool Button::Read() {
  return bool(gpio_.Read() ^ polarity_);
}

}  // namespace rtlib::lib

#elif !defined(LIB_USE_BUTTON)
#error "LIB_USE_BUTTON macro not found. (Did you define it in your board configuration?)"
#endif
