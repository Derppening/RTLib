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
#include <limits>

#include <libopencm3/stm32/usart.h>

#include "core/util.h"
#include "core/stm32f1/gpio.h"

namespace rtlib::core::stm32f1 {

class UART {
 public:
  /**
   * @brief Enumeration for different UART interfaces.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f4/html/group__usart__reg__base.html
   */
  enum Interface {
    /**
     * @brief Representation of an invalid UART interface.
     */
      kNullUART = -1,
    /**
     * @brief Corresponds to hardware USART1.
     */
        kUART1 = USART1,
    /**
     * @brief Corresponds to hardware USART2.
     */
        kUART2 = USART2,
    /**
     * @brief Corresponds to hardware USART3.
     */
        kUART3 = USART3,
    /**
     * @brief Corresponds to hardware UART4.
     */
        kUART4 = UART4,
    /**
     * @brief Corresponds to hardware UART5.
     */
        kUART5 = UART5
  };

  /**
   * @brief Enumeration for standard baud rates.
   */
  enum struct BaudRate {
    /**
     * @brief Transmit at 2400Hz.
     */
        k2400 = 2400,
    /**
     * @brief Transmit at 4800Hz.
     */
        k4800 = 4800,
    /**
     * @brief Transmit at 9600Hz.
     */
        k9600 = 9600,
    /**
     * @brief Transmit at 14400Hz.
     */
        k14400 = 14400,
    /**
     * @brief Transmit at 19200Hz.
     */
        k19200 = 19200,
    /**
     * @brief Transmit at 28800Hz.
     */
        k28800 = 28800,
    /**
     * @brief Transmit at 38400Hz.
     */
        k38400 = 38400,
    /**
     * @brief Transmit at 57600Hz.
     */
        k57600 = 57600,
    /**
     * @brief Transmit at 76800Hz.
     */
        k76800 = 76800,
    /**
     * @brief Transmit at 115200Hz.
     */
        k115200 = 115200,
    /**
     * @brief Transmit at 230400Hz.
     */
        k230400 = 230400,
    /**
     * @brief Transmit at 468000Hz.
     */
        k460800 = 460800,
  };

  /**
   * @brief Enumeration for different UART modes.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__usart__cr1__mode.html.
   */
  enum struct Mode {
    /**
     * @brief Use this UART only for RX.
     *
     * Equivalent to libopencm3 macro @c USART_MODE_RX.
     */
        kRx = USART_MODE_RX,
    /**
     * @brief Use this UART only for TX.
     *
     * Equivalent to libopencm3 macro @c USART_MODE_TX.
     */
        kTx = USART_MODE_TX,
    /**
     * @brief Use this UART for both TX and RX.
     *
     * Equivalent to libopencm3 macro @c USART_MODE_TX_RX.
     */
        kBoth = USART_MODE_TX_RX
  };

  /**
   * @brief Enumeration for different UART parity selection.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__usart__cr1__parity.html.
   */
  enum struct Parity {
    /**
     * @brief Do not use any parity.
     *
     * Equivalent to libopencm3 macro @c USART_PARITY_NONE.
     */
        kNone = USART_PARITY_NONE,
    /**
     * @brief Use even parity.
     *
     * Parity bit will be set/reset such that the data frame has even number of set bits.
     *
     * Equivalent to libopencm3 macro @c USART_PARITY_EVEN.
     */
        kEven = USART_PARITY_EVEN,
    /**
     * @brief Use odd parity.
     *
     * Parity bit will be set/reset such that the data frame has odd number of set bits.
     *
     * Equivalent to libopencm3 macro @c USART_PARITY_ODD.
     */
        kOdd = USART_PARITY_ODD
  };

  /**
   * @brief Enumeration for different UART stop bit configurations.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__usart__cr2__stopbits.html.
   */
  enum struct StopBits {
    /**
     * @brief Use 1 stop bit.
     *
     * Equivalent to libopencm3 macro @c USART_CR2_STOPBITS_1.
     */
        kOne = USART_CR2_STOPBITS_1,
    /**
     * @brief Use 0.5 stop bit.
     *
     * This is used when receiving data in Smartcard mode.
     *
     * Equivalent to libopencm3 macro @c USART_CR2_STOPBITS_0.5.
     */
        kHalf = USART_CR2_STOPBITS_0_5,
    /**
     * @brief Use 2 stop bits.
     *
     * Equivalent to libopencm3 macro @c USART_CR2_STOPBITS_2.
     */
        kDouble = USART_CR2_STOPBITS_2,
    /**
     * @brief Use 1.5 stop bits.
     *
     * This is used when transmitting and receiving data in Smartcard mode.
     *
     * Equivalent to libopencm3 macro @c USART_CR2_STOPBITS_1_5.
     */
        kOneHalf = USART_CR2_STOPBITS_1_5,
  };

  /**
   * @brief Enumeration for different flow control configurations.
   *
   * This enum is intended to replace equivalent macros used in libopencm3.
   *
   * See http://libopencm3.org/docs/latest/stm32f1/html/group__usart__cr3__flowcontrol.html.
   */
  enum struct FlowControl {
    /**
     * @brief Do not use any flow control.
     *
     * Equivalent to libopencm3 macro @c USART_FLOWCONTROL_NONE.
     */
        kNone = USART_FLOWCONTROL_NONE,
    /**
     * @brief Use RTS flow control.
     *
     * When enabled, RTS output will be tied low as long as the UART receiver is ready to receive a new data.
     *
     * Equivalent to libopencm3 macro @c USART_FLOWCONTROL_RTS.
     */
        kRTS = USART_FLOWCONTROL_RTS,
    /**
     * @brief Use CTS flow control.
     *
     * When enabled, data is only transmitted when CTS input is tiered low.
     *
     * Equivalent to libopencm3 macro @c USART_FLOWCONTROL_CTS.
     */
        kCTS = USART_FLOWCONTROL_CTS,
    /**
     * @brief Enables both RTS and CTS flow control.
     *
     * See FlowControl::kRTS and FlowControl::kCTS for their description.
     *
     * Equivalent to libopencm3 macro @c USART_FLOWCONTROL_RTS_CTS.
     */
        kBoth = USART_FLOWCONTROL_RTS_CTS
  };

  /**
   * @brief Type definition for RX interrupt handler function.
   */
  using HandlerFn = void (*)(const Interface, const uint16_t);

  /**
   * @brief Configuration for UART.
   */
  struct Config {
    /**
     * @brief UART interface to enable and initialize.
     *
     * Defaults to an indeterminate state.
     */
    Interface uart;
    /**
     * @brief UART Baud Rate.
     *
     * Defaults to BaudRate#k9600 (9600Hz).
     */
    BaudRate baud_rate = BaudRate::k9600;
    /**
     * @brief Data bits per word (8 or 9).
     *
     * See http://libopencm3.org/docs/latest/stm32f1/html/group__usart__defines.html#ga8333883729d5f460bf02c1f24bd3c37d.
     *
     * Defaults to 8-bits.
     */
    uint32_t data_bits = 8;
    /**
     * @brief Number of stop bits.
     *
     * Defaults to StopBits#kOne (1 stop bit).
     */
    StopBits stop_bits = StopBits::kOne;
    /**
     * @brief UART mode(s) to enable.
     *
     * Defaults to Mode#kBoth (Use both TX and RX).
     */
    Mode mode = Mode::kBoth;
    /**
     * @brief UART Parity Mode.
     *
     * Defaults to Parity#kNone (Disable parity).
     */
    Parity parity = Parity::kNone;
    /**
     * @brief UART Flow Control.
     *
     * Defaults to FlowControl#kNone (Disable flow control).
     */
    FlowControl flow_control = FlowControl::kNone;
    /**
     * @brief RX Interrupt Handler function.
     *
     * RX interrupts from all UART interfaces will be redirected to this function.
     * This function must have the prototype equivalent to HandlerFn.
     *
     * Defaults to @c nullptr (RX interrupts not handled).
     */
    HandlerFn rx_handler_fn = nullptr;
  };

  /**
   * @brief Default constructor.
   *
   * Initializes this object to an "unbinded" state, i.e. this object does not manage any UART.
   */
  constexpr UART() : usart_(kNullUART) {}

  /**
   * @brief Conversion constructor.
   *
   * @param[in] config Configuration for the UART. See UART#Config.
   */
  explicit UART(const Config& config);

  /**
   * @brief Destructor.
   */
  ~UART();

  /**
   * @brief Move constructor.
   *
   * @param[in] other UART object to move from
   */
  constexpr UART(UART&& other) noexcept :
      usart_(other.usart_), tx_(std::move(other.tx_)), rx_(std::move(other.rx_)) {
    other.usart_ = kNullUART;
  }
  /**
   * @brief Move assignment operator.
   *
   * @param[in] other UART object to move from
   *
   * @return Reference to the moved UART.
   */
  constexpr UART& operator=(UART&& other) noexcept {
    if (this != &other) {
      usart_ = other.usart_;
      tx_ = std::move(other.tx_);
      rx_ = std::move(other.rx_);

      other.usart_ = kNullUART;
    }

    return *this;
  }

  /**
   * @brief Copy constructor.
   *
   * This constructor is deleted because there should only be one object managing each UART interface, similar to @c
   * std::unique_ptr.
   */
  UART(const UART&) = delete;
  /**
   * @brief Copy assignment operator.
   *
   * This constructor is deleted because there should only be one object managing each UART interface, similar to @c
   * std::unique_ptr.
   */
  UART& operator=(const UART&) = delete;

  /**
   * @return Whether this object is managing a UART interface.
   */
  constexpr bool IsBinded() const { return usart_ != kNullUART; }

  /**
   * @brief Releases the ownership of the UART from this object.
   *
   * This function implicitly invokes GPIO#Reset, and releases the ownership of the currently managed UART interface and
   * GPIO pinouts.
   *
   * This function is null-safe, i.e. if this object is not bound to any UART, there are no effects.
   */
  void Release();

  /**
   * @brief Sends one byte.
   *
   * @param[in] c Character to send
   */
  void Tx(char c) const;
  /**
   * @brief Sends a character string.
   *
   * Sends a c-string for @p len characters, or until a null-terminating character is reached.
   *
   * @note Previously, this function accepts a format string and variable arguments (similar to @c std::printf). To
   * migrate to this version, use @c std::snprinf to format your string, then pass the c-string into this function.
   *
   * @param[in] str Character string to send
   * @param[in] len Length of string to send
   */
  void Tx(const char* str, std::size_t len = std::numeric_limits<std::size_t>::max()) const;

 private:
  /**
   * @brief Initializes an RCC Clock according to which port is initialized.
   *
   * This function is a simple helper function to determine which RCC to initialize, given the UART we are working with.
   *
   * @param port The UART interface which should be initialized
   */
  constexpr void InitRcc() const;
  /**
   * @brief Enables interrupt for this UART interface (if supported).
   */
  constexpr void EnableIrq() const;

  /**
   * @brief Resets the UART to its original configuration, i.e. when the RESET button is first pushed.
   *
   * No changes will be made to the GPIO pins which are bound to this object.
   *
   * This function is null-safe, i.e. if this object is not bound to any pinout, there are no effects.
   */
  void Reset() const;

  /**
   * @brief Retrieves the TX pinout from the configuration file.
   *
   * @return TX pinout
   */
  constexpr Pinout GetTxPinout() const;
  /**
   * @brief Retrieves the RX pinout from the configuration file.
   *
   * @return RX pinout
   */
  constexpr Pinout GetRxPinout() const;

  /**
   * @brief C-String buffer size for transmission.
   */
  static constexpr const uint16_t kTxBufferSize = 128;

  uint32_t usart_;
  GPIO tx_;
  GPIO rx_;
};

}  // namespace rtlib::core::stm32f1

#endif  // defined(STM32F1)

#endif  // RTLIB_CORE_STM32F1_UART_H_
