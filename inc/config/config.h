#ifndef RTLIB_CONFIG_CONFIG_H_
#define RTLIB_CONFIG_CONFIG_H_

#if defined(EXAMPLE_CONFIG)
#include "example_config.h"
#elif defined(MAINBOARD_VER4_2)
#include "mainboard_ver4_2.h"
#elif defined(STM32F103_DEV)
#include "config/stm32f103_dev.h"
#elif defined(STM32F407_DEV)
#include "config/stm32f407_dev.h"
#else
#error "No configuration is specified. Please specify a target mainboard in CMakeLists.txt"
#endif

#endif  // RTLIB_CONFIG_CONFIG_H_
