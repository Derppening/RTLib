/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2018 Derppening <david.18.19.21@gmail.com>
 *
 * RTLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RTLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RTLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "core/stm32f1/uart.h"

#if defined(STM32F1)

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>

#include "config/config.h"
#include "core/assert.h"
#include "core/util.h"

namespace {
// TODO(Derppening): Refactor to use one handler per UART
libdev::UART::HandlerFn rx_handler = nullptr;

extern "C" void usart1_isr() {
  if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(USART1);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f1::UART;
      rx_handler(UART::kUART1, data);
    }
  }
}

extern "C" void usart2_isr() {
  if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART2) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(USART2);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f1::UART;
      rx_handler(UART::kUART2, data);
    }
  }
}

extern "C" void usart3_isr() {
  if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART3) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(USART3);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f1::UART;
      rx_handler(UART::kUART3, data);
    }
  }
}

extern "C" void uart4_isr() {
  if (((USART_CR1(UART4) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(UART4) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(UART4);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f1::UART;
      rx_handler(UART::kUART4, data);
    }
  }
}

extern "C" void uart5_isr() {
  if (((USART_CR1(UART5) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(UART5) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(UART5);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f1::UART;
      rx_handler(UART::kUART5, data);
    }
  }
}
}  // namespace

namespace rtlib::core::stm32f1 {
UART::UART(const Config& config) :
    usart_(uint32_t(config.uart)) {
  if (usart_ == kNullUART) {
    return;
  }

  GPIO::Config gpio_config;
  gpio_config.pin = GetTxPinout();
  gpio_config.cnf = GPIO::Configuration::kOutputAltFnPushPull;
  gpio_config.mode = GPIO::Mode::kOutput50MHz;
  tx_ = GPIO(gpio_config);
  gpio_config.pin = GetRxPinout();
  gpio_config.cnf = GPIO::Configuration::kInputFloat;
  gpio_config.mode = GPIO::Mode::kInput;
  rx_ = GPIO(gpio_config);

  InitRcc();
  EnableIrq();

  usart_set_baudrate(usart_, uint32_t(config.baud_rate));
  usart_set_databits(usart_, config.data_bits);
  usart_set_stopbits(usart_, uint32_t(config.stop_bits));
  usart_set_mode(usart_, uint32_t(config.mode));
  usart_set_parity(usart_, uint32_t(config.parity));
  usart_set_flow_control(usart_, uint32_t(config.flow_control));

  if (config.mode != Mode::kTx) {
    usart_enable_rx_interrupt(usart_);
  }

  usart_enable(usart_);

  rx_handler = config.rx_handler_fn;
}

UART::~UART() {
  Release();
}

void UART::Release() {
  if (!IsBinded()) {
    return;
  }

  Reset();

  tx_.Release();
  rx_.Release();

  usart_ = kNullUART;
}

void UART::Reset() const {
  if (!IsBinded()) {
    return;
  }

  // There is no need to check for usart_ perfect matches, because these are already checked in initialization
  // this saves us ~100 B in debug builds
  switch (usart_) {
    case USART1:
      rcc_periph_reset_pulse(RST_USART1);
      break;
    default:
      rcc_periph_reset_pulse(rcc_periph_rst(GetRCCRegister(0x10, std::uint8_t(17 + (usart_ - USART2_BASE) / 0x0400))));
      break;
  }
}

void UART::Tx(char c) const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(usart_ != kNullUART, __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  usart_send_blocking(usart_, uint16_t(c));
}

void UART::Tx(const char* str, std::size_t len) const {
#if defined(RTLIB_ENABLE_VALIDATION)
  Assert(usart_ != kNullUART, __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)

  for (std::size_t i = 0; i < len && *str != '\0'; ++str, ++i) {
    Tx(*str);
  }
}

constexpr void UART::InitRcc() const {
  // There is no need to check for usart_ perfect matches, because these are already checked in GPIO inits
  // this saves us ~100 B in debug builds
  switch (usart_) {
    case USART1:
      rcc_periph_clock_enable(RCC_USART1);
      break;
    default:
      rcc_periph_clock_enable(rcc_periph_clken(GetRCCRegister(0x1C, std::uint8_t(17 + (usart_ - USART2_BASE) / 0x0400))));
      break;
  }
}

constexpr GPIO::Pinout UART::GetTxPinout() const {
  switch (usart_) {
    case USART1:
      return {GPIOA, GPIO_USART1_TX};
    case USART2:
      return {GPIOA, GPIO_USART2_TX};
    case USART3:
      return {GPIOB, GPIO_USART3_TX};
    case UART4:
      return {GPIOC, GPIO_UART4_TX};
    case UART5:
      return {GPIOC, GPIO_UART5_TX};
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid or unsupported UART Interface");
      return {};
  }
}

constexpr GPIO::Pinout UART::GetRxPinout() const {
  switch (usart_) {
    case USART1:
      return {GPIOA, GPIO_USART1_RX};
    case USART2:
      return {GPIOA, GPIO_USART2_RX};
    case USART3:
      return {GPIOB, GPIO_USART3_RX};
    case UART4:
      return {GPIOC, GPIO_UART4_RX};
    case UART5:
      return {GPIOC, GPIO_UART5_RX};
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid or unsupported UART Interface");
      return {};
  }
}

constexpr void UART::EnableIrq() {
  switch (usart_) {
    case USART1:
      nvic_ = NVICInterrupt(NVICInterrupt::kUSART1);
      break;
    case USART2:
    case USART3:
      nvic_ = NVICInterrupt(NVICInterrupt::Source(NVIC_USART2_IRQ + (usart_ - USART2) / 0x0400));
      break;
    case UART4:
    case UART5:
      nvic_ = NVICInterrupt(NVICInterrupt::Source(NVIC_UART4_IRQ + (usart_ - UART4) / 0x0400));
      break;
    default:
      // Target UART cannot be used synchronously; Assert/ignore this request.
#if defined(RTLIB_ENABLE_VALIDATION)
      Assert(usart_ != kNullUART, __FILE__, __LINE__, __func__);
#endif  // defined(RTLIB_ENABLE_VALIDATION)
      break;
  }

  if (nvic_.IsBinded()) {
    nvic_.Enable();
  }
}

}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)
