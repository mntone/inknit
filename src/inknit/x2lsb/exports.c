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

#define INKNIT_USE_MACRO_NAME
#include "../inc/inknit/x2lsb.h"

const struct inknit_exports x2lsb = {
	.blit            = inknit_blit,
	.clear           = inknit_clear,
	.fill_rect       = inknit_fill_rect,
	.fill_rect_coord = inknit_fill_rect_coord,
	.draw_circle     = inknit_draw_circle,
	.draw_ellipse    = inknit_draw_ellipse,
	.draw_point      = inknit_draw_point,
	.draw_hline      = inknit_draw_hline,
	.draw_vline      = inknit_draw_vline,
	.draw_line       = inknit_draw_line,
	.draw_rect       = inknit_draw_rect,
	.draw_rect_coord = inknit_draw_rect_coord,
	.features        = INKNIT_FEATURE_PRIMITIVE | INKNIT_FEATURE_BLIT | INKNIT_FEATURE_DRAW,
	.layout          = INKNIT_X2LSB,
};

#endif
