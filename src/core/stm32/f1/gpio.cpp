#if defined(STM32F1)

#include "core/stm32/f1/gpio.h"

namespace rtlib::core::stm32::f1 {

gpio::gpio(const gpio::ports port, const std::uint8_t pin) :
    _mmio(reinterpret_cast<mmio*>(std::uint32_t(port))),
    _pin(pin) {}

void gpio::cnf(const gpio::cnfs configuration) {
  _mmio->cr = reset_and_set(_mmio->cr, _pin * 4 + 2, 2, std::uint64_t(configuration));
}

gpio::cnfs gpio::cnf() const {
  return gpio::cnfs(get_bitmask(_mmio->cr, _pin * 4 + 2, 2));
}

void gpio::mode(const gpio::modes mode) {
  _mmio->cr = reset_and_set(_mmio->cr, _pin * 4, 2, std::uint64_t(mode));
}

gpio::modes gpio::mode() const {
  return gpio::modes(get_bitmask(_mmio->cr, _pin * 4, 2));
}

void gpio::state(const bool new_state) {
  auto v = std::uint32_t(1) << (_pin + (16 * std::uint8_t(!new_state)));
  _mmio->bsrr = v;
}

bool gpio::state() const {
  if (mode() == gpio::modes::input) {
    return bool(get_bitmask(_mmio->idr, _pin, 1));
  }

  return bool(get_bitmask(_mmio->odr, _pin, 1));
}

bool gpio::toggle() {
  bool state = !this->state();
  this->state(state);

  return state;
}

}  // namespace rtlib::core::stm32::f1

#endif  // defined(STM32F1)
