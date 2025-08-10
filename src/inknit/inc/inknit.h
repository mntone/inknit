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

// -- MARK: headers
#include "inknit/assertion.h"
#include "inknit/types.h"
#include "inknit/prefix.h"


// -- MARK: primitive type definitions
static inline bool inknit_image_is_single_channel(struct inknit_image const *image) {
	return (image->pixel_layout & 0xC0) == 0x00;
}

static inline bool inknit_image_is_grayscale(struct inknit_image const *image) {
	return inknit_image_is_single_channel(image) && image->pixel_format == INKNIT_GRAYSCALE;
}


// -- MARK: function prototypes
#ifdef __cplusplus
extern "C" {
#endif

// Pixel Layout: x1 (32-bit width/align, 1-bpp, BE)
#define INKNIT_CURRENT_LAYOUT x1
#include "inknit/proto.h.in"
#undef INKNIT_CURRENT_LAYOUT

// Pixel Layout: x1lsb (32-bit width/align, 1-bpp, LE)
#define INKNIT_CURRENT_LAYOUT x1lsb
#include "inknit/proto.h.in"
#undef INKNIT_CURRENT_LAYOUT

// Pixel Layout: x2 (32-bit width/align, 2-bpp, BE)
#define INKNIT_CURRENT_LAYOUT x2
#include "inknit/proto.h.in"
#undef INKNIT_CURRENT_LAYOUT

// Pixel Layout: x2lsb (32-bit width/align, 2-bpp, LE)
#define INKNIT_CURRENT_LAYOUT x2lsb
#include "inknit/proto.h.in"
#undef INKNIT_CURRENT_LAYOUT

// Pixel Layout: x4 (32-bit width/align, 4-bpp, BE)
#define INKNIT_CURRENT_LAYOUT x4
#include "inknit/proto.h.in"
#undef INKNIT_CURRENT_LAYOUT

// Pixel Layout: x4lsb (32-bit width/align, 4-bpp, LE)
#define INKNIT_CURRENT_LAYOUT x4lsb
#include "inknit/proto.h.in"
#undef INKNIT_CURRENT_LAYOUT

#ifdef __cplusplus
} /* extern "C" { */
#endif
