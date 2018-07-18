/**
  ******************************************************************************
  * @file    usbh_adk_core.c
  * @author  Yuuichi Akagawa
  * @version V1.0.0
  * @date    2012/03/05
  * @brief   Android Open Accessory implementation
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

#include "usbh_adk_core.h"

#include <stdint.h>
#include <string.h>

#include "usbh_core.h"
#include "usbh_device_driver.h"
#include "usart_helper.h"

#define USBH_ADK_MAX_DEVICES 1

static bool initialized = false;

struct _adk_process {
  uint16_t pid;
  uint8_t BulkOutEp;
  uint8_t BulkInEp;
  uint16_t BulkInEpSize;
  uint16_t BulkOutEpSize;
  uint8_t inbuff[USBH_ADK_DATA_SIZE];
  uint8_t outbuff[USBH_ADK_DATA_SIZE];
  uint16_t inSize;
  uint16_t outSize;
  enum ADK_INIT_STATE initstate;
  enum ADK_STATE state;
  uint8_t acc_manufacturer[64];
  uint8_t acc_model[64];
  uint8_t acc_description[64];
  uint8_t acc_version[64];
  uint8_t acc_uri[64];
  uint8_t acc_serial[64];
  uint16_t protocol;

  usbh_device_t* usbh_device;
  uint8_t ep_in_toggle;
  uint8_t ep_out_toggle;
};
typedef struct _adk_process adk_machine_t;

static struct _adk_process adk_machine[1];

static void send_string(adk_machine_t* adk, uint16_t index, uint8_t* buff);

void adk_driver_init(uint8_t* manufacture,
                     uint8_t* model,
                     uint8_t* description,
                     uint8_t* version,
                     uint8_t* uri,
                     uint8_t* serial) {
  initialized = true;

  int i;
  for (i = 0; i < USBH_ADK_MAX_DEVICES; ++i) {
    strncpy(adk_machine[i].acc_manufacturer, manufacture, 64);
    adk_machine->acc_manufacturer[63] = '\0';
    strncpy(adk_machine[i].acc_model, model, 64);
    adk_machine[i].acc_model[63] = '\0';
    strncpy(adk_machine[i].acc_description, description, 64);
    adk_machine[i].acc_description[63] = '\0';
    strncpy(adk_machine[i].acc_version, version, 64);
    adk_machine[i].acc_version[63] = '\0';
    strncpy(adk_machine[i].acc_uri, uri, 64);
    adk_machine[i].acc_uri[63] = '\0';
    strncpy(adk_machine[i].acc_serial, serial, 64);
    adk_machine[i].acc_serial[63] = '\0';

    adk_machine[i].initstate = ADK_INIT_STATE_SETUP;
    adk_machine[i].state = ADK_STATE_WAIT_INIT;
  }
}

static void* init(usbh_device_t* usbh_dev) {
  if (!initialized) {
    ToUART("WARN : usbh_driver_cdc::init() - driver not initialized\r\n");
    return 0;
  }
  uint32_t i;
  adk_machine_t* drvdata = NULL;

  // find free data space
  for (i = 0; i < USBH_ADK_MAX_DEVICES; i++) {
    drvdata = &adk_machine[i];
    adk_machine->BulkInEp = 0;
    adk_machine->BulkOutEp = 0;
    adk_machine->inSize = 0;
    adk_machine->outSize = 0;
    adk_machine->ep_in_toggle = 0;
    adk_machine->ep_out_toggle = 0;
    adk_machine->usbh_device = usbh_dev;
    break;
  }

  return drvdata;
}

static void remove(void* drvdata) {
  ToUART("INFO : usbh_adk_core::remove()\r\n");

  adk_machine_t* adk = (adk_machine_t*) drvdata;

  if (adk->BulkInEp) {
    adk->BulkInEp = 0;
  }

  if (adk->BulkOutEp) {
    adk->BulkOutEp = 0;
  }

  adk->initstate = ADK_INIT_STATE_SETUP;
  adk->state = ADK_STATE_WAIT_INIT;
}

static void send_data(usbh_device_t* dev, usbh_packet_callback_data_t cb_data) {
  adk_machine_t* adk = (adk_machine_t*) dev->drvdata;

  if (adk->state != ADK_STATE_INITIALIZING) {
    ToUART("WARN : usbh_driver_cdc::send_data() - send data while not in config state\r\n");
    return;
  }

  ToUART("INFO : send_data() - adk->initstate = %d\r\n", adk->initstate);

  switch (adk->initstate) {
    case ADK_INIT_STATE_SETUP:
      break;
    case ADK_INIT_STATE_GET_PROTOCOL:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_adk_core::send_data() - Error getting protocol\r\n");
      } else {
        if (adk->protocol >= 1) {
          adk->initstate = ADK_INIT_STATE_SEND_MANUFACTURER;

          ToUART("INFO : usbh_adk_core::send_data() - Device supports protocol 1\r\n");

          send_string(adk, ACCESSORY_STRING_MANUFACTURER, (uint8_t*) adk->acc_manufacturer);
        } else {
          adk->initstate = ADK_INIT_STATE_FAILED;

          ToUART("WARN : usbh_adk_core::send_data() - Could not read device protocol version\r\n");
        }
      }
      break;
    case ADK_INIT_STATE_SEND_MANUFACTURER:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_adk_core::send_data() - Error sending manufacturer\r\n");
      } else {
        ToUART("INFO : usbh_adk_core::send_data() - SEND_MANUFACTURER successful\r\n");

        adk->initstate = ADK_INIT_STATE_SEND_MODEL;

        send_string(adk, ACCESSORY_STRING_MODEL, (uint8_t*) adk->acc_model);
      }
      break;
    case ADK_INIT_STATE_SEND_MODEL:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_adk_core::send_data() - Error sending model\r\n");
      } else {
        ToUART("INFO : usbh_adk_core::send_data() - SEND_MODEL successful\r\n");

        adk->initstate = ADK_INIT_STATE_SEND_DESCRIPTION;

        send_string(adk, ACCESSORY_STRING_DESCRIPTION, (uint8_t*) adk->acc_description);
      }
      break;
    case ADK_INIT_STATE_SEND_DESCRIPTION:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_adk_core::send_data() - Error sending description\r\n");
      } else {
        ToUART("INFO : usbh_adk_core::send_data() - SEND_DESCRIPTION successful\r\n");

        adk->initstate = ADK_INIT_STATE_SEND_VERSION;

        send_string(adk, ACCESSORY_STRING_VERSION, (uint8_t*) adk->acc_version);
      }
      break;
    case ADK_INIT_STATE_SEND_VERSION:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_adk_core::send_data() - Error sending version\r\n");
      } else {
        ToUART("INFO : usbh_adk_core::send_data() - SEND_VERSION successful\r\n");

        adk->initstate = ADK_INIT_STATE_SEND_URI;

        send_string(adk, ACCESSORY_STRING_URI, (uint8_t*) adk->acc_uri);
      }
      break;
    case ADK_INIT_STATE_SEND_URI:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_adk_core::send_data() - Error sending URI\r\n");
      } else {
        ToUART("INFO : usbh_adk_core::send_data() - SEND_URI successful\r\n");

        adk->initstate = ADK_INIT_STATE_SEND_SERIAL;

        send_string(adk, ACCESSORY_STRING_SERIAL, (uint8_t*) adk->acc_serial);
      }
      break;
    case ADK_INIT_STATE_SEND_SERIAL:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_adk_core::send_data() - Error sending serial\r\n");
      } else {
        ToUART("INFO : usbh_adk_core::send_data() - SEND_SERIAL successful\r\n");

        adk->initstate = ADK_INIT_STATE_SWITCHING;

        struct usb_setup_data data;
        data.bmRequestType = USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_DEVICE;
        data.bRequest = ACCESSORY_START;
        data.wValue = 0;
        data.wIndex = 0;
        data.wLength = 0;

        device_control(adk->usbh_device, send_data, &data, NULL);
      }
      break;
    case ADK_INIT_STATE_SWITCHING:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_adk_core::send_data() - Error switching mode\r\n");
      } else {
        ToUART("INFO : usbh_adk_core::send_data() - Switched to accessory mode\r\n");

        adk->initstate = ADK_INIT_STATE_GET_DEVDESC;

      }
    case ADK_INIT_STATE_GET_DEVDESC:
      // TODO(Derppening): See if we need to do something here
//      break;
      // fallthrough
    case ADK_INIT_STATE_CONFIGURE_ANDROID:
//      analyze_descriptor(dev->drvdata, )
      // fallthrough
    case ADK_INIT_STATE_DONE:
      adk->state = ADK_STATE_IDLE;
      ToUART("INFO : usbh_adk_core::send_data() - Configuration complete\r\n");
      break;
    case ADK_INIT_STATE_FAILED:
      ToUART("FATAL: usbh_adk_core::send_data() - :(((\r\n");
      break;
    default:
      break;
  }
}

static void send_string(adk_machine_t* adk, uint16_t index, uint8_t* buff) {
  uint16_t length = (uint16_t) (strlen(buff) + 1);

  struct usb_setup_data data;
  data.bmRequestType = USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_DEVICE;
  data.bRequest = ACCESSORY_SEND_STRING;
  data.wValue = 0;
  data.wIndex = index;
  data.wLength = length;

  device_control(adk->usbh_device, send_data, &data, buff);
}

static bool analyze_descriptor(void* drvdata, void* descriptor) {
  adk_machine_t* adk = (adk_machine_t*) drvdata;
  uint8_t desc_type = ((uint8_t*) descriptor)[1];

  switch (desc_type) {
    case USB_DT_CONFIGURATION: {
      const struct usb_config_descriptor* cfg = (const struct usb_config_descriptor*) descriptor;
      (void) cfg;

      break;
    }
    case USB_DT_DEVICE: {
      const struct usb_device_descriptor* devDesc = (const struct usb_device_descriptor*) descriptor;

      adk->pid = devDesc->idProduct;
      break;
    }
    case USB_DT_ENDPOINT: {
      ToUART("INFO : usbh_adk_core::analyze_descriptor() - Configure bulk endpoint\r\n");

      const struct usb_endpoint_descriptor* ep = (const struct usb_endpoint_descriptor*) descriptor;

      ToUART("INFO : usbh_adk_core::analyze_descriptor() - packet length = %d\r\n", ep->bLength);
      ToUART("INFO : usbh_adk_core::analyze_descriptor() - packet = %x\r\n", ep[0]);

      if (ep->bEndpointAddress & 0x80) {
        adk->BulkInEp = ep->bEndpointAddress & 0x7f;
        adk->BulkInEpSize = ep->wMaxPacketSize;
      } else {
        adk->BulkOutEp = ep->bEndpointAddress;
        adk->BulkOutEpSize = ep->wMaxPacketSize;
      }

      break;
    }
    default:
      break;
  }

  ToUART("INFO : usbh_adk_core::analyze_descriptor() - %d %d\r\n", adk->BulkInEp, adk->BulkOutEp);

  if (adk->BulkInEp && adk->BulkOutEp) {
    adk->initstate = ADK_INIT_STATE_DONE;
    return true;
  }

  return false;
}

void adk_write(const uint8_t* buff, uint16_t len) {
  memcpy(adk_machine->outbuff, buff, len);
  adk_machine->outSize = len;
}

uint16_t adk_read(uint8_t* buff, uint16_t len) {
  uint16_t xfercount = adk_machine->inSize;
  if (xfercount > 0) {
    memcpy(buff, adk_machine->inbuff, len);
    adk_machine->inSize = 0;
  }

  return (uint16_t) xfercount;
}

static void write_buffer_event(usbh_device_t* dev, usbh_packet_callback_data_t cb_data) {
  (void) dev;

  if (cb_data.status == USBH_PACKET_CALLBACK_STATUS_OK) {
    ToUART("INFO : usbh_adk_core::write_buffer_event() - Write OK\r\n");
  } else {
    ToUART("WARN : usbh_adk_core::write_buffer_event() - Write Failed (%d)\r\n", cb_data.status);
  }
}

static void read_buffer_event(usbh_device_t* dev, usbh_packet_callback_data_t cb_data) {
  adk_machine_t* adk = (adk_machine_t*) dev->drvdata;

  if (cb_data.status == USBH_PACKET_CALLBACK_STATUS_OK) {
    adk->inSize = (uint16_t) cb_data.transferred_length;

    ToUART("INFO : usbh_adk_core::read_buffer_event() - Read OK\r\n");
  } else {
    ToUART("WARN : usbh_adk_core::read_buffer_event() - Read Failed (%d)\r\n", cb_data.status);
  }
}

static void write_adk_in_endpoint(adk_machine_t* adk) {
  ToUART("INFO : write_adk_in_endpoint() - %d\r\n", adk->ep_out_toggle);

  usbh_packet_t packet;
  packet.address = adk->usbh_device->address;
  packet.data.out = adk->outbuff;
  packet.datalen = adk->outSize;
  packet.endpoint_address = adk->BulkOutEp;
  packet.endpoint_size_max = adk->BulkOutEpSize;
  packet.endpoint_type = USBH_ENDPOINT_TYPE_BULK;
  packet.callback = write_buffer_event;
  packet.callback_arg = adk->usbh_device;
  packet.speed = adk->usbh_device->speed;
  packet.toggle = &adk->ep_out_toggle;

  usbh_write(adk->usbh_device, &packet);

  adk->state = ADK_STATE_GET_DATA;
  adk->outSize = 0;
}

static void read_adk_in_endpoint(adk_machine_t* adk) {
  ToUART("INFO : read_adk_in_endpoint() - %d\r\n", adk->ep_in_toggle);

  usbh_packet_t packet;
  packet.address = adk->usbh_device->address;
  packet.data.in = &adk->inbuff[0];
  packet.endpoint_address = adk->BulkInEp;
  packet.endpoint_size_max = adk->BulkInEpSize;
  packet.endpoint_type = USBH_ENDPOINT_TYPE_BULK;
  packet.callback = read_buffer_event;
  packet.callback_arg = adk->usbh_device;
  packet.speed = adk->usbh_device->speed;
  packet.toggle = &adk->ep_in_toggle;

  usbh_read(adk->usbh_device, &packet);
}

enum ADK_STATE adk_get_state(void) {
  return adk_machine->state;
}

static void poll(void* drvdata, uint32_t time_curr_us) {
  (void) time_curr_us;

  adk_machine_t* adk = (adk_machine_t*) drvdata;

  switch (adk->state) {
    case ADK_STATE_IDLE:
      adk->state = ADK_STATE_SEND_DATA;
      // fallthrough
    case ADK_STATE_SEND_DATA:
      if (adk->outSize > 0) {
        write_adk_in_endpoint(adk);
      }
      break;
    case ADK_STATE_GET_DATA:
      read_adk_in_endpoint(adk);
      adk->state = ADK_STATE_IDLE;
      break;
    case ADK_STATE_BUSY:
      adk->state = ADK_STATE_IDLE;
      adk->outSize = 0;
      break;
    case ADK_STATE_WAIT_INIT:
      ToUART("INFO : usbh_adk_core::poll() - initializing\r\n");

      if (adk->usbh_device->drv->info->idVendor == USB_ACCESSORY_VENDOR_ID
          && (adk->usbh_device->drv->info->idProduct == USB_ACCESSORY_PRODUCT_ID
              || adk->usbh_device->drv->info->idProduct == USB_ACCESSORY_ADB_PRODUCT_ID)) {
        adk->initstate = ADK_INIT_STATE_DONE;

        adk->state = ADK_STATE_IDLE;
        ToUART("INFO : usbh_adk_core::poll() - Configuration complete\r\n");
      } else {
        adk->initstate = ADK_INIT_STATE_GET_PROTOCOL;
        adk->protocol = -1;

        struct usb_setup_data data;
        data.bmRequestType = USB_REQ_TYPE_IN | USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_DEVICE;
        data.bRequest = ACCESSORY_GET_PROTOCOL;
        data.wValue = 0;
        data.wIndex = 0;
        data.wLength = 2;

        device_control(adk->usbh_device, send_data, &data, &adk_machine->protocol);

        adk->state = ADK_STATE_INITIALIZING;
      }

    default:
      break;
  }
}

static const usbh_dev_driver_info_t driver_info = {
    .deviceClass = -1,
    .deviceSubClass = -1,
    .deviceProtocol = -1,
    .idVendor = USB_ACCESSORY_VENDOR_ID,
    .idProduct = USB_ACCESSORY_PRODUCT_ID,
    .ifaceClass = -1,
    .ifaceSubClass = -1,
    .ifaceProtocol = -1
};

static const usbh_dev_driver_info_t adb_driver_info = {
    .deviceClass = -1,
    .deviceSubClass = -1,
    .deviceProtocol = -1,
    .idVendor = USB_ACCESSORY_VENDOR_ID,
    .idProduct = USB_ACCESSORY_ADB_PRODUCT_ID,
    .ifaceClass = -1,
    .ifaceSubClass = -1,
    .ifaceProtocol = -1
};

static const usbh_dev_driver_info_t adk_generic_driver_info = {
    .deviceClass = -1,
    .deviceSubClass = -1,
    .deviceProtocol = -1,
    .idVendor = -1,
    .idProduct = -1,
    .ifaceClass = -1,
    .ifaceSubClass = -1,
    .ifaceProtocol = -1
};

const usbh_dev_driver_t usbh_adk_driver = {
    .init = init,
    .analyze_descriptor = analyze_descriptor,
    .poll = poll,
    .remove = remove,
    .info = &driver_info
};

const usbh_dev_driver_t usbh_adb_adk_driver = {
    .init = init,
    .analyze_descriptor = analyze_descriptor,
    .poll = poll,
    .remove = remove,
    .info = &adb_driver_info
};

const usbh_dev_driver_t usbh_adk_generic_driver = {
    .init = init,
    .analyze_descriptor = analyze_descriptor,
    .poll = poll,
    .remove = remove,
    .info = &adk_generic_driver_info
};
