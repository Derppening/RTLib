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

#ifndef RTLIB_CORE_STM32F4_UART_H_
#define RTLIB_CORE_STM32F4_UART_H_

#if defined(STM32F4)

#include <cstdint>
#include <string>

#include <libopencm3/stm32/usart.h>

#include "core/util.h"
#include "core/stm32f4/gpio.h"

namespace rtlib::core::stm32f4 {

class UART {
 public:
  enum Interface {
    kUART1 = USART1,
    kUART2 = USART2,
    kUART3 = USART3,
    kUART4 = UART4,
    kUART5 = UART5,
    kUART6 = USART6,
    kUART7 = UART7,
    kUART8 = UART8
  };

  enum struct BaudRate {
    k2400 = 2400,
    k4800 = 4800,
    k9600 = 9600,
    k14400 = 14400,
    k19200 = 19200,
    k28800 = 28800,
    k38400 = 38400,
    k57600 = 57600,
    k76800 = 76800,
    k115200 = 115200,
    k230400 = 230400,
    k460800 = 460800,
  };

  enum struct Mode {
    kRx = USART_MODE_RX,
    kTx = USART_MODE_TX,
    kBoth = USART_MODE_TX_RX
  };

  enum struct Parity {
    kNone = USART_PARITY_NONE,
    kEven = USART_PARITY_EVEN,
    kOdd = USART_PARITY_ODD
  };

  enum struct StopBits {
    kOne = USART_CR2_STOPBITS_1,
    kHalf = USART_CR2_STOPBITS_0_5,
    kDouble = USART_CR2_STOPBITS_2,
    kOneHalf = USART_CR2_STOPBITS_1_5,
  };

  enum struct FlowControl {
    kNone = USART_FLOWCONTROL_NONE,
    kRTS = USART_FLOWCONTROL_RTS,
    kCTS = USART_FLOWCONTROL_CTS,
    kBoth = USART_FLOWCONTROL_RTS_CTS
  };

  struct Config {
    Interface uart;
    BaudRate baud_rate = BaudRate::k9600;
    uint32_t data_bits = 8;
    StopBits stop_bits = StopBits::kOne;
    Mode mode = Mode::kBoth;
    Parity parity = Parity::kNone;
    FlowControl flow_control = FlowControl::kNone;
  };

  explicit UART(const Config& config);

  UART(Interface uart,
       BaudRate baud_rate,
       Mode mode,
       uint32_t
       data_bits = 8,
       StopBits stop_bits = StopBits::kOne,
       Parity parity = Parity::kNone,
       FlowControl flow_control = FlowControl::kNone);

  void TxByte(char c);
  void Tx(const char* data, ...) __attribute__((format(printf, 2, 3)));

 private:
  void InitRcc() const;

  Pinout GetTxPinout() const;
  Pinout GetRxPinout() const;
  GPIO::AltFn GetAltFn() const;

  void EnableRxIrq() const;

  static constexpr const uint16_t kTxBufferSize = 256;

  uint32_t usart_;
  GPIO tx_;
  GPIO rx_;
};

}  // namespace rtlib::core::stm32f4

#endif  // defined(STM32F4)

#endif  // RTLIB_CORE_STM32F4_UART_H_
