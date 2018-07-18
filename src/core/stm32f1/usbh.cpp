#include "usbh.h"

USBH::USBH(const Config& config)
    : timer_(*config.timer_config) {
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
