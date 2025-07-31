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

#if defined(__GNUC__) || defined(__clang__)
#define INKBM_ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define INKBM_ALWAYS_INLINE __forceinline
#else
#define INKBM_ALWAYS_INLINE
#endif

#ifdef __cplusplus
#define INKBM_DECLS_BEGIN extern "C" {
#define INKBM_DECLS_END   }
#else
#define INKBM_DECLS_BEGIN
#define INKBM_DECLS_END
#endif

#if __cplusplus >= 201703L
#define INKBM_MAYBE_UNUSED [[maybe_unused]]
#elif defined(__GNUC__) || defined(__clang__)
#define INKBM_MAYBE_UNUSED __attribute__((unused))
#else
#define INKBM_MAYBE_UNUSED
#endif

#if defined(__has_cpp_attribute)
// Check for standard C++17 [[nodiscard]]
#if __has_cpp_attribute(nodiscard) >= 201603L
#define INKBM_NODISCARD [[nodiscard]]
#endif
#endif
#ifndef INKBM_NODISCARD
#if defined(__GNUC__) || defined(__clang__)
#define INKBM_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
#define INKBM_NODISCARD _Check_return_
#else
#define INKBM_NODISCARD /* NODISCARD not supported */
#endif
#endif

#ifdef __cplusplus
#if defined(_MSC_VER) && _MSC_VER < 1900
#define INKBM_NOEXCEPT /* NOEXCEPT not supported */
#elif __cplusplus < 201103L
#define INKBM_NOEXCEPT /* NOEXCEPT not supported */
#else
#define INKBM_NOEXCEPT noexcept
#endif
#else
#define INKBM_NOEXCEPT /* NOEXCEPT not supported */
#endif

#if __cplusplus >= 201103L
// Check for standard C++11 [[noreturn]]
#define INKBM_NORETURN [[noreturn]]
#else
#if defined(__GNUC__) || defined(__clang__)
#define INKBM_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define INKBM_NORETURN __declspec(noreturn)
#else
#define INKBM_NORETURN /* NORETURN not supported */
#pragma warning "NORETURN is not defined for this compiler in C++03 mode"
#endif
#endif

#if __cplusplus >= 201103L
#define INKBM_THREAD_LOCAL thread_local
#elif defined(__GNUC__) || defined(__clang__)
#define INKBM_THREAD_LOCAL __thread
#elif defined(_MSC_VER)
#define INKBM_THREAD_LOCAL __declspec(thread)
#else
#pragma warning "Thread-local storage not supported on this compiler"
#define INKBM_THREAD_LOCAL /* THREAD_LOCAL */
#endif

#if defined(_MSC_VER)
#define INKBM_UNREACHABLE __assume(0)
#elif defined(__GNUC__) || defined(__clang__)
#define INKBM_UNREACHABLE __builtin_unreachable()
#else
#define INKBM_UNREACHABLE ((void)0)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define INKBM_WEAK __attribute__((weak))
#else
#define INKBM_WEAK
#endif

#define INKBM_HEADER         "[benchmark] "
#define INKBM_STRINGIFY(str) #str

INKBM_DECLS_BEGIN

INKBM_NORETURN
void inkbm_fatal_handler(char const *format, ...) INKBM_NOEXCEPT INKBM_WEAK;
void inkbm_error_handler(int level, char const *format, ...) INKBM_NOEXCEPT INKBM_WEAK;

INKBM_DECLS_END

#define INKBM_FATAL(...)              \
	inkbm_fatal_handler(__VA_ARGS__); \
	INKBM_UNREACHABLE
#define INKBM_WARN(...) inkbm_error_handler(0, __VA_ARGS__);


#ifdef __cplusplus
#if defined(_MSC_VER)
#include <intrin.h>  // _ReadWriteBarrier
#endif

namespace inkbm {

template<class T>
INKBM_ALWAYS_INLINE void do_not_optimize_away(T const& value) {
#if defined(__GNUC__) || defined(__clang__)
	asm volatile("" : : "r,m"(value) : "memory");
#elif defined(_MSC_VER)
	(void)static_cast<const volatile void *>(&value);
	_ReadWriteBarrier();
#else
	(void)static_cast<const volatile void *>(&value);
#endif
}

}  // namespace inkbm
#endif
