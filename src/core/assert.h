/**
 * @file src/core/assert.h
 *
 * @brief Defines custom assertion functions.
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

#pragma once

#include "util.h"

namespace rtlib::core {

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
constexpr void assert_that(const bool bool_constexpr,
                           [[maybe_unused]] const char* file,
                           [[maybe_unused]] const unsigned line,
                           [[maybe_unused]] const char* fn,
                           [[maybe_unused]] const char* message = "") noexcept {
  if (bool_constexpr) { return; }

  __builtin_trap();
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
constexpr void todo([[maybe_unused]] const char* file,
                    [[maybe_unused]] const unsigned line,
                    [[maybe_unused]] const char* fn,
                    [[maybe_unused]] const char* message = "Stub") noexcept {
  static_assert(!FAIL_COMPILE, "Incomplete compile-time TODO");

  __builtin_trap();
}
}  // namespace rtlib::core
