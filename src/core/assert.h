/**
 * @file src/core/assert.h
 *
 * @brief Defines custom assertion functions.
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

#ifndef RTLIB_CORE_ASSERT_H_
#define RTLIB_CORE_ASSERT_H_

#include "util.h"

/**
 * @brief Generic assertion function.
 *
 * A custom, constant-expression assertion function to replace @c assert(condition).
 *
 * @param[in] bool_constexpr Expression to check against.
 * @param[in] file Source file which invoked this function. Always @c __FILE__.
 * @param[in] line Line in source file which invoked this function. Always @c __LINE__.
 * @param[in] fn Function which invoked this function. Always @c __func__.
 * @param[in] message Message if assertion fails.
 */
constexpr void Assert(const bool bool_constexpr,
                      [[maybe_unused]] const char* file,
                      [[maybe_unused]] const unsigned line,
                      [[maybe_unused]] const char* fn,
                      [[maybe_unused]] const char* message = "") noexcept {
  if (bool_constexpr) { return; }

  __builtin_trap();
}

/**
 * @brief Pin assertion function.
 *
 * Asserts whether a rtlib::core::Pinout is valid (i.e. not default constructed).
 *
 * @param[in] pinout Pinout to check
 * @param[in] file Source file which invoked this function. Always @c __FILE__.
 * @param[in] line Line in source file which invoked this function. Always @c __LINE__.
 * @param[in] fn Function which invoked this function. Always @c __func__.
 *
 * @return @p pinout if assertion succeeds.
 */
constexpr const rtlib::core::Pinout& AssertPin(const rtlib::core::Pinout& pinout,
                                               const char* file,
                                               const unsigned line,
                                               const char* fn) noexcept {
  Assert(pinout != rtlib::core::Pinout() && pinout != rtlib::core::kNullPinout, file, line, fn, "Invalid Pin");

  return pinout;
}

/**
 * @brief Runtime assertion for methods missing an implementation.
 *
 * @tparam FAIL_COMPILE Whether to fail during compile time.
 * @param[in] file Source file which invoked this function. Always @c __FILE__.
 * @param[in] line Line in source file which invoked this function. Always @c __LINE__.
 * @param[in] fn Function which invoked this function. Always @c __func__.
 * @param[in] message Message explaining the failure.
 */
template<bool FAIL_COMPILE = false>
constexpr void TODO([[maybe_unused]] const char* file,
                    [[maybe_unused]] const unsigned line,
                    [[maybe_unused]] const char* fn,
                    [[maybe_unused]] const char* message = "Stub") noexcept {
  static_assert(!FAIL_COMPILE, "Incomplete compile-time TODO");

  __builtin_trap();
}

#endif  // RTLIB_CORE_ASSERT_H_
