/**
  ******************************************************************************
  * @file    usbh_adk_core.h
  * @author  Yuuichi Akagawa
  * @version V1.0.0
  * @date    2012/01/22
  * @brief   This file contains all the prototypes for the usbh_adk_core.c
  ******************************************************************************
  * @attention
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *      http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  * <h2><center>&copy; COPYRIGHT (C)2012 Yuuichi Akagawa</center></h2>
  *
  ******************************************************************************
  */
/*
 * This file is part of RTLib, ported from the STM32F4_ADK library
 * (https://github.com/YuuichiAkagawa/STM32F4_ADK). The original license text
 * is attached directly above this license.
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

#ifndef RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_ADK_CORE_H_
#define RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_ADK_CORE_H_

#include <stdint.h>

#include "usbh_core.h"

BEGIN_DECLS

//AOA 1.0
#define USB_ACCESSORY_VENDOR_ID         0x18D1
#define USB_ACCESSORY_PRODUCT_ID        0x2D00
#define USB_ACCESSORY_ADB_PRODUCT_ID    0x2D01
//AOA 2.0
#define USB_AUDIO_PRODUCT_ID               0x2D02
#define USB_AUDIO_ADB_PRODUCT_ID           0x2D03
#define USB_ACCESSORY_AUDIO_PRODUCT_ID     0x2D04
#define USB_ACCESSORY_AUDIO_ADB_PRODUCT_ID 0x2D05

#define ACCESSORY_STRING_MANUFACTURER   0
#define ACCESSORY_STRING_MODEL          1
#define ACCESSORY_STRING_DESCRIPTION    2
#define ACCESSORY_STRING_VERSION        3
#define ACCESSORY_STRING_URI            4
#define ACCESSORY_STRING_SERIAL         5

//AOA 1.0
#define ACCESSORY_GET_PROTOCOL          51
#define ACCESSORY_SEND_STRING           52
#define ACCESSORY_START                 53

//AOA 2.0
#define ACCESSORY_REGISTER_HID          54
#define ACCESSORY_UNREGISTER_HID        55
#define ACCESSORY_SET_HID_REPORT_DESC   56
#define ACCESSORY_SEND_HID_EVENT        57
#define ACCESSORY_SET_AUDIO_MODE        58

#define USBH_ADK_DATA_SIZE  64
#define USBH_ADK_NAK_RETRY_LIMIT 1

enum ADK_INIT_STATE {
  ADK_INIT_STATE_SETUP = 0,
  ADK_INIT_STATE_GET_PROTOCOL,
  ADK_INIT_STATE_SEND_MANUFACTURER,
  ADK_INIT_STATE_SEND_MODEL,
  ADK_INIT_STATE_SEND_DESCRIPTION,
  ADK_INIT_STATE_SEND_VERSION,
  ADK_INIT_STATE_SEND_URI,
  ADK_INIT_STATE_SEND_SERIAL,
  ADK_INIT_STATE_SWITCHING,
  ADK_INIT_STATE_GET_DEVDESC,
  ADK_INIT_STATE_CONFIGURE_ANDROID,
  ADK_INIT_STATE_DONE,
  ADK_INIT_STATE_FAILED,
};

enum ADK_STATE {
  ADK_STATE_IDLE = 0,
  ADK_STATE_SEND_DATA,
  ADK_STATE_BUSY,
  ADK_STATE_GET_DATA,
  ADK_STATE_WAIT_INIT,
  ADK_STATE_INITIALIZING,
  ADK_STATE_ERROR,
};

void adk_driver_init(uint8_t* manufacture, uint8_t* model, uint8_t* description, uint8_t* version, uint8_t* uri, uint8_t* serial);
void adk_write(const uint8_t* buff, uint16_t len);
uint16_t adk_read(uint8_t* buff, uint16_t len);
enum ADK_STATE adk_get_state(void);

typedef struct _usbh_dev_driver usbh_dev_driver_t;
extern const usbh_dev_driver_t usbh_adk_driver;
extern const usbh_dev_driver_t usbh_adb_adk_driver;
extern const usbh_dev_driver_t usbh_adk_generic_driver;

END_DECLS

#endif  // RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_ADK_CORE_H_
