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

#include "draw_line.h"

#include "../macro_impl.h"

static inline enum _inknit_line_outcode
_inknit_compute_line_outcode(int32_t x, int32_t y, const struct inknit_rect *clip_rect) {
	enum _inknit_line_outcode code = INKNIT_LINE_OUTCODE_INSIDE;

	if (x < clip_rect->left) {
		code |= INKNIT_LINE_OUTCODE_LEFT;
	} else if (clip_rect->right <= x) {
		code |= INKNIT_LINE_OUTCODE_RIGHT;
	}

	if (y < clip_rect->top) {
		code |= INKNIT_LINE_OUTCODE_TOP;
	} else if (clip_rect->bottom <= y) {
		code |= INKNIT_LINE_OUTCODE_BOTTOM;
	}

	return code;
}

static INKNIT_ALWAYS_INLINE int32_t
_inknit_lerp(int32_t p1, int32_t p2, int32_t q1, int32_t q2, int32_t t) {
	const int32_t dq = q2 - q1;
	if (dq == 0) {
		return p1;
	}

	const int32_t FIXED_SHIFT = 6;

	const int32_t dp    = p2 - p1;
	const int32_t numer = dp * (t - q1);
	const int32_t val   = p1 + ((numer << FIXED_SHIFT) / dq >> FIXED_SHIFT);
	return val;
}

// Clips a line segment from (x1, y1) to (x2, y2) against clip_rect.
// Returns true if the line should be drawn, and updates the endpoint
// coordinates (x1, y1, x2, y2) by reference.
bool _inknit_clip_line_to_bounds(
	int32_t *INKNIT_RESTRICT                  x1,
	int32_t *INKNIT_RESTRICT                  y1,
	int32_t *INKNIT_RESTRICT                  x2,
	int32_t *INKNIT_RESTRICT                  y2,
	const struct inknit_rect *INKNIT_RESTRICT clip_rect
) {
	enum _inknit_line_outcode outcode1 = _inknit_compute_line_outcode(*x1, *y1, clip_rect);
	enum _inknit_line_outcode outcode2 = _inknit_compute_line_outcode(*x2, *y2, clip_rect);

	while (1) {
		// Trivial Accept: both endpoints are inside the clipping rectangle.
		if (!(outcode1 | outcode2)) {
			return true;
		}

		// Trivial Reject: both endpoints are in the same outside region.
		else if (outcode1 & outcode2) {
			return false;
		}

		// Clipping required.
		else {
			enum _inknit_line_outcode outcode = outcode1 ? outcode1 : outcode2;
			int32_t                   x, y;

			if (outcode & INKNIT_LINE_OUTCODE_TOP) {
				x = _inknit_lerp(*x1, *x2, *y1, *y2, clip_rect->top);
				y = clip_rect->top;
			} else if (outcode & INKNIT_LINE_OUTCODE_BOTTOM) {
				x = _inknit_lerp(*x1, *x2, *y1, *y2, clip_rect->bottom);
				y = clip_rect->bottom - 1;
			} else if (outcode & INKNIT_LINE_OUTCODE_RIGHT) {
				x = clip_rect->right - 1;
				y = _inknit_lerp(*y1, *y2, *x1, *x2, clip_rect->right);
			} else if (outcode & INKNIT_LINE_OUTCODE_LEFT) {
				x = clip_rect->left;
				y = _inknit_lerp(*y1, *y2, *x1, *x2, clip_rect->left);
			}

			if (outcode == outcode1) {
				*x1 = x;
				*y1 = y;

				outcode1 = _inknit_compute_line_outcode(x, y, clip_rect);
			} else {
				*x2 = x;
				*y2 = y;

				outcode2 = _inknit_compute_line_outcode(x, y, clip_rect);
			}
		}
	}
}
