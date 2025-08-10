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

#include "inknit/types.h"
#include "inknit/prefix.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pixel Layout: x1 (32-bit width/align, 1-bpp, BE)
#define INKNIT_CURRENT_GROUP INKNIT_INTERNAL_GROUP(32, 1, be)
#include "inknit/proto_internal.h.in"
#undef INKNIT_CURRENT_GROUP

// Pixel Layout: x1lsb (32-bit width/align, 1-bpp, LE)
#define INKNIT_CURRENT_GROUP INKNIT_INTERNAL_GROUP(32, 1, le)
#include "inknit/proto_internal.h.in"
#undef INKNIT_CURRENT_GROUP

// Pixel Layout: x2 (32-bit width/align, 2-bpp, BE)
#define INKNIT_CURRENT_GROUP INKNIT_INTERNAL_GROUP(32, 2, be)
#include "inknit/proto_internal.h.in"
#undef INKNIT_CURRENT_GROUP

// Pixel Layout: x2lsb (32-bit width/align, 2-bpp, LE)
#define INKNIT_CURRENT_GROUP INKNIT_INTERNAL_GROUP(32, 2, le)
#include "inknit/proto_internal.h.in"
#undef INKNIT_CURRENT_GROUP

// Pixel Layout: x4 (32-bit width/align, 4-bpp, BE)
#define INKNIT_CURRENT_GROUP INKNIT_INTERNAL_GROUP(32, 4, be)
#include "inknit/proto_internal.h.in"
#undef INKNIT_CURRENT_GROUP

// Pixel Layout: x4lsb (32-bit width/align, 4-bpp, LE)
#define INKNIT_CURRENT_GROUP INKNIT_INTERNAL_GROUP(32, 4, le)
#include "inknit/proto_internal.h.in"
#undef INKNIT_CURRENT_GROUP

#ifdef __cplusplus
} /* extern "C" { */
#endif
