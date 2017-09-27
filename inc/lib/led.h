#ifndef RTLIB_LIB_LED_H_
#define RTLIB_LIB_LED_H_

#include <cstdint>

#include "config/config.h"
#include "core/gpio.h"

static_assert(LIB_USE_LED, "Led library is disabled in your configuration.");

class Led {
 public:
  struct Config {
    uint8_t id = 0;
    bool polarity = false;
  };

  explicit Led(const Config& config);

  void SetEnable(bool flag);

  void Switch();

 private:
  GPIO gpio_;
  bool polarity_;
};

#endif  // RTLIB_LIB_LED_H_
