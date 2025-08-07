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

#ifdef INKNIT_ENABLE_EXPORTS

#include "../inc/inknit_exports.h"
#include "../inc/inknit/x2.h"

const struct inknit_exports x2 = {
	.blit            = INKNIT_X2_NAME(blit),
	.clear           = INKNIT_X2_NAME(clear),
	.fill_rect       = INKNIT_X2_NAME(fill_rect),
	.fill_rect_coord = INKNIT_X2_NAME(fill_rect_coord),
	.draw_circle     = INKNIT_X2_NAME(draw_circle),
	.draw_ellipse    = INKNIT_X2_NAME(draw_ellipse),
	.draw_point      = INKNIT_X2_NAME(draw_point),
	.draw_hline      = INKNIT_X2_NAME(draw_hline),
	.draw_vline      = INKNIT_X2_NAME(draw_vline),
	.draw_line       = INKNIT_X2_NAME(draw_line),
	.draw_rect       = INKNIT_X2_NAME(draw_rect),
	.draw_rect_coord = INKNIT_X2_NAME(draw_rect_coord),
	.features        = INKNIT_FEATURE_PRIMITIVE | INKNIT_FEATURE_BLIT | INKNIT_FEATURE_DRAW,
	.layout          = INKNIT_X2,
};

#endif
