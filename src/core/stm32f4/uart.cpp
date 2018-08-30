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

#include "core/stm32f4/uart.h"

#if defined(STM32F4)

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>

#include "config/config.h"
#include "core/assert.h"
#include "core/nvic_interrupt.h"
#include "core/util.h"

namespace {
rtlib::core::stm32f4::UART::HandlerFn rx_handler = nullptr;

extern "C" void usart1_isr() {
  if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(USART1);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f4::UART;
      rx_handler(UART::kUART1, data);
    }
  }
}

extern "C" void usart2_isr() {
  if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART2) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(USART2);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f4::UART;
      rx_handler(UART::kUART2, data);
    }
  }
}

extern "C" void usart3_isr() {
  if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART3) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(USART3);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f4::UART;
      rx_handler(UART::kUART3, data);
    }
  }
}

extern "C" void uart4_isr() {
  if (((USART_CR1(UART4) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(UART4) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(UART4);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f4::UART;
      rx_handler(UART::kUART4, data);
    }
  }
}

extern "C" void uart5_isr() {
  if (((USART_CR1(UART5) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(UART5) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(UART5);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f4::UART;
      rx_handler(UART::kUART5, data);
    }
  }
}

extern "C" void usart6_isr() {
  if (((USART_CR1(USART6) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART6) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(USART6);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f4::UART;
      rx_handler(UART::kUART6, data);
    }
  }
}

extern "C" void uart7_isr() {
  if (((USART_CR1(UART7) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(UART7) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(UART7);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f4::UART;
      rx_handler(UART::kUART7, data);
    }
  }
}

extern "C" void uart8_isr() {
  if (((USART_CR1(UART8) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(UART8) & USART_SR_RXNE) != 0)) {

    uint16_t data = usart_recv(UART8);
    if (rx_handler != nullptr) {
      using rtlib::core::stm32f4::UART;
      rx_handler(UART::kUART8, data);
    }
  }
}
}  // namespace

namespace rtlib::core::stm32f4 {

UART::UART(const Config& config) :
    usart_(uint32_t(config.uart)) {
  if (usart_ == kNullUART) {
    return;
  }

  GPIO::Config gpio_config;
  gpio_config.pin = GetTxPinout();
  gpio_config.mode = GPIO::Mode::kAF;
  gpio_config.speed = GPIO::Speed::k50MHz;
  gpio_config.driver = GPIO::DriverType::kPushPull;
  gpio_config.altfn = GetAltFn();
  tx_ = GPIO(gpio_config);
  gpio_config.pin = GetRxPinout();
  gpio_config.driver = GPIO::DriverType::kOpenDrain;
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

  switch (usart_) {
    case USART1:
      rcc_periph_reset_pulse(RST_USART1);
      break;
    case USART6:
      rcc_periph_reset_pulse(RST_USART6);
      break;
    default:
      rcc_periph_reset_pulse(rcc_periph_rst(GetRCCRegister(0x20, std::uint8_t(17 + (usart_ - USART2_BASE) / 0x0400))));
      break;
  }
}

void UART::Tx(char c) const {
  usart_send_blocking(usart_, uint16_t(c));
}

void UART::Tx(const char* str, std::size_t len) const {
  for (std::size_t i = 0; i < len && *str != '\0'; ++str, ++i) {
    Tx(*str);
  }
}

constexpr void UART::InitRcc() const {
  switch (usart_) {
    case USART1:
      rcc_periph_clock_enable(RCC_USART1);
      break;
    case USART6:
      rcc_periph_clock_enable(RCC_USART6);
      break;
    default:
      rcc_periph_clock_enable(rcc_periph_clken(GetRCCRegister(0x40, std::uint8_t(17 + (usart_ - USART2_BASE) / 0x0400))));
      break;
  }
}

constexpr Pinout UART::GetTxPinout() const {
  switch (usart_) {
    case USART1:
      return CORE_UART1_TX_PINOUT;
    case USART2:
      return CORE_UART2_TX_PINOUT;
    case USART3:
      return CORE_UART3_TX_PINOUT;
    case UART4:
      return CORE_UART4_TX_PINOUT;
    case UART5:
      return {GPIOC, GPIO12};
    case USART6:
      return CORE_UART6_TX_PINOUT;
    case UART7:
    case UART8:
      TODO(__FILE__, __LINE__, __func__);
      [[fallthrough]];
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid or unsupported UART Interface");
      return {};
  }
}

constexpr Pinout UART::GetRxPinout() const {
  switch (usart_) {
    case USART1:
      return CORE_UART1_RX_PINOUT;
    case USART2:
      return CORE_UART2_RX_PINOUT;
    case USART3:
      return CORE_UART3_RX_PINOUT;
    case UART4:
      return CORE_UART4_RX_PINOUT;
    case UART5:
      return {GPIOC, GPIO12};
    case USART6:
      return CORE_UART6_RX_PINOUT;
    case UART7:
    case UART8:
      TODO(__FILE__, __LINE__, __func__);
      [[fallthrough]];
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid or unsupported UART Interface");
      return {};
  }
}

constexpr GPIO::AltFn UART::GetAltFn() const {
  switch (usart_) {
    case USART1:
    case USART2:
    case USART3:
      return GPIO_AF7;
    case UART4:
    case UART5:
    case USART6:
      return GPIO_AF8;
    case UART7:
    case UART8:
      TODO(__FILE__, __LINE__, __func__);
      [[fallthrough]];
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid or unsupported UART Interface");
      return GPIO_AF0;
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
    case USART6:
      nvic_ = NVICInterrupt(NVICInterrupt::kUSART6);
      break;
    case UART7:
    case UART8:
      nvic_ = NVICInterrupt(NVICInterrupt::Source(NVIC_UART7_IRQ + (usart_ - UART7) / 0x0400));
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

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)
