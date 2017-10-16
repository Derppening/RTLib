#include "config/config.h"

#if defined(LIB_USE_BUTTON) && LIB_USE_BUTTON > 0

#include "lib/button.h"

#include <cassert>

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
#error "STM32F1xx configuration incomplete. Please file a bug report on Github."
// TODO(Derppening): STM32F1 button config
#elif defined(STM32F4)
    gpio_(GetConfigPinout(config.id), GPIO_MODE_INPUT, config.pullup, GPIO_OSPEED_50MHZ),
#endif
    polarity_(config.pullup == PullupState::kPullUp) {
}

bool Button::Read() {
  return static_cast<bool>(gpio_.Read() ^ polarity_);
}

#elif !defined(LIB_USE_BUTTON)
#error "LIB_USE_BUTTON macro not found. (Did you define it in your board configuration?)"
#endif
