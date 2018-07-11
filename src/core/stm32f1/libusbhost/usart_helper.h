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

#ifndef RTLIB_CORE_STM32F1_LIBUSBHOST_USART_HELPER_H_
#define RTLIB_CORE_STM32F1_LIBUSBHOST_USART_HELPER_H_

#include <stdarg.h>

#include "config/config.h"

/**
 * @brief Directly pipes a message to an UART interface.
 *
 * The UART interface to use should be defined by @c CORE_LIBUSBHOST_USART_DEBUG in your board configuration file.
 *
 * The internal buffer is guaranteed to have a capacity of 1024 characters.
 *
 * @param fmt Format string.
 * @param ... Arguments for the format string.
 */
void ToUART(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

#endif  // RTLIB_CORE_STM32F1_LIBUSBHOST_USART_HELPER_H_
