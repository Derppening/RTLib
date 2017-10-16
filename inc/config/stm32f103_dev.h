#ifndef RTLIB_CONFIG_STM32F103_DEV_H_
#define RTLIB_CONFIG_STM32F103_DEV_H_

#if !defined(STM32F103VCT6)
#error "This configuration is designed for a STM32F103VCT6 device. (Did you set DEVICE in CMakeLists.txt correctly?)"
#endif  // !defined(STM32F103VCT6)

#define LIB_USE_LED 1
#define LIB_LED0_PINOUT {GPIOB, GPIO0}

#define LIB_USE_BUTTON 0

#endif  // RTLIB_CONFIG_STM32F103_DEV_H_
