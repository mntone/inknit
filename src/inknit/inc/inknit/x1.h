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

#include "proto.h"  // load base proto
#include "types.h"  // load base types

#ifdef __cplusplus
extern "C" {
#endif

// -- MARK: macro
#define INKNIT_X1_BASE       x1
#define INKNIT_X1_NAME(name) INKNIT_PUBLIC_FUNCNAME(INKNIT_X1_BASE, name)
#define PUBN(name)           INKNIT_PUBLIC_FUNCNAME(INKNIT_X1_BASE, name)
#define INTN(name)           INKNIT_INTERNAL_FUNCNAME(INKNIT_X1_BASE, name)


// -- MARK: primitive
inknit_color_t INKNIT_EXPORT
	PUBN(get_pixel)(const struct inknit_image *image, int32_t x, int32_t y) INKNIT_NONNULL(1);

void INKNIT_EXPORT PUBN(clear)(struct inknit_image *image, inknit_color_t color) INKNIT_NONNULL(1);


// -- MARK: draw
void INKNIT_EXPORT PUBN(draw_circle)(
	struct inknit_image *image, int32_t cx, int32_t cy, int32_t radius, inknit_color_t color
) INKNIT_NONNULL(1);

void INKNIT_EXPORT PUBN(draw_ellipse)(
	struct inknit_image *image, int32_t cx, int32_t cy, int32_t rx, int32_t ry, inknit_color_t color
) INKNIT_NONNULL(1);

void INKNIT_EXPORT PUBN(draw_hline)(
	struct inknit_image *image, int32_t x1, int32_t x2, int32_t y, inknit_color_t color
) INKNIT_NONNULL(1);

void INKNIT_EXPORT PUBN(draw_line)(
	struct inknit_image *image, int32_t x1, int32_t y1, int32_t x2, int32_t y2, inknit_color_t color
) INKNIT_NONNULL(1);

void INKNIT_EXPORT PUBN(draw_point)(
	struct inknit_image *image, int32_t x, int32_t y, inknit_color_t color
) INKNIT_NONNULL(1);

void INKNIT_EXPORT PUBN(draw_rect)(
	struct inknit_image *image,
	int32_t              x,
	int32_t              y,
	int32_t              width,
	int32_t              height,
	inknit_color_t       color
) INKNIT_NONNULL(1);

void INKNIT_EXPORT PUBN(draw_rect_coord)(
	struct inknit_image *image, int32_t x1, int32_t y1, int32_t x2, int32_t y2, inknit_color_t color
) INKNIT_NONNULL(1);

void INKNIT_EXPORT PUBN(draw_vline)(
	struct inknit_image *image, int32_t x, int32_t y1, int32_t y2, inknit_color_t color
) INKNIT_NONNULL(1);


// -- MARK: blit
INKNIT_BMPROTO(INKNIT_PROTO_BLIT(INTN(blit_pixel_old)));
INKNIT_BMPROTO(INKNIT_PROTO_BLIT(INTN(blit_pixel)));
INKNIT_BMPROTO(INKNIT_PROTO_BLIT(INTN(blit_unaligned)));
INKNIT_BMPROTO(INKNIT_PROTO_BLIT(INTN(blit_mask)));
INKNIT_BMPROTO(INKNIT_PROTO_BLIT(INTN(blit_aligned)));
INKNIT_PROTO(INKNIT_PROTO_BLIT(PUBN(blit)));


// -- MARK: clean up
#undef PUBN
#undef INTN

#ifdef __cplusplus
}
#endif
