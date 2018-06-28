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
rtlib::core::stm32f1::UART::HandlerFn rx_handler = nullptr;

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
    usart_(uint32_t(interface)) {
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
    default:
      Assert(false, __FILE__, __LINE__, __func__, "Invalid or unsupported UART Interface");
      break;
  }
}

constexpr Pinout UART::GetTxPinout() const {
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

constexpr Pinout UART::GetRxPinout() const {
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

constexpr void UART::EnableIrq() const {
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
    default:
      // Target UART cannot be used synchronously. Ignore enable IRQ request.
      break;
  }
}

}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)
