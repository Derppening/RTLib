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

#include <cassert>
#include <cstdarg>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>

#include "config/config.h"
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
    UART(config.uart,
         config.baud_rate,
         config.mode,
         config.data_bits,
         config.stop_bits,
         config.parity,
         config.flow_control,
         config.rx_handler_fn) {}

UART::UART(Interface interface,
           BaudRate baud_rate,
           Mode mode,
           uint32_t data_bits,
           StopBits stop_bits,
           Parity parity,
           FlowControl flow_control,
           HandlerFn handler) :
    usart_(uint32_t(interface)),
    tx_(GetTxPinout(),
        GPIO::Mode::kAF,
        GPIO::Pullup::kNone,
        GPIO::Speed::k50MHz,
        GPIO::DriverType::kPushPull,
        GetAltFn()),
    rx_(GetRxPinout(),
        GPIO::Mode::kAF,
        GPIO::Pullup::kNone,
        GPIO::Speed::k50MHz,
        GPIO::DriverType::kOpenDrain,
        GetAltFn()) {
  InitRcc();
  EnableIrq();

  usart_set_baudrate(usart_, uint32_t(baud_rate));
  usart_set_databits(usart_, data_bits);
  usart_set_stopbits(usart_, uint32_t(stop_bits));
  usart_set_mode(usart_, uint32_t(mode));
  usart_set_parity(usart_, uint32_t(parity));
  usart_set_flow_control(usart_, uint32_t(flow_control));

  if (mode != Mode::kTx) {
    usart_enable_rx_interrupt(usart_);
  }

  usart_enable(usart_);

  rx_handler = handler;
}

void UART::TxByte(char c) {
  usart_send(usart_, uint16_t(c));
}

void UART::Tx(const char* format, ...) {
  va_list args;
  char buffer[kTxBufferSize];
  char* ptr = buffer;

  va_start(args, format);
  std::vsprintf(buffer, format, args);
  va_end(args);

  usart_enable_tx_interrupt(usart_);

  while (*ptr != '\0') {
    usart_send(usart_, uint16_t(*ptr));
    ++ptr;
  }

  usart_disable_tx_interrupt(usart_);
}

void UART::InitRcc() const {
  switch (usart_) {
    case USART1:
      rcc_periph_clock_enable(RCC_USART1);
      break;
    case USART2:
      rcc_periph_clock_enable(RCC_USART2);
      break;
    case USART3:
      rcc_periph_clock_enable(RCC_USART3);
      break;
    case UART4:
      rcc_periph_clock_enable(RCC_UART4);
      break;
    case UART5:
      rcc_periph_clock_enable(RCC_UART5);
      break;
    case USART6:
      rcc_periph_clock_enable(RCC_USART6);
      break;
    case UART7:
      rcc_periph_clock_enable(RCC_UART7);
      break;
    case UART8:
      rcc_periph_clock_enable(RCC_UART8);
      break;
    default:
      assert(false);
      break;
  }
}

Pinout UART::GetTxPinout() const {
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
      // TODO(Derppening): Fill in UART7/8 Pins
//      return GPIO_AF0;
    default:
      assert(false);
      break;
  }
}

Pinout UART::GetRxPinout() const {
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
      // TODO(Derppening): Fill in UART7/8 Pins
//      return GPIO_AF0;
    default:
      assert(false);
      break;
  }
}

GPIO::AltFn UART::GetAltFn() const {
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
      // TODO(Derppening): Fill in UART7/8 AltFn
//      return GPIO_AF0;
    default:
      assert(false);
      break;
  }
}

void UART::EnableIrq() const {
  switch (usart_) {
    case USART1:
      nvic_enable_irq(NVIC_USART1_IRQ);
      break;
    case USART2:
      nvic_enable_irq(NVIC_USART2_IRQ);
      break;
    case USART3:
      nvic_enable_irq(NVIC_USART3_IRQ);
      break;
    case USART6:
      nvic_enable_irq(NVIC_USART6_IRQ);
      break;
    default:
      // Target UART cannot be used synchronously. Ignore enable IRQ request.
      break;
  }
}

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)
