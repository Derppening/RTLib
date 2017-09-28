#ifndef RTLIB_CONFIG_STM32F407_DEV_H_
#define RTLIB_CONFIG_STM32F407_DEV_H_

#if !EMPTY_MACRO(STM32F407VET6)
#error "This configuration is designed for a STM32F407VET6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !EMPTY_MACRO(STM32F407VET6)

#define LIB_USE_LED 2
#define LIB_LED1_PINOUT {GPIOA, GPIO6}
#define LIB_LED2_PINOUT {GPIOA, GPIO7}

#endif  // RTLIB_CONFIG_STM32F407_DEV_H_
