/**
 * @file src/core/util.h
 *
 * @brief Utility helpers for core library.
 */
#ifndef RTLIB_CORE_UTIL_H_
#define RTLIB_CORE_UTIL_H_

#include <cstdint>
#include <utility>

#if defined(STM32F1)
#define CORE_NS core::stm32f1
#elif defined(STM32F4)
#define CORE_NS core::stm32f4
#endif

/**
 * @brief Type definition for MCU Port.
 */
using Port = unsigned;
/**
 * @brief Type definition for MCU Pin.
 */
using Pin = uint16_t;
/**
 * @brief Type definition for MCU Pinout.
 */
using Pinout = std::pair<Port, Pin>;

#endif  // RTLIB_CORE_UTIL_H_
