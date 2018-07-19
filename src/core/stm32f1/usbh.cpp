/*
 * This file is part of RTLib, ported from the libusbhost library
 * (https://github.com/libusbhost/libusbhost).
 *
 * Copyright (C) 2015 Amir Hammad <amir.hammad@hotmail.com>
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

#include "core/stm32f1/usbh.h"

#if defined(STM32F1)

namespace rtlib::core::stm32f1 {

USBH::USBH(const Config& config)
    : timer_(*config.timer_config) {
  // TODO(Derppening): Initialize the timer's prescaler and period here.

  rcc_periph_clock_enable(RCC_OTGFS);

  libdev::GPIO::Config gpio_config;
  gpio_config.mode = libdev::GPIO::Mode::kOutput50MHz;
  gpio_config.cnf = libdev::GPIO::Configuration::kOutputAltFnPushPull;
  gpio_config.pin = {GPIOA, GPIO11};
  dm_ = libdev::GPIO(gpio_config);
  gpio_config.pin = {GPIOA, GPIO12};
  dp_ = libdev::GPIO(gpio_config);

  timer_.Start();

  config.driver_init();

  usbh_init(config.lld_drivers, config.device_drivers);
}

}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)
