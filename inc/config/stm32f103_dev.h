#ifndef RTLIB_CONFIG_STM32F103_DEV_H_
#define RTLIB_CONFIG_STM32F103_DEV_H_

#if !EMPTY_MACRO(STM32F103VCT6)
#error "This configuration is designed for a STM32F103VCT6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !EMPTY_MACRO(STM32F103VCT6)

#define LIB_USE_LED 1
#define LIB_LED1_PINOUT {GPIOB, GPIO0}

#endif  // RTLIB_CONFIG_STM32F103_DEV_H_
