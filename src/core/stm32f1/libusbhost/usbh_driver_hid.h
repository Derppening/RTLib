#ifndef RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_DRIVER_HID_H_
#define RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_DRIVER_HID_H_

#include "usbh_core.h"

#include <stdint.h>

BEGIN_DECLS

struct _hid_mouse_config {
  /**
   * @brief this is called when some data is read when polling the device
   * @param device_id handle of HID device
   * @param data pointer to the data
   * @param length count of bytes in the data
   *
   * TODO: make better interface that provides data contained in the report descriptor
   *
   */
  void (* hid_in_message_handler)(uint8_t device_id, const uint8_t* data, uint32_t length);
};
typedef struct _hid_mouse_config hid_config_t;

/**
 * @brief hid_mouse_driver_init initialization routine - this will initialize internal structures of this device driver
 * @param config
 * @see hid_mouse_config_t
 */
void hid_driver_init(const hid_config_t* config);

/**
 * @brief hid_set_report
 * @param device_id handle of HID device
 * @returns true on success, false otherwise
 */
bool hid_set_report(uint8_t device_id, uint8_t val);

enum HID_TYPE {
  HID_TYPE_NONE,
  HID_TYPE_MOUSE,
  HID_TYPE_KEYBOARD,
};

/**
 * @brief hid_get_type
 * @param device_id handle of HID device
 * @return type of attached HID
 * @see enum HID_TYPE
 */
enum HID_TYPE hid_get_type(uint8_t device_id);

/**
 * @brief hid_is_connected
 * @param device_id handle of HID device
 * @return true if the device with device_id is connected
 */
bool hid_is_connected(uint8_t device_id);

extern const usbh_dev_driver_t usbh_hid_driver;

END_DECLS

#endif  // RTLIB_CORE_STM32F1_LIBUSBHOST_USBH_DRIVER_HID_H_
