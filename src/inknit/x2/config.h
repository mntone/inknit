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

// - Pixel Layout: x2
// - Pixel Format: index or grayscale

#define INKNIT_CURRENT_LAYOUT x2
#define INKNIT_CURRENT_ENDIAN be
#define INKNIT_CURRENT_GROUP  INKNIT_INTERNAL_GROUP(32, 2, be)

#define PIXEL_LAYOUT    INKNIT_X2
#define IS_LITTLE       0
#define BITS_PER_PIXEL  2
#define PIXELS_PER_BYTE 4
#define PIXELS_PER_WORD 16

#define BITS_PER_WORD_NEG1       31    // BITS_PER_WORD - 1
#define LOG2_BITS_PER_WORD       5     // log2(BITS_PER_WORD)
#define PIXELS_PER_BYTE_NEG1     0x03  // PIXELS_PER_BYTE - 1
#define PIXELS_PER_WORD_NEG1     0x0F  // PIXELS_PER_WORD - 1
#define POW2_BITS_PER_PIXEL_NEG1 3     // 2^(BITS_PER_PIXEL) - 1
#define LOG2_BITS_PER_PIXEL      1     // log2(BITS_PER_PIXEL)
#define LOG2_PIXELS_PER_BYTE     2     // log2(PIXELS_PER_BYTE)
#define LOG2_PIXELS_PER_WORD     4     // log2(PIXELS_PER_WORD)
