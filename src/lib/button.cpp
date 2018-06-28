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

#include "core/assert.h"

using libcore::Pinout;
using libdev::GPIO;

namespace {

constexpr libcore::Pinout GetConfigPinout(const uint8_t id) {
  Assert(id < LIB_USE_BUTTON, __FILE__, __LINE__, __func__, "Invalid Button ID");
  switch (id) {
    default:
      // not handled, since assert will catch this error
      return {};
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
    polarity_(config.pullup == 0x1) {
#if defined(STM32F1)
  GPIO::Config gpio_config;
  gpio_config.pin = GetConfigPinout(config.id);
  gpio_config.cnf = GPIO::Configuration::kInputPullUpDown;
  gpio_config.mode = GPIO::Mode::kInput;
  gpio_ = GPIO(gpio_config);
#elif defined(STM32F4)
  gpio_ = GPIO(GetConfigPinout(config.id),
          GPIO::Mode::kInput,
          GPIO::Pullup(config.pullup),
          GPIO::Speed::k50MHz),
#endif
}

bool Button::Read() {
  return bool(gpio_.Read() ^ polarity_);
}

}  // namespace rtlib::lib

#elif !defined(LIB_USE_BUTTON)
#error "LIB_USE_BUTTON macro not found. (Did you define it in your board configuration?)"
#endif
