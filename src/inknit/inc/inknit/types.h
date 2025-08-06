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

#include <stdbool.h>  // bool
#include <stdint.h>   // uint16_t, int32_t, uint32_t
#include "macro.h"    // INKNIT_RESTRICT

typedef uint32_t inknit_color_t;
typedef uint32_t inknit_uint_t;

#define PL(_CHANNEL, _BASE) ((_CHANNEL - 1) << 6) | ((_BASE - 1) << 2)
#define EX                  (1 << 1)
#define LE                  1

enum inknit_pixellayout {
	INKNIT_X1    = PL(1, 1),        ///< 0123 4567 89ab cdef ...
	INKNIT_X1LSB = INKNIT_X1 | LE,  ///< 7654 3210 fedc ba98 ... (LSB first+4-bytes LE)
	INKNIT_X2    = PL(1, 2),        ///< 0011 2233 4455 6677 ...
	INKNIT_X2LSB = INKNIT_X2 | LE,  ///< 3322 1100 7766 5544 ... (LSB first+4-bytes LE)
	INKNIT_X4    = PL(1, 4),        ///< 0000 1111 2222 3333 ...
	INKNIT_X4LSB = INKNIT_X4 | LE,  ///< 1111 0000 3333 2222 ... (LSB first+4-bytes LE)
	INKNIT_X8    = PL(1, 8),        ///< 0000 0000 1111 1111 ...
	INKNIT_X16   = PL(1, 16),

	INKNIT_X3Y3Z2      = PL(3, 2),  ///< x[ 7: 5], y[ 4: 2], z[1:0]
	INKNIT_X5Y6Z5      = PL(3, 5),  ///< x[15:11], y[10: 5], z[4:0]
	INKNIT_X5Y6Z5LE    = INKNIT_X5Y6Z5 | LE,
	INKNIT_X8Y8Z8      = PL(3, 8),            ///< x[23:16], y[15: 8], z[ 7: 0]
	INKNIT_X8Y8Z8LE    = INKNIT_X8Y8Z8 | LE,  ///< x[ 7: 0], y[15: 8], z[23:16]
	INKNIT_X11Y11Z10   = PL(3, 10),           ///< x[31:21], y[20:10], z[ 9: 0]
	INKNIT_X11Y11Z10LE = INKNIT_X11Y11Z10 | LE,

	// EX=REV
	INKNIT_X2Y2Z2W2      = PL(4, 2),              ///< x[ 7: 6], y[ 5: 4], z[ 3: 2], w[ 1: 0]
	INKNIT_X4Y4Z4W4      = PL(4, 4),              ///< x[15:12], y[11: 8], z[ 7: 4], w[ 3: 0]
	INKNIT_X1Y5Z5W5      = PL(4, 5),              ///< x[15:15], y[14:10], z[ 9: 5], w[ 4: 0]
	INKNIT_X8Y8Z8W8      = PL(4, 8),              ///< x[31:24], y[23:16], z[15: 8], w[ 7: 0]
	INKNIT_X8Y8Z8W8LE    = INKNIT_X8Y8Z8W8 | LE,  ///< x[ 7: 0], y[15: 8], z[23:16], w[31:24]
	INKNIT_X2Y10Z10W10   = PL(4, 10),             ///< x[31:30], y[29:20], z[19:10], w[ 9: 0]
	INKNIT_X2Y10Z10W10LE = INKNIT_X2Y10Z10W10 | LE,
	INKNIT_W10Z10Y10X2   = PL(4, 10) | EX,  ///< x[ 1: 0], y[11: 2], z[21:12], w[31:22]
	INKNIT_W10Z10Y10X2LE = INKNIT_W10Z10Y10X2 | LE,
};

#undef PL
#undef EX
#undef LE

enum inknit_pixelformat {
	INKNIT_INDEX,      ///< 1ch
	INKNIT_GRAYSCALE,  ///< 1ch

	INKNIT_BGR,   ///< 3ch
	INKNIT_XBGR,  ///< 4ch
	INKNIT_ABGR,  ///< 4ch
};

enum inknit_colorspace {
	INKNIT_COLORSPACE_UNSPECIFIED,
	INKNIT_SRGB,      ///< sRGB
	INKNIT_ADOBERGB,  ///< Adobe RGB
	INKNIT_DISPLAYP3,
	INKNIT_BT601,
	INKNIT_BT709,
	INKNIT_DCIP3,
	INKNIT_BT2020,
};

enum inknit_colorrange {
	INKNIT_COLORRANGE_FULL,     // 0-255, 0-1023, 0-4095
	INKNIT_COLORRANGE_LIMITED,  // 16-235/240 (uv), 64-940/960 (uv), 256-3760/3840 (uv)
};

enum inknit_alignment {
	INKNIT_ALIGN8,
	INKNIT_ALIGN16,
	INKNIT_ALIGN32,
	INKNIT_ALIGN64,
	INKNIT_ALIGN128,
	INKNIT_ALIGN256,
	INKNIT_ALIGN512,
	INKNIT_ALIGN1024,
};

struct inknit_rect {
	int16_t left;
	int16_t top;
	int16_t right;
	int16_t bottom;
};

struct inknit_image {
	const enum inknit_pixellayout pixel_layout : 8;
	const enum inknit_pixelformat pixel_format : 4;
	const enum inknit_colorspace  colorspace   : 3;
	const enum inknit_colorrange  colorrange   : 1;

	const uint16_t              stride;
	const enum inknit_alignment alignment : 3;
	const uint32_t              width     : 13;
	const uint16_t              height;
	void *INKNIT_RESTRICT       data;
};
