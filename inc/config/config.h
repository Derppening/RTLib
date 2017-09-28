#ifndef RTLIB_CONFIG_CONFIG_H_
#define RTLIB_CONFIG_CONFIG_H_

#define EMPTY_MACRO_HELPER(VAL) VAL ## 1
#define EMPTY_MACRO(VAL) EMPTY_MACRO_HELPER(VAL)

#if defined(EXAMPLE_CONFIG)
#include "example_config.h"
#elif defined(STM32F103_DEV)
#include "config/stm32f103_dev.h"
#elif defined(STM32F407_DEV)
#include "config/stm32f407_dev.h"
#else
#error "No configuration is specified. Please specify a target mainboard in CMakeLists.txt"
#endif

#endif  // RTLIB_CONFIG_CONFIG_H_
