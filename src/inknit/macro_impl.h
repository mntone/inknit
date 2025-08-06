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
#define INKNIT_ASSERT(expr, msg)
#else
#define INKNIT_ASSERT(expr, msg)                                            \
	if (!(expr)) _inknit_assert((msg), #expr, __FILE__, __func__, __LINE__)
#endif

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
#define INKNIT_ASSUME(expr, msg) INKNIT_ASSERT(expr, msg)
#endif

#if defined(_MSC_VER)
#define INKNIT_ALWAYS_INLINE __forceinline
#define INKNIT_NOINLINE      __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define INKNIT_ALWAYS_INLINE inline __attribute__((always_inline))
#define INKNIT_NOINLINE      __attribute__((noinline))
#else
#define INKNIT_ALWAYS_INLINE
#define INKNIT_NOINLINE
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

#define INKNIT_STRINGIFY(str) #str
#define INKNIT_TOSTRING(x)    INKNIT_STRINGIFY(x)

#ifdef INKNIT_TARGET_IS_PICO
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

#define INKNIT_INTERNAL_WRAPFUNC(name) _INKNIT_CONCAT2(__wrap, name)


// -- MARK: internal constants
#define INKNIT_CIRCLE_MAX  8191   // 2^13 - 1
#define INKNIT_ELLIPSE_MAX 1023   // 2^10 - 1
#define INKNIT_X_MIN       -4096  // -2^12
#define INKNIT_X_MAX       4095   // 2^12 - 1
#define INKNIT_Y_MIN       -4096  // -2^12
#define INKNIT_Y_MAX       4095   // 2^12 - 1
#define INKNIT_WIDTH_MAX   4095   // 2^12 - 1
#define INKNIT_HEIGHT_MAX  4095   // 2^12 - 1


// -- MARK: assume macros
#include "message.h"

#define INKNIT_ASSUME_RANGE(var, min, max, msg_low, msg_high) \
	do {                                                      \
		INKNIT_ASSUME(min <= (var), msg_low);                 \
		INKNIT_ASSUME((var) <= max, msg_high);                \
	} while (0)
#define INKNIT_ASSUME_BOUNDED_RANGE(min, max, min_limit, max_limit, msg_low, msg_order, msg_high) \
	do {                                                                                          \
		INKNIT_ASSUME(min_limit <= (min), msg_low);                                               \
		INKNIT_ASSUME((min) <= (max), msg_order);                                                 \
		INKNIT_ASSUME((max) <= max_limit, msg_high);                                              \
	} while (0)

#define _INKNIT_ASSUME_IMAGE_BASE(image, _layout, _align, _width, _height)                     \
	do {                                                                                       \
		INKNIT_ASSUME((image) != NULL, #image MSG_ERROR_IMAGE);                                \
		INKNIT_ASSUME(                                                                         \
			(image)->pixel_layout == _layout,                                                  \
			#image MSG_ERROR_IMAGE_LAYOUT INKNIT_TOSTRING(_layout)                             \
		);                                                                                     \
		INKNIT_ASSUME(                                                                         \
			(image)->alignment >= _align, #image MSG_ERROR_IMAGE_ALIGN INKNIT_TOSTRING(_align) \
		);                                                                                     \
		INKNIT_ASSUME((image)->width <= _width, #image MSG_ERROR_IMAGE_WIDTH);                 \
		INKNIT_ASSUME((image)->height <= _height, #image MSG_ERROR_IMAGE_HEIGHT);              \
	} while (0)
#define INKNIT_ASSUME_IMAGE(image)                                                  \
	_INKNIT_ASSUME_IMAGE_BASE(                                                      \
		image, TARGET_LAYOUT, TARGET_ALIGNMENT, INKNIT_WIDTH_MAX, INKNIT_HEIGHT_MAX \
	)

#define INKNIT_ASSUME_COORD_X(var_x)                                                               \
	INKNIT_ASSUME_RANGE(                                                                           \
		var_x, INKNIT_X_MIN, INKNIT_X_MAX, MSG_X_LESS_THAN_MIN_VALUE, MSG_X_GREATER_THAN_MAX_VALUE \
	)
#define INKNIT_ASSUME_COORD_Y(var_y)                                                               \
	INKNIT_ASSUME_RANGE(                                                                           \
		var_y, INKNIT_Y_MIN, INKNIT_Y_MAX, MSG_Y_LESS_THAN_MIN_VALUE, MSG_Y_GREATER_THAN_MAX_VALUE \
	)

#define INKNIT_ASSUME_CENTER_X(var_cx) \
	INKNIT_ASSUME_RANGE(               \
		var_cx,                        \
		INKNIT_X_MIN,                  \
		INKNIT_X_MAX,                  \
		MSG_CX_LESS_THAN_MIN_VALUE,    \
		MSG_CX_GREATER_THAN_MAX_VALUE  \
	)
#define INKNIT_ASSUME_CENTER_Y(var_cy) \
	INKNIT_ASSUME_RANGE(               \
		var_cy,                        \
		INKNIT_Y_MIN,                  \
		INKNIT_Y_MAX,                  \
		MSG_CY_LESS_THAN_MIN_VALUE,    \
		MSG_CY_GREATER_THAN_MAX_VALUE  \
	)

#define INKNIT_ASSUME_COORD_X_RANGE(var_x1, var_x2) \
	INKNIT_ASSUME_BOUNDED_RANGE(                    \
		var_x1,                                     \
		var_x2,                                     \
		INKNIT_X_MIN,                               \
		INKNIT_X_MAX,                               \
		MSG_X1_LESS_THAN_MIN_VALUE,                 \
		MSG_X1_GREATER_THAN_X2,                     \
		MSG_X2_GREATER_THAN_MAX_VALUE               \
	)
#define INKNIT_ASSUME_COORD_Y_RANGE(var_y1, var_y2) \
	INKNIT_ASSUME_BOUNDED_RANGE(                    \
		var_y1,                                     \
		var_y2,                                     \
		INKNIT_Y_MIN,                               \
		INKNIT_Y_MAX,                               \
		MSG_Y1_LESS_THAN_MIN_VALUE,                 \
		MSG_Y1_GREATER_THAN_Y2,                     \
		MSG_Y2_GREATER_THAN_MAX_VALUE               \
	)

#define INKNIT_ASSUME_CLIP_X_RANGE(var_min_x, var_max_x) \
	INKNIT_ASSUME_BOUNDED_RANGE(                         \
		var_min_x,                                       \
		var_max_x,                                       \
		0,                                               \
		INKNIT_X_MAX,                                    \
		MSG_MIN_X_LESS_THAN_ZERO,                        \
		MSG_MIN_X_GREATER_THAN_MAX_X,                    \
		MSG_MAX_X_GREATER_THAN_MAX_VALUE                 \
	)
#define INKNIT_ASSUME_CLIP_Y_RANGE(var_min_y, var_max_y) \
	INKNIT_ASSUME_BOUNDED_RANGE(                         \
		var_min_y,                                       \
		var_max_y,                                       \
		0,                                               \
		INKNIT_Y_MAX,                                    \
		MSG_MIN_Y_LESS_THAN_ZERO,                        \
		MSG_MIN_Y_GREATER_THAN_MAX_Y,                    \
		MSG_MAX_Y_GREATER_THAN_MAX_VALUE                 \
	)
#define INKNIT_ASSUME_CLIP_RECT(var_rect)                            \
	INKNIT_ASSUME_CLIP_X_RANGE((var_rect)->left, (var_rect)->right); \
	INKNIT_ASSUME_CLIP_Y_RANGE((var_rect)->top, (var_rect)->bottom)

#define INKNIT_ASSUME_COLOR(var_color)                                                       \
	INKNIT_ASSUME((var_color) <= POW2_BITS_PER_PIXEL_NEG1, MSG_COLOR_GREATER_THAN_MAX_VALUE)
