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

// -- MARK: macro definitions
#include "inknit/macro.h"
#include "inknit/prefix.h"
#include "inknit/proto.h"


// -- MARK: primitive type definitions
#include "inknit/types.h"

static inline bool inknit_image_is_single_channel(struct inknit_image const *image) {
	return (image->pixel_layout & 0xC0) == 0x00;
}

static inline bool inknit_image_is_grayscale(struct inknit_image const *image) {
	return inknit_image_is_single_channel(image) && image->pixel_format == INKNIT_GRAYSCALE;
}


// -- MARK: function prototypes
#include "inknit/x1.h"
#include "inknit/x1lsb.h"
#include "inknit/x2.h"
#include "inknit/x2lsb.h"
#include "inknit/x4.h"
#include "inknit/x4lsb.h"
