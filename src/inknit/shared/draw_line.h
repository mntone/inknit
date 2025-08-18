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

#include "../inc/inknit/types.h"
#include "../inc/inknit/macro.h"

enum _inknit_line_outcode {
	INKNIT_LINE_OUTCODE_INSIDE = 0,
	INKNIT_LINE_OUTCODE_LEFT   = 1 << 1,
	INKNIT_LINE_OUTCODE_RIGHT  = 1 << 2,
	INKNIT_LINE_OUTCODE_BOTTOM = 1 << 3,
	INKNIT_LINE_OUTCODE_TOP    = 1 << 4,
};

bool _inknit_clip_line_to_bounds(
	int32_t *INKNIT_RESTRICT                  x1,
	int32_t *INKNIT_RESTRICT                  y1,
	int32_t *INKNIT_RESTRICT                  x2,
	int32_t *INKNIT_RESTRICT                  y2,
	const struct inknit_rect *INKNIT_RESTRICT clip_rect
);
