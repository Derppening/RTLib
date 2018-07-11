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

#include "usart_helper.h"

#include <stdio.h>
#include <libopencm3/stm32/usart.h>

void ToUART(const char* fmt, ...) {
#if defined(CORE_LIBUSBHOST_USART_DEBUG) && CORE_LIBUSBHOST_USART_DEBUG != 0
#define BUFFER_SIZE 1024

  char buf[BUFFER_SIZE];

  va_list va;
  va_start(va, fmt);
  vsnprintf(buf, BUFFER_SIZE, fmt, va);
  va_end(va);

  for (int i = 0; buf[i] != '\0'; ++i) {
    usart_send_blocking(CORE_LIBUSBHOST_USART_DEBUG, (uint16_t)buf[i]);
  }
#undef BUFFER_SIZE
#endif  // defined(CORE_LIBUSBHOST_USART_DEBUG) && CORE_LIBUSBHOST_USART_DEBUG != 0
}
