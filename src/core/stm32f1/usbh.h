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
