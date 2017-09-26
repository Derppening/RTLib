#include "config/config.h"

#if defined(LIB_USE_LED)

#include "lib/led.h"

#include <cassert>

namespace {
inline Pinout GetPinout(const uint8_t id) {
  assert(id < LIB_USE_LED);
  switch (id) {
    default:
      // not handled, since assert will catch this error
      assert(false);
      break;
#if LIB_USE_LED > 0
    case 0:
      return LIB_LED1_PINOUT;
#endif
  }
}
}  // namespace

Led::Led(const Config& config) :
    gpio_(::GetPinout(config.id), GPIO_CNF_OUTPUT_PUSHPULL, GPIO_MODE_OUTPUT_50_MHZ),
    polarity_(config.polarity) {}

void Led::SetEnable(const bool flag) {
  gpio_.Set(polarity_ == !flag);
}

void Led::Switch() {
  gpio_.Toggle();
}

#endif  // defined(LIB_USE_LED)
