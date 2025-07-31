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

#if NDEBUG
#if defined(__clang__)
#define INKNIT_ASSUME(expr, msg) __builtin_assume(expr)
#elif defined(__GNUC__)
#define INKNIT_ASSUME(expr, msg)         \
	if (!(expr)) __builtin_unreachable()
#elif defined(_MSC_VER)
#define INKNIT_ASSUME(expr, msg) __assume(expr)
#else
#define INKNIT_ASSUME(expr, msg) ((void)0)
#endif
#else
#include <assert.h>
#define INKNIT_ASSUME(expr, msg) assert((expr) && (void *)msg)
#endif

#if defined(_MSC_VER)
#define INKNIT_ALWAYS_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define INKNIT_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#define INKNIT_ALWAYS_INLINE
#endif

#ifndef INKNIT_BYTEORDER_ENDIAN
#if defined(__BYTE_ORDER__)  // GCC version from 4.6
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define INKNIT_BYTEORDER_LITTLE
#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define INKNIT_BYTEORDER_BIG
#else
#error "Unknown byteorder endianness detected. You should define INKNIT_BYTEORDER_ENDIAN."
#endif
#elif defined(__GLIBC__)
#include <endian.h>
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define INKNIT_BYTEORDER_ENDIAN BYTEORDER_LITTLE_ENDIAN
#elif (__BYTE_ORDER == __BIG_ENDIAN)
#define INKNIT_BYTEORDER_BIG
#else
#error "Unknown byteorder endianness detected. You should define INKNIT_BYTEORDER_ENDIAN."
#endif
#elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#define INKNIT_BYTEORDER_LITTLE
#elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
#define INKNIT_BYTEORDER_BIG
#elif defined(_M_IX86) || defined(_M_IA64)
#define INKNIT_BYTEORDER_LITTLE
#elif defined(_MSC_VER) && (defined(_M_ARM) || defined(_M_ARM64))
#define INKNIT_BYTEORDER_LITTLE
#else
#error "Unknown byteorder endianness detected. You should define INKNIT_BYTEORDER_ENDIAN."
#endif
#endif

#if defined(_MSC_VER)
#define INKNIT_BSWAP32(_x) _byteswap_ulong(_x)
#elif __has_builtin(__builtin_bswap32)
#define INKNIT_BSWAP32(_x) __builtin_bswap32(_x)
#else
#include <byteswap.h>
#define INKNIT_BSWAP32(_x) bswap_32(_x)
#endif

#if defined(__GNUC__) && __GNUC__ >= 7
#define INKNIT_FALLTHROUGH __attribute__((fallthrough))
#elif defined(__clang__) && __has_attribute(fallthrough)
#define INKNIT_FALLTHROUGH __attribute__((fallthrough))
#elif defined(_MSC_VER)
#ifndef __fallthrough
#define __fallthrough __pragma(warning(suppress : 26819))
#endif
#define INKNIT_FALLTHROUGH __fallthrough
#else
#define INKNIT_FALLTHROUGH \
	do {                   \
	} while (0)
#endif

#ifdef INKNIT_TARGET_IS_PICO
#define INKNIT_STRINGIFY(str) #str
#define INKNIT_ARRAY_IN_RAM(name, size)                                           \
	name[size] __attribute__((section(".time_critical." INKNIT_STRINGIFY(name))))
#else
#define INKNIT_ARRAY_IN_RAM(name, size) name[size]
#endif

#if defined(__ARM_ARCH_6M__) && !defined(INKNIT_TARGET_IS_PICO)
#define INKNIT_SLOW_MULTIPLY
#endif

#ifdef INKNIT_ENABLE_BENCHMARKS
#define INKNIT_BMPREFIX
#else
#define INKNIT_BMPREFIX static INKNIT_ALWAYS_INLINE
#endif

#define INKNIT_RADIUS_MAX (4095)  // 2^12-1
#define INKNIT_X_MAX      (4095)  // 2^12-1
#define INKNIT_Y_MAX      (4095)  // 2^12-1

// #if defined(__GNUC__) || defined(__clang__)
// #define SPLAT2D_PACKED_STRUCT struct __attribute__((__packed__))
// #define SPLAT2D_PACKED_STRUCT_END
// #elif defined(_MSC_VER)
// #define SPLAT2D_PACKED_STRUCT     __pragma(pack(push, 1)) struct
// #define SPLAT2D_PACKED_STRUCT_END __pragma(pack(pop))
// #elif __has_builtin(__builtin_bswap32)
// #else
// #define SPLAT2D_PACKED_STRUCT struct
// #define SPLAT2D_PACKED_STRUCT_END
// #endif
