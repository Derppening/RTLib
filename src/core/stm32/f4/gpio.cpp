#if defined(STM32F4)

#include "core/stm32/f4/gpio.h"

namespace rtlib::core::stm32::f4 {

gpio::gpio(const ports port, const std::uint8_t pin) :
    _mmio(reinterpret_cast<mmio*>(std::uint32_t(port))),
    _pin(pin) {}

void gpio::mode(const gpio::modes mode) {
  _mmio->moder = reset_and_set(_mmio->moder, _pin * 2, 2, std::uint32_t(mode));
}

gpio::modes gpio::mode() const {
  return gpio::modes(get_bitmask(_mmio->moder, _pin * 2, 2));
}

void gpio::otype(const gpio::otypes otype) {
  _mmio->otyper = reset_and_set(_mmio->otyper, _pin * 2, 1, std::uint32_t(otype));
}

gpio::otypes gpio::otype() const {
  return gpio::otypes(get_bitmask(_mmio->otyper, _pin * 2, 1));
}

void gpio::ospeed(const gpio::ospeeds ospeed) {
  _mmio->ospeedr = reset_and_set(_mmio->ospeedr, _pin * 2, 2, std::uint32_t(ospeed));
}

gpio::ospeeds gpio::ospeed() const {
  return gpio::ospeeds(get_bitmask(_mmio->ospeedr, _pin * 2, 2));
}

void gpio::pupd(const gpio::pupds pupd) {
  _mmio->pupdr = reset_and_set(_mmio->pupdr, _pin * 2, 2, std::uint32_t(pupd));
}

gpio::pupds gpio::pupd() const {
  return gpio::pupds(get_bitmask(_mmio->pupdr, _pin * 2, 2));
}

void gpio::af(const std::uint8_t af) {
  _mmio->afr = reset_and_set(_mmio->afr, _pin * 4, 4, std::uint64_t(af));
}

std::uint8_t gpio::af() const {
  return std::uint8_t(get_bitmask(_mmio->afr, _pin * 4, 4));
}

void gpio::state(const bool new_state) const {
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

}  // namespace rtlib::core::stm32::f4

#endif  // defined(STM32F4)
