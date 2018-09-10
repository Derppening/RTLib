/**
 * @file src/util/ranges.h
 *
 * @brief Defines range-related functions.
 */

/*
 * This file is part of RTLib.
 *
 * Copyright (C) 2018 Derppening <david.18.19.21@gmail.com>
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

#ifndef RTLIB_UTIL_RANGES_H_
#define RTLIB_UTIL_RANGES_H_

#include <functional>

namespace rtlib::util {
/**
 * @brief Checks whether a value is in range.
 *
 * @tparam T
 * @tparam Compare Comparator. Should return true if arg0 < arg1.
 * @param[in] v Value to check.
 * @param[in] lo Lower bound of clamp.
 * @param[in] hi Upper bound of clamp.
 * @param[in] comp Comparator function.
 * @return Whether v is between lo and hi (i.e. lo <= v <= hi).
 */
template<class T, class Compare>
constexpr bool in_range(const T& v, const T& lo, const T& hi, Compare comp) {
  return Assert(!comp(hi, lo), __FILE__, __LINE__, __func__), comp(v, lo) ? false : comp(hi, v) ? false : true;
}

/**
 * @brief Returns whether a value is between two values (inclusive).
 *
 * @tparam T
 * @param[in] v Value to check.
 * @param[in] lo Lower bound of clamp.
 * @param[in] hi Upper bound of clamp.
 * @return Whether v is inclusively between lo and hi (i.e. lo <= v <= hi).
 */
template<class T>
constexpr bool in_range(const T& v, const T& lo, const T& hi) {
  return in_range(v, lo, hi, std::less<>());
}
}  // namespace rtlib::util

#endif  // RTLIB_UTIL_RANGES_H_
