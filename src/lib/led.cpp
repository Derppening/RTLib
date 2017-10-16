#include "config/config.h"

#if defined(LIB_USE_LED) && LIB_USE_LED > 0

#include "lib/led.h"

#include <cassert>

namespace {
inline Pinout GetConfigPinout(const uint8_t id) {
  assert(id < LIB_USE_LED);
  switch (id) {
    default:
      // not handled, since assert will catch this error
      assert(false);
      break;
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

Led::Led(const Config& config) :
#if defined(STM32F1)
    gpio_(GetConfigPinout(config.id), GPIO_CNF_OUTPUT_PUSHPULL, GPIO_MODE_OUTPUT_50_MHZ),
#elif defined(STM32F4)
    gpio_(GetConfigPinout(config.id), GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_OSPEED_50MHZ),
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

#elif !defined(LIB_USE_LED)
#error "LIB_USE_LED macro not found. (Did you define it in your board configuration?)"
#endif
