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

#ifndef RTLIB_CORE_STM32F1_USBH_H_
#define RTLIB_CORE_STM32F1_USBH_H_

#include <libopencm3/stm32/rcc.h>

#include "core/util.h"
#include "gpio.h"
#include "libusbhost/usbh_lld.h"
#include "timer.h"

class USBH {
 public:
  struct Config {
    using DriverInitFn = void (*)();

    const usbh_low_level_driver_t* const* lld_drivers;
    const usbh_dev_driver_t** device_drivers;
    DriverInitFn driver_init;
    const libdev::Timer::Config* timer_config;
  };

  explicit USBH(const Config& config);

  void Poll() { usbh_poll(GetTimeUs()); }

 private:
  std::uint32_t GetTimeUs() { return timer_.GetCount() * 100; }

  libdev::Timer timer_;
  libdev::GPIO dm_;
  libdev::GPIO dp_;
};

#endif  // RTLIB_CORE_STM32F1_USBH_H_
