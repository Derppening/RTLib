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

/**
 * @brief Custom assertion function.
 *
 * A custom, constant-expression assertion function to replace @c assert(condition).
 *
 * @param bool_constexpr Expression to check against.
 * @param file Source file which invoked this function. Always @c __FILE__.
 * @param line Line in source file which invoked this function. Always @c __LINE__
 * @param fn Function which invoked this function. Always @c __func__.
 * @param message Message if assertion fails.
 */
constexpr void Assert(const bool bool_constexpr,
                      const char* file,
                      const unsigned line,
                      const char* fn,
                      const char* message = "") noexcept {
  if (bool_constexpr) { return; }

  __builtin_trap();
}

#endif  // RTLIB_CORE_ASSERT_H_
