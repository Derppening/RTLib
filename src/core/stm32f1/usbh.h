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

#if defined(STM32F1)

#include <libopencm3/stm32/rcc.h>

#include "core/util.h"
#include "gpio.h"
#include "libusbhost/usbh_lld.h"
#include "timer.h"

namespace rtlib::core::stm32f1 {

/**
 * @brief STM32F1xx-specific HAL implementation for acting as USB Host.
 */
class USBH {
 public:
  /**
   * @brief Configuration for USBH.
   */
  struct Config {
    /**
     * Type definition for driver initialization function.
     */
    using DriverInitFn = void (*)();

    /**
     * @brief Array of low level drivers to enable.
     *
     * The list of available low-level drivers are listed in usbh_lld.h.
     *
     * This array must be terminated by a `nullptr`.
     */
    const usbh_low_level_driver_t* const* lld_drivers;
    /**
     * @brief Array of device drivers to use.
     *
     * The list of available device drivers are listed in usbh_driver_*.h.
     *
     * This array must be terminated by a `nullptr`.
     */
    const usbh_dev_driver_t** device_drivers;
    /**
     * @brief Function which initializes all device drivers in Config#device_drivers.
     *
     * If this field is `nullptr`, the constructing this object is undefined behavior.
     */
    DriverInitFn driver_init;
    /**
     * @brief Configuration for the timer to use for USB frame timekeeping.
     *
     * The timer must have a prescaler of `7199`, and a period of `65535`.
     */
    const libdev::Timer::Config* timer_config;
  };

  /**
   * @brief Conversion constructor.
   *
   * @param[in] config Configuration for the USB Host. See USBH#Config.
   */
  explicit USBH(const Config& config);

  /**
   * @brief Performs a poll as the USB Host.
   *
   * This action should be performed at least once per 1ms in the application main loop.
   */
  void Poll() { usbh_poll(GetTimeUs()); }

 private:
  /**
   * @return Current time in microseconds.
   */
  std::uint32_t GetTimeUs() { return timer_.GetCount() * 100; }

  libdev::Timer timer_;
  libdev::GPIO dm_;
  libdev::GPIO dp_;
};

}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)

#endif  // RTLIB_CORE_STM32F1_USBH_H_
