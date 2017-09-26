#ifndef RTLIB_LIB_LED_H_
#define RTLIB_LIB_LED_H_

#include <cstdint>

#include "config/config.h"
#include "core/gpio.h"

#if !defined(LIB_USE_LED) || LIB_USE_LED < 1
#error "This library is not configured to use LED."
#endif  // !defined(LIB_USE_LED) || LIB_USE_LED < 1

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
