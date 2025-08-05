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

#ifdef INKNIT_ENABLE_EXPORTS

#include "inknit.h"

#ifdef __cplusplus
extern "C" {
#endif

enum inknit_features {
	INKNIT_FEATURE_PRIMITIVE = (1 << 0),
	INKNIT_FEATURE_BLIT      = (1 << 1),
	INKNIT_FEATURE_SCALE     = (1 << 2),
	INKNIT_FEATURE_DRAW      = (1 << 3),
};

struct inknit_exports {
	void (*const blit)(
		struct inknit_image *INKNIT_RESTRICT       dst,
		inknit_uint_t                              dx,
		inknit_uint_t                              dy,
		const struct inknit_image *INKNIT_RESTRICT src,
		inknit_uint_t                              sx,
		inknit_uint_t                              sy,
		inknit_uint_t                              width,
		inknit_uint_t                              height
	) INKNIT_NONNULL(1, 4);

	void (*const clear)(struct inknit_image *image, inknit_color_t color) INKNIT_NONNULL(1);

	void (*const draw_circle)(
		struct inknit_image *image, int32_t cx, int32_t cy, int32_t radius, inknit_color_t color
	) INKNIT_NONNULL(1);

	void (*const draw_ellipse)(
		struct inknit_image *image,
		int32_t              cx,
		int32_t              cy,
		int32_t              rx,
		int32_t              ry,
		inknit_color_t       color
	) INKNIT_NONNULL(1);

	void (*const draw_point)(
		struct inknit_image *image, int32_t x, int32_t y, inknit_color_t color
	) INKNIT_NONNULL(1);

	void (*const draw_hline)(
		struct inknit_image *image,
		inknit_uint_t        x1,
		inknit_uint_t        x2,
		inknit_uint_t        y,
		inknit_color_t       color
	) INKNIT_NONNULL(1);

	void (*const draw_vline)(
		struct inknit_image *image,
		inknit_uint_t        x,
		inknit_uint_t        y1,
		inknit_uint_t        y2,
		inknit_color_t       color
	) INKNIT_NONNULL(1);

	void (*const draw_line)(
		struct inknit_image *image,
		int32_t              x1,
		int32_t              y1,
		int32_t              x2,
		int32_t              y2,
		inknit_color_t       color
	) INKNIT_NONNULL(1);

	void (*const draw_rect)(
		struct inknit_image *image,
		inknit_uint_t        x,
		inknit_uint_t        y,
		inknit_uint_t        width,
		inknit_uint_t        height,
		inknit_color_t       color
	) INKNIT_NONNULL(1);

	void (*const draw_rectp)(
		struct inknit_image *image,
		inknit_uint_t        x1,
		inknit_uint_t        y1,
		inknit_uint_t        x2,
		inknit_uint_t        y2,
		inknit_color_t       color
	) INKNIT_NONNULL(1);

	const enum inknit_features    features;
	const enum inknit_pixellayout layout;
};


// -- MARK: Extern variables
extern const struct inknit_exports x1;
extern const struct inknit_exports x1lsb;
extern const struct inknit_exports x2;
extern const struct inknit_exports x2lsb;
extern const struct inknit_exports x4;
extern const struct inknit_exports x4lsb;

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif
