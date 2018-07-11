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

#ifndef RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_DRIVER_CDC_H_
#define RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_DRIVER_CDC_H_

#include "usbh_core.h"

BEGIN_DECLS

enum USBH_CDC_PARITY {
  USBH_CDC_PARITY_NONE,
  USBH_CDC_PARITY_ODD,
  USBH_CDC_PARITY_EVEN,
  USBH_CDC_PARITY_MARK,
  USBH_CDC_PARITY_SPACE,
};

enum USBH_CDC_DATABITS {
  USBH_CDC_DATABITS_5,
  USBH_CDC_DATABITS_6,
  USBH_CDC_DATABITS_7,
  USBH_CDC_DATABITS_8,
  USBH_CDC_DATABITS_9,
};

enum USBH_CDC_STOPBITS {
  USBH_CDC_STOPBITS_1,
  USBH_CDC_STOPBITS_1_5,
  USBH_CDC_STOPBITS_2,
};

enum CDC_STATE_IDLE_CYCLE {
  CDC_STATE_IDLE_CYCLE_GET_STATUS,
  CDC_STATE_IDLE_CYCLE_WRITE_BUFFER,
  CDC_STATE_IDLE_CYCLE_READ_BUFFER,
  CDC_STATE_IDLE_CYCLE_MAX,
};

#define CP210X_IFC_ENABLE   0x00
#define CP210X_SET_BAUDDIV  0x01
#define CP210X_GET_BAUDDIV  0x02
#define CP210X_SET_LINE_CTL 0x03
#define CP210X_GET_LINE_CTL 0x04
#define CP210X_SET_BREAK    0x05
#define CP210X_IMM_CHAR     0x06
#define CP210X_SET_MHS      0x07
#define CP210X_GET_MDMSTS   0x08
#define CP210X_SET_XON      0x09
#define CP210X_SET_XOFF     0x0A
#define CP210X_SET_EVENTMASK    0x0B
#define CP210X_GET_EVENTMASK    0x0C
#define CP210X_SET_CHAR     0x0D
#define CP210X_GET_CHARS    0x0E
#define CP210X_GET_PROPS    0x0F
#define CP210X_GET_COMM_STATUS  0x10
#define CP210X_RESET        0x11
#define CP210X_PURGE        0x12
#define CP210X_SET_FLOW     0x13
#define CP210X_GET_FLOW     0x14
#define CP210X_EMBED_EVENTS 0x15
#define CP210X_GET_EVENTSTATE   0x16
#define CP210X_SET_CHARS    0x19
#define CP210X_GET_BAUDRATE 0x1D
#define CP210X_SET_BAUDRATE 0x1E

/* CP210X_IFC_ENABLE */
#define UART_ENABLE     0x0001
#define UART_DISABLE        0x0000

/* CP210X_(SET|GET)_BAUDDIV */
#define BAUD_RATE_GEN_FREQ  0x384000

/* CP210X_(SET|GET)_LINE_CTL */
#define BITS_DATA_MASK      0X0f00
#define BITS_DATA_5     0X0500
#define BITS_DATA_6     0X0600
#define BITS_DATA_7     0X0700
#define BITS_DATA_8     0X0800
#define BITS_DATA_9     0X0900

#define BITS_PARITY_MASK    0x00f0
#define BITS_PARITY_NONE    0x0000
#define BITS_PARITY_ODD     0x0010
#define BITS_PARITY_EVEN    0x0020
#define BITS_PARITY_MARK    0x0030
#define BITS_PARITY_SPACE   0x0040

#define BITS_STOP_MASK      0x000f
#define BITS_STOP_1     0x0000
#define BITS_STOP_1_5       0x0001
#define BITS_STOP_2     0x0002

/* CP210X_SET_BREAK */
#define BREAK_ON        0x0001
#define BREAK_OFF       0x0000

/* CP210X_(SET_MHS|GET_MDMSTS) */
#define CONTROL_DTR     0x0001
#define CONTROL_RTS     0x0002
#define CONTROL_CTS     0x0010
#define CONTROL_DSR     0x0020

void cdc_driver_init(void);

typedef struct _usbh_dev_driver usbh_dev_driver_t;
extern const usbh_dev_driver_t usbh_cdc_driver;

END_DECLS

#endif  // RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_DRIVER_CDC_H_
