#pragma once

namespace rtlib::core {
/**
 * @brief Loads data from the specified address.
 *
 * @tparam T Length of data to retrieve. Must be an integral type.
 * @param base_addr Address to load from.
 * @return Data from the specified address, represented as T.
 */
template<typename T = std::uint32_t, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr volatile T const get_mem(const std::uint32_t base_addr) {
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
}  // namespace rtlib::core
