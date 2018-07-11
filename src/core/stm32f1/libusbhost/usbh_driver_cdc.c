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

#include "usbh_driver_cdc.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "usbh_core.h"
#include "usbh_device_driver.h"
#include "usart_helper.h"
#include "mini_endian.h"

static bool initialized = false;

enum CDC_STATE {
  CDC_STATE_INITIAL,
  CDC_STATE_CONFIGURE_DEFAULTS,
  CDC_STATE_CONFIGURE,
  CDC_STATE_IDLE,
  CDC_STATE_GET_MODEM_STATUS,
};

enum CDC_CONFIG_STATE {
  CDC_CONFIG_STATE_ENABLE_INTERFACE,
  CDC_CONFIG_STATE_INITIAL,
  CDC_CONFIG_STATE_SET_BAUD,
  CDC_CONFIG_STATE_SET_MHS,
};

//moove
#define USBH_CDC_BUFFER 256
#define USBH_CDC_MAX_DEVICES 1

struct _cdc_device {
  unsigned int device_id;

  uint8_t ep_in_address;
  uint8_t ep_in_toggle;
  uint16_t ep_in_maxpacketsize;

  uint8_t ep_out_address;
  uint8_t ep_out_toggle;
  uint16_t ep_out_maxpacketsize;

  usbh_device_t* usbh_device;
  enum CDC_STATE state;
  enum CDC_CONFIG_STATE config_state;
  bool done;
  struct {
    enum USBH_CDC_DATABITS databits;
    enum USBH_CDC_PARITY parity;
    enum USBH_CDC_STOPBITS stopbits;
    uint32_t baud;
  } line;

  uint8_t buffer[32];
  enum CDC_STATE_IDLE_CYCLE cycle;

  bool complete;
};
typedef struct _cdc_device cdc_device_t;

static cdc_device_t cdc_device[1];

void cdc_driver_init(void) {
  initialized = true;
  int i;
  for (i = 0; i < USBH_CDC_MAX_DEVICES; i++) {
    cdc_device[i].state = CDC_STATE_INITIAL;
    cdc_device[i].cycle = CDC_STATE_IDLE_CYCLE_GET_STATUS;
  }
}

static void* init(usbh_device_t* usbh_dev) {
  if (!initialized) {
    ToUART("WARN : usbh_driver_cdc::init() - driver not initialized\r\n");
    return 0;
  }
  uint32_t i;
  cdc_device_t* drvdata = NULL;

  // find free data space for midi device
  for (i = 0; i < USBH_CDC_MAX_DEVICES; i++) {
    if (cdc_device[i].state == CDC_STATE_INITIAL) {
      drvdata = &cdc_device[i];
      drvdata->device_id = i;
      drvdata->ep_in_address = 0;
      drvdata->ep_out_address = 0;
      drvdata->ep_in_toggle = 0;
      drvdata->ep_out_toggle = 0;
      drvdata->usbh_device = usbh_dev;
      break;
    }
  }

  return drvdata;
}

static bool analyze_descriptor(void* drvdata, void* descriptor) {
  cdc_device_t* cdc = (cdc_device_t*) drvdata;
  uint8_t desc_type = ((uint8_t*) descriptor)[1];
  switch (desc_type) {
    case USB_DT_CONFIGURATION: {
      const struct usb_config_descriptor* cfg = (const struct usb_config_descriptor*) descriptor;
      (void) cfg;
    }
      break;

    case USB_DT_DEVICE: {
      const struct usb_device_descriptor* devDesc = (const struct usb_device_descriptor*) descriptor;
      (void) devDesc;
    }
      break;

    case USB_DT_INTERFACE: {
      const struct usb_interface_descriptor* ifDesc = (const struct usb_interface_descriptor*) descriptor;
      (void) ifDesc;
    }
      break;

    case USB_DT_ENDPOINT: {
      const struct usb_endpoint_descriptor* ep = (const struct usb_endpoint_descriptor*) descriptor;
      if ((ep->bmAttributes & 0x03) == USB_ENDPOINT_ATTR_BULK) {
        uint8_t epaddr = ep->bEndpointAddress;
        if (epaddr & (1 << 7)) {
          cdc->ep_in_address = epaddr & 0x7f;
          cdc->ep_in_maxpacketsize = ep->wMaxPacketSize;
        } else {
          cdc->ep_out_address = epaddr;
          cdc->ep_out_maxpacketsize = ep->wMaxPacketSize;
        }
      }
    }
      break;

    default:
      break;
  }

  if (cdc->ep_in_address && cdc->ep_out_address) {
    cdc->state = CDC_STATE_CONFIGURE_DEFAULTS;
    cdc->complete = true;
    return true;
  }

  return false;
}

static void configure_event(usbh_device_t* dev, usbh_packet_callback_data_t cb_data) {
  cdc_device_t* cdc = (cdc_device_t*) dev->drvdata;

  if (cdc->state != CDC_STATE_CONFIGURE) {
    ToUART("WARN : usbh_driver_cdc::configure_event() - configure callback while not in config state\r\n");
    return;
  }

  switch (cdc->config_state) {
    case CDC_CONFIG_STATE_SET_BAUD:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_driver_cdc::configure_event() - Error setting baud\r\n");
        break;
      } else {
        struct usb_setup_data setup_data;
        setup_data.bmRequestType = USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_INTERFACE;
        setup_data.bRequest = CP210X_SET_MHS;
        setup_data.wValue = htobe16(0x0303); // Hardcoded!
        setup_data.wIndex = 0;
        setup_data.wLength = 0;

        cdc->config_state = CDC_CONFIG_STATE_SET_MHS;
        device_control(dev, configure_event, &setup_data, NULL);
      }
      break;

    case CDC_CONFIG_STATE_SET_MHS:
      if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("ERROR: usbh_driver_cdc::configure_event() - setting MHS\r\n");
        break;
      } else {
        cdc->state = CDC_STATE_IDLE;
      }
      break;

    default:
      ToUART("ERROR: usbh_driver_cdc::configure_event() - Invalid state %d\r\n", cdc->config_state);
      break;
  }

}

static void configure(cdc_device_t* cdc) {
  struct usb_setup_data setup_data;
  setup_data.bmRequestType = USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_INTERFACE;
  setup_data.bRequest = CP210X_SET_BAUDRATE;
  setup_data.wValue = 0;
  setup_data.wIndex = 0;
  setup_data.wLength = 4;

  uint32_t baud = htole32(cdc->line.baud);
  memcpy(cdc->buffer, &baud, sizeof(baud));

  cdc->state = CDC_STATE_CONFIGURE;
  cdc->config_state = CDC_CONFIG_STATE_SET_BAUD;

  device_control(cdc->usbh_device, configure_event, &setup_data, cdc->buffer);
}

static void configure_defaults_event(usbh_device_t* dev, usbh_packet_callback_data_t cb_data) {
  cdc_device_t* cdc = (cdc_device_t*) dev->drvdata;
  if (cdc->state != CDC_STATE_CONFIGURE) {
    ToUART("WARN : usbh_driver_cdc::configure_defaults_event() - configure defaults callback while not in config state\r\n");
    return;
  }

  switch (cdc->config_state) {
    case CDC_CONFIG_STATE_ENABLE_INTERFACE:
      if (cb_data.status == USBH_PACKET_CALLBACK_STATUS_OK) {
        ToUART("INFO : usbh_driver_cdc::configure_defaults_event() - Uart initialized - defaults:\r\n");
        cdc->line.baud = 115200;
        cdc->line.databits = USBH_CDC_DATABITS_8;
        cdc->line.parity = USBH_CDC_PARITY_NONE;
        cdc->line.stopbits = USBH_CDC_STOPBITS_1;
        configure(cdc);
      } else {
        ToUART("ERROR: : usbh_driver_cdc::configure_defaults_event() - error enabling interface\r\n");
      }
      break;

    default:
      break;
  }
}

static void configure_defaults(cdc_device_t* cdc) {
  struct usb_setup_data setup_data;
  setup_data.bmRequestType = USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_INTERFACE;
  setup_data.bRequest = CP210X_IFC_ENABLE;
  setup_data.wValue = UART_ENABLE;
  setup_data.wIndex = 0;
  setup_data.wLength = 0;

  cdc->config_state = CDC_CONFIG_STATE_ENABLE_INTERFACE;

  cdc->state = CDC_STATE_CONFIGURE;
  device_control(cdc->usbh_device, configure_defaults_event, &setup_data, NULL);
}

static void get_status_event(usbh_device_t* dev, usbh_packet_callback_data_t cb_data) {
  cdc_device_t* cdc = (cdc_device_t*) dev->drvdata;
  if (cdc->state != CDC_STATE_GET_MODEM_STATUS) {
    ToUART("WARN : usbh_driver_cdc::get_status_event() -get status callback while not in config state\r\n");
    return;
  }
  if (cb_data.status != USBH_PACKET_CALLBACK_STATUS_OK) {
    ToUART("ERROR: usbh_driver_cdc::get_status_event() - Error getting status\r\n");
    return;
  }
  ToUART("INFO : usbh_driver_cdc::get_status_event() - Status: %02x\r\n", cdc->buffer[0]);
  cdc->state = CDC_STATE_IDLE;
}

static void write_buffer_event(usbh_device_t* dev, usbh_packet_callback_data_t cb_data) {
  cdc_device_t* cdc = (cdc_device_t*) dev->drvdata;
  if (cb_data.status == USBH_PACKET_CALLBACK_STATUS_OK) {
    ToUART("INFO : usbh_driver_cdc::write_buffer_event() - WRITE OK\r\n");
    cdc->complete = true;
  } else {
    // do something
  }
}

static void write_buffer(cdc_device_t* cdc) {
  memcpy(cdc->buffer, "AHOJ", 4);

  usbh_packet_t packet;
  packet.address = cdc->usbh_device->address;
  packet.data.out = cdc->buffer;
  packet.datalen = 4;
  packet.endpoint_address = cdc->ep_out_address;
  packet.endpoint_size_max = cdc->ep_out_maxpacketsize;
  packet.endpoint_type = USBH_ENDPOINT_TYPE_BULK;
  packet.callback = write_buffer_event;
  packet.callback_arg = cdc->usbh_device;
  packet.speed = cdc->usbh_device->speed;
  packet.toggle = &cdc->ep_out_toggle;

  usbh_write(cdc->usbh_device, &packet);
}

static void process_idle(cdc_device_t* cdc) {
  struct usb_setup_data setup_data;
  switch (cdc->cycle) {
    case CDC_STATE_IDLE_CYCLE_GET_STATUS:
      setup_data.bmRequestType = USB_REQ_TYPE_IN | USB_REQ_TYPE_VENDOR | USB_REQ_TYPE_INTERFACE;
      setup_data.bRequest = CP210X_GET_MDMSTS;
      setup_data.wValue = 0;
      setup_data.wIndex = 0;
      setup_data.wLength = 1;
      cdc->state = CDC_STATE_GET_MODEM_STATUS;
      cdc->cycle++;
      device_control(cdc->usbh_device, get_status_event, &setup_data, &cdc->buffer[0]);
      break;

    case CDC_STATE_IDLE_CYCLE_WRITE_BUFFER:
      cdc->state = CDC_STATE_IDLE;
      if (cdc->complete) {
        cdc->complete = false;
        write_buffer(cdc);
      }
      break;

    case CDC_STATE_IDLE_CYCLE_READ_BUFFER:
      cdc->state = CDC_STATE_IDLE;
//		if (cdc->complete) {
//			cdc->complete = false;
//			read_buffer(cdc);
//		}
      break;

    default:
      cdc->cycle = CDC_STATE_GET_MODEM_STATUS;
      break;
  }
}

static void poll(void* drvdata, uint32_t time_curr_us) {
  (void) time_curr_us;
  cdc_device_t* cdc = (cdc_device_t*) drvdata;

  switch (cdc->state) {
    case CDC_STATE_CONFIGURE_DEFAULTS:
      configure_defaults(cdc);
      break;

    case CDC_STATE_IDLE:
      process_idle(cdc);
      break;

    default:
      break;
  }

  return;
}

static void remove(void* drvdata) {
  cdc_device_t* cdc = (cdc_device_t*) drvdata;
  cdc->state = CDC_STATE_INITIAL;
}

static const usbh_dev_driver_info_t driver_info = {
    .deviceClass = 0,
    .deviceSubClass = 0,
    .deviceProtocol = 0,
    .idVendor = 0x10c4,
    .idProduct = 0xea60,
    .ifaceClass = 0xff, // Vendor specific class
    .ifaceSubClass = 0,
    .ifaceProtocol = 0,
};

const usbh_dev_driver_t usbh_cdc_driver = {
    .init = init,
    .analyze_descriptor = analyze_descriptor,
    .poll = poll,
    .remove = remove,
    .info = &driver_info
};
