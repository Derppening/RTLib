/**
 * @file src/core/gpio.h
 *
 * @brief Helper file for selecting which GPIO class to enable.
 *
 * This file selects which GPIO class to enable according to the @c DEVICE set in @c CMakeLists.txt.
 */

#ifndef RTLIB_CORE_GPIO_H_
#define RTLIB_CORE_GPIO_H_

#include "core/util.h"

#if defined(STM32F1)
#include "core/stm32f1/gpio.h"
#elif defined(STM32F4)
#include "core/stm32f4/gpio.h"
#endif

#endif  // RTLIB_CORE_GPIO_H_
