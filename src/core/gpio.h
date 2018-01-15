#ifndef RTLIB_CORE_GPIO_H_
#define RTLIB_CORE_GPIO_H_

#include "core/util.h"

#if defined(STM32F1)
#include "core/stm32f1/gpio.h"
#elif defined(STM32F4)
#include "core/stm32f4/gpio.h"
#endif

#endif  // RTLIB_CORE_GPIO_H_
