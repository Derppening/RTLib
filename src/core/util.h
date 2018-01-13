#ifndef RTLIB_CORE_UTIL_H_
#define RTLIB_CORE_UTIL_H_

#include <cstdint>
#include <utility>

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
