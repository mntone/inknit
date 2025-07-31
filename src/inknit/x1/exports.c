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
#include "../inc/inknit/x1.h"

const struct inknit_exports x1 = {
	.blit       = INKNIT_X1_NAME(blit),
	.clear      = INKNIT_X1_NAME(clear),
	.draw_point = INKNIT_X1_NAME(draw_point),
	.draw_hline = INKNIT_X1_NAME(draw_hline),
	.draw_vline = INKNIT_X1_NAME(draw_vline),
	.draw_line  = INKNIT_X1_NAME(draw_line),
	.draw_rect  = INKNIT_X1_NAME(draw_rect),
	.draw_rectp = INKNIT_X1_NAME(draw_rectp),
	.features   = INKNIT_FEATURE_PRIMITIVE | INKNIT_FEATURE_BLIT | INKNIT_FEATURE_DRAW,
	.layout     = INKNIT_X1,
};

#endif
