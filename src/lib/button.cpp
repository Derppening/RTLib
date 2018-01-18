#include "config/config.h"

#if defined(LIB_USE_BUTTON) && LIB_USE_BUTTON > 0

#include "lib/button.h"

#include <cassert>

using CORE_NS::GPIO;

namespace {
inline Pinout GetConfigPinout(const uint8_t id) {
  assert(id < LIB_USE_BUTTON);
  switch (id) {
    default:
      // not handled, since assert will catch this error
      assert(false);
      break;
#if LIB_USE_BUTTON > 0
    case 0:
      return LIB_BUTTON0_PINOUT;
#endif  // LIB_USE_BUTTON > 0
#if LIB_USE_BUTTON > 1
    case 1:
      return LIB_BUTTON1_PINOUT;
#endif  // LIB_USE_BUTTON > 1
#if LIB_USE_BUTTON > 2
    case 2:
      return LIB_BUTTON2_PINOUT;
#endif  // LIB_USE_BUTTON > 2
  }
}
}  // namespace

Button::Button(const Config& config) :
#if defined(STM32F1)
    gpio_(GetConfigPinout(config.id), GPIO::Configuration::kInputPullUpDown, GPIO::Mode::kInput),
#elif defined(STM32F4)
    gpio_(GetConfigPinout(config.id),
          GPIO::Mode::kInput,
          static_cast<GPIO::Pullup>(config.pullup),
          GPIO::Speed::k50MHz),
#endif
    polarity_(config.pullup == 0x1) {
}

bool Button::Read() {
  return static_cast<bool>(gpio_.Read() ^ polarity_);
}

#elif !defined(LIB_USE_BUTTON)
#error "LIB_USE_BUTTON macro not found. (Did you define it in your board configuration?)"
#endif
