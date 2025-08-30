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
#include "../macro_impl.h"

/**
 * @brief Clip an inclusive X-range against an exclusive rectangle.
 *
 * This function clips the horizontal range [*x1, *x2] (inclusive endpoints)
 * to the valid range defined by the exclusive rectangle @p r.
 *
 * After clipping, @p *x1 and @p *x2 are updated to represent
 * the inclusive intersection with the rectangle.
 *
 * @param[in,out] x1    Pointer to the left coordinate (inclusive).
 * @param[in,out] x2    Pointer to the right coordinate (inclusive).
 * @param[in]     rect  Pointer to the exclusive rectangle to clip against.
 *
 * @retval true   If the clipped range contains at least one pixel.
 * @retval false  If the range lies completely outside the rectangle.
 */
static INKNIT_ALWAYS_INLINE bool clip_range_x_exclusive(
	int32_t *INKNIT_RESTRICT                  x1,
	int32_t *INKNIT_RESTRICT                  x2,
	const struct inknit_rect *INKNIT_RESTRICT rect
) {
	if (*x2 < rect->left || rect->right <= *x1) {
		return false;
	}
	if (*x1 < rect->left) {
		*x1 = rect->left;
	}
	if (*x2 >= rect->right) {
		*x2 = rect->right - 1;
	}
	return *x1 <= *x2;
}

/**
 * @brief Clip an inclusive Y-range against an exclusive rectangle.
 *
 * This function clips the vertical range [*y1, *y2] (inclusive endpoints)
 * to the valid range defined by the exclusive rectangle @p r.
 *
 * After clipping, @p *y1 and @p *y2 are updated to represent
 * the inclusive intersection with the rectangle.
 *
 * @param[in,out] y1    Pointer to the top coordinate (inclusive).
 * @param[in,out] y2    Pointer to the bottom coordinate (inclusive).
 * @param[in]     rect  Pointer to the exclusive rectangle to clip against.
 *
 * @retval true   If the clipped range contains at least one pixel.
 * @retval false  If the range lies completely outside the rectangle.
 */
static INKNIT_ALWAYS_INLINE bool clip_range_y_exclusive(
	int32_t *INKNIT_RESTRICT                  y1,
	int32_t *INKNIT_RESTRICT                  y2,
	const struct inknit_rect *INKNIT_RESTRICT rect
) {
	if (*y2 < rect->top || rect->bottom <= *y1) {
		return false;
	}
	if (*y1 < rect->top) {
		*y1 = rect->top;
	}
	if (*y2 >= rect->bottom) {
		*y2 = rect->bottom - 1;
	}
	return *y1 <= *y2;
}
