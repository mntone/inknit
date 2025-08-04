/*
 * inknit <https://github.com/mntone/inknit>
 *
 * Copyright (C) 2025 mntone <901816+mntone@users.noreply.github.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#if defined(__has_cpp_attribute)
// Check for standard C++17 [[nodiscard]]
#if __has_cpp_attribute(nodiscard) >= 201603L
#define INKNIT_NODISCARD [[nodiscard]]
#endif
#endif

#ifndef INKNIT_NODISCARD
#if defined(__GNUC__) || defined(__clang__)
#define INKNIT_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
#define INKNIT_NODISCARD _Check_return_
#else
#define INKNIT_NODISCARD /* NODISCARD not supported */
#endif
#endif

#define INKNIT_SUBCASE_INVOKE(invoke, subcase_name) \
	do {                                            \
		SUBCASE(subcase_name) {                     \
			(invoke);                               \
		}                                           \
	} while (false)

#if NDEBUG
#define INKNIT_SUBCASE_EXPECT_ASSERT(invoke, subcase_name, expected_message)
#else
#define INKNIT_SUBCASE_EXPECT_ASSERT(invoke, subcase_name, expected_message) \
	do {                                                                     \
		SUBCASE(subcase_name) {                                              \
			auto& instance = assert_helper::instance();                      \
			instance.reset();                                                \
                                                                             \
			(invoke);                                                        \
                                                                             \
			CHECK(instance.fired());                                         \
			CHECK_EQ(instance.message(), expected_message);                  \
		}                                                                    \
	} while (false)
#endif
