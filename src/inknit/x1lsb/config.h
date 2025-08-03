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

#include "../inc/inknit/prefix.h"

// - Pixel Layout: x1lsb
// - Pixel Format: index or grayscale

#define INKNIT_CURRENT_GROUP    INKNIT_INTERNAL_GROUP(32, 1, le)
#define TARGET_LAYOUT           INKNIT_X1LSB
#define PUBLIC_FUNCNAME(name)   INKNIT_PUBLIC_FUNCNAME(INKNIT_X1LSB_BASE, name)
#define INTERNAL_FUNCNAME(name) INKNIT_INTERNAL_FUNCNAME(INKNIT_X1LSB_BASE, name)
#define INTERNAL_VARNAME(name)  INKNIT_INTERNAL_VARNAME(INKNIT_X1LSB_BASE, name)

#define IS_LITTLE       1
#define BITS_PER_PIXEL  1
#define PIXELS_PER_BYTE 8
#define PIXELS_PER_WORD 32

#define BITS_PER_WORD_NEG1       31    // BITS_PER_WORD - 1
#define LOG2_BITS_PER_WORD       5     // log2(BITS_PER_WORD)
#define PIXELS_PER_BYTE_NEG1     0x07  // PIXELS_PER_BYTE - 1
#define PIXELS_PER_WORD_NEG1     0x1F  // PIXELS_PER_WORD - 1
#define POW2_BITS_PER_PIXEL_NEG1 1     // 2^(BITS_PER_PIXEL) - 1
#define LOG2_BITS_PER_PIXEL      0     // log2(BITS_PER_PIXEL)
#define LOG2_PIXELS_PER_BYTE     3     // log2(PIXELS_PER_BYTE)
#define LOG2_PIXELS_PER_WORD     5     // log2(PIXELS_PER_WORD)
