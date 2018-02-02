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

#ifndef RTLIB_CORE_STM32F1_UART_H_
#define RTLIB_CORE_STM32F1_UART_H_

#if defined(STM32F1)

#include <cstdint>

#include <libopencm3/stm32/usart.h>

namespace rtlib::core::stm32f1 {

class UART {
 public:
  enum Interface {
    kUART1,
    kUART2,
    kUART3,
    kUART4,
    kUART5,
    kUART6,
    kUART7,
    kUART8
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

  };

 private:

};

}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)

#endif  // RTLIB_CORE_STM32F1_UART_H_
