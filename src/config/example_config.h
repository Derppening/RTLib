/**
 * @file src/config/example_config.h
 *
 * @brief Example configuration.
 *
 * This configuration file is an example on how to create your own mainboard configurations.
 */

#ifndef RTLIB_CONFIG_EXAMPLE_CONFIG_H_
#define RTLIB_CONFIG_EXAMPLE_CONFIG_H_

// Use !defined(...) to check if the appropriate device is targeted before proceeding
// Replace ... in the brackets to the MCU model of your target device.
#if !defined(STM32F103VCT6)
#error "This configuration is designed for a STM32F103VCT6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F103VCT6)

// You must define all LIB_USE_*. If your board does not have the feature, or you do not intend to expose the API to
// other users, set the macro to 0. A compilation error will occur if a user tries to include a library which is
// disabled in this file.
#define LIB_USE_BUTTON 0
#define LIB_USE_LED 1

// To use wrapper classes for any devices under lib, define all pin outputs here.
// The format of the macro is
// #define LIB_DEVICEx_PINOUT {GPIO, GPIO_PIN}
// where DEVICE is the device under lib, x is the ID of the device, GPIO is the GPIO, and GPIO_PIN is the pin.
// All devices IDs in this file start from 0, and corresponds to 0 in Config structs.
#define LIB_LED0_PINOUT {GPIOB, GPIO0}

// Other devices may require more than one pinout. These will also be defined here.
// TODO(Derppening): Add example from UART

#endif  // RTLIB_CONFIG_EXAMPLE_CONFIG_H_
