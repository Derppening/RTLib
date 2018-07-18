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
