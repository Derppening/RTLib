/**
 * @file src/core/stm32/utils/rcc.h
 *
 * @brief Utility methods for STM32 RCC.
 */

/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2019 Derppening <david.18.19.21@gmail.com>
 *
 * RTLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RTLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RTLib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RTLIB_CORE_STM32_UTILS_RCC_
#define RTLIB_CORE_STM32_UTILS_RCC_

namespace rtlib::core::stm32::utils {
/**
 * @brief Returns the address of a given RCC register.
 *
 * @param base Base address.
 * @param bit Bit offset.
 * @return Address of RCC register.
 */
constexpr std::uint32_t rcc_get_reg(std::uint8_t base, std::uint8_t bit) {
  return std::uint32_t((base << 5) + bit);
}
}  // namespace rtlib::core::stm32::utils

#endif  // RTLIB_CORE_STM32_UTILS_RCC_
