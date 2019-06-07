#pragma once

#include <cstdint>
#include <limits>

namespace rtlib::core {
/**
 * @brief Loads data from the specified address.
 *
 * @tparam T Length of data to retrieve. Must be an integral type.
 * @param base_addr Address to load from.
 * @return Data from the specified address, represented as T.
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
[[nodiscard]]inline constexpr const volatile T get_mem(const std::uint32_t base_addr) {
  return *reinterpret_cast<volatile T* const>(base_addr);
}

/**
 * @brief Stores data into the registered address.
 *
 * @tparam T Length of data to retrieve. Must be an integral type.
 * @param base_addr Address to store into.
 * @param val Data to store.
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr void set_mem(const std::uint32_t base_addr, const T val) {
  *reinterpret_cast<volatile T* const>(base_addr) = val;
}

/**
 * @brief Obtains a range of bits from an integer.
 *
 * @note Behavior is undefined if @c pos or @c pos + len is greater than number of bits in @c T.
 *
 * @tparam T Type of data to read.
 * @param val Original value.
 * @param pos Start index of bit to read. LSB is index 0.
 * @param len Length of bitmask.
 * @return Integer representing bits @c [pos,pos+len).
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
[[nodiscard]] inline constexpr T get_bitmask(const T val, const int pos, const int len) {
  const T bitmask = len >= std::numeric_limits<T>::digits ? ~T(0) : (T(1) << len) - 1;

  return (val >> pos) & bitmask;
}

/**
 * @brief Manipulates a range of bits within an integer.
 *
 * Effectively resets the bits @c [pos,pos+len), and sets the bits according to @c new_val.
 *
 * @note Behavior is undefined if @c pos or @c pos + len is greater than number of bits in @c T.
 *
 * @tparam T Type of data to manipulate.
 * @param val Original value.
 * @param pos Start index of bit to manipulate. LSB is index 0.
 * @param len Length of bitmask.
 * @param new_val Value to replace in bit range.
 * @return @c val with @c new_val inserted into @c [pos,pos+len).
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
[[nodiscard]] inline constexpr T reset_and_set(T val, const int pos, const int len, const T new_val) {
  const T bitmask = len >= std::numeric_limits<T>::digits ? ~T(0) : (T(1) << len) - 1;

  val &= ~(bitmask << pos);
  val |= ((new_val & bitmask) << pos);
  return val;
}

/**
 * @brief Directly manipulates a range of bits for a memory location.
 *
 * Same as `reset_and_set`, but also retrieves and save the value from/to memory.
 *
 * @note Behavior is undefined if @c pos or @c pos + len is greater than number of bits in @c T.
 *
 * @tparam T Type of data to manipulate.
 * @param base_addr Base address of value to manipulate.
 * @param pos Start index of bit to manipulate. LSB is index 0.
 * @param len Length of bitmask.
 * @param new_val Value to replace in bit range.
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr void clear_and_set_mem(const std::uint32_t base_addr, const int pos, const int len, const T new_val) {
  auto v = get_mem<T>(base_addr);
  v = reset_and_set(v, pos, len, new_val);
  set_mem<T>(base_addr, v);
}

/**
 * @brief Obtains a range of bits from a memory location.
 *
 * Same as `get_bitmask`, but also retrieves the value from memory.
 *
 * @note Behavior is undefined if @c pos or @c pos + len is greater than number of bits in @c T.
 *
 * @tparam T Type of data to read.
 * @param base_addr Base address of value to read.
 * @param pos Start index of bit to read. LSB is index 0.
 * @param len Length of bitmask.
 * @return Integer representing bits @c [pos,pos+len).
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
[[nodiscard]] inline constexpr T get_mem_bitmask(const std::uint32_t base_addr, const int pos, const int len) {
  auto v = get_mem<T>(base_addr);
  return get_bitmask<T>(v, pos, len);
}
}  // namespace rtlib::core
