/*
 * This file is part of RTLib, ported from the libusbhost library
 * (https://github.com/libusbhost/libusbhost).
 *
 * Copyright (C) 2015 Amir Hammad <amir.hammad@hotmail.com>
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

#ifndef RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_LLD_H_
#define RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_LLD_H_

//#include <cstdint>

#include "usbh_core.h"

//extern const USBHLowLevelDriver usbh_lld_driver_fs;

BEGIN_DECLS

// pass this to usbh init
extern const usbh_low_level_driver_t usbh_lld_stm32f1_driver_fs;

#ifdef USART_DEBUG
void print_channels(const void *drvdata);
#else
#define print_channels(arg) ((void)arg)
#endif

END_DECLS

#endif  // RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_LLD_H_
