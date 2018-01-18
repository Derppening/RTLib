#include "config/config.h"

#if defined(LIB_USE_LED) && LIB_USE_LED > 0

#include "lib/led.h"

#include <cassert>

using CORE_NS::GPIO;

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
    gpio_(GetConfigPinout(config.id), GPIO::Configuration::kOutputPushPull, GPIO::Mode::kOutput50MHz),
#elif defined(STM32F4)
    gpio_(GetConfigPinout(config.id), GPIO::Mode::kOutput, GPIO::Pullup::kNone, GPIO::Speed::k50MHz),
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
