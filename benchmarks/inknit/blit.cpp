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

#include "inknit.hpp"
#include "utils/inkbm.hpp"
#include "constants.hpp"

using namespace inknit;
using namespace inkbm;

#define TYPES      std::tuple<uint_t, uint_t, uint_t, uint_t, uint_t, uint_t>
#define ITERATIONS 100

#define GROUP_NAME  blit
#define APPLY(name) INKBM_FIXTURE_APPLY(name, blit, f_blit)
#define FUNC_NAME(name)                                                               \
	INKNIT_INTERNAL_FUNCNAME(INKNIT_X1LSB_BASE, _INKNIT_CONCAT3(GROUP_NAME, _, name))

class f_blit: public fixture {
public:
	explicit f_blit() {
		src_.clear(COLOR_BLACK);
		for (std::int32_t y = 0; y < src_.height(); ++y) {
			for (std::int32_t x = 0; x < src_.width(); ++x) {
				if (((x / 8) + (y / 8)) % 2 == 0) {
					src_.draw_point(x, y, COLOR_BLACK);
				} else {
					src_.draw_point(x, y, COLOR_WHITE);
				}
			}
		}
	}

	void load(std::any const *data) noexcept override {
		auto const *ptr = std::any_cast<TYPES>(data);
		if (ptr == nullptr) {
			INKBM_FATAL("Failed to unbind args.");
		}

		auto [dx, dy, sx, sy, width, height] = *ptr;

		dx_     = dx;
		dy_     = dy;
		sx_     = sx;
		sy_     = sy;
		width_  = width;
		height_ = height;
	}

	void setup() noexcept override {
		dst_.clear(COLOR_BLACK);
	}

protected:
	fixed_image<96, 96, pixel_layout::x1lsb, pixel_format::grayscale> src_;
	fixed_image<96, 96, pixel_layout::x1lsb, pixel_format::grayscale> dst_;

	uint_t dx_, dy_, sx_, sy_, width_, height_;
};

INKBM_ARGS(
	blit,
	"Test Method",
	// clang-format off
	{
		// {"short_name", "description", TYPES {dx, dy, sx, sy, width, height}},
		// b: blit
		// sq: square, w: wide, t: tall (shape)
		// a: aligned, u: unaligned (alignment)
		// bnd: boundary crossing
		// ovl: overlap
		// clp: clip
		// rd, ru, ld, lu: overlap direction (right-down, right-up, etc.)

		// -- basic performance & sizing
		// Measures basic copy performance with various sizes and aspect ratios.
		{"b_sq_16_a",  "16x16 square, aligned", TYPES {32, 32, 32, 32, 16, 16}},
		{"b_sq_64_a",  "64x64 square, aligned", TYPES {32, 32, 32, 32, 64, 64}},
		{"b_w_64x8_a", "64x8 wide, aligned",    TYPES {32, 32, 32, 32, 64,  8}},
		{"b_t_8x64_a", "8x64 tall, aligned",    TYPES {32, 32, 32, 32,  8, 64}},

		// -- alignment performance
		// Measures performance changes due to differences in source/destination alignment.
		// a_a: aligned src/dst, u_a: unaligned src, a_u: unaligned dst, u_u: unaligned src/dst
		{"b_32_a_a", "32x32, src/dst aligned",   TYPES {32, 10, 32, 10, 32, 32}},
		{"b_32_u_a", "32x32, src unaligned",     TYPES {32, 10, 33, 10, 32, 32}},
		{"b_32_a_u", "32x32, dst unaligned",     TYPES {33, 10, 32, 10, 32, 32}},
		{"b_32_u_u", "32x32, src/dst unaligned", TYPES {33, 10, 35, 10, 32, 32}},

		// -- boundary crossing performance
		// Measures performance when the source or destination crosses 32-bit word boundaries.
		{"b_16_bnd_s", "16x16, starts aligned, crosses boundary",   TYPES {24, 10, 24, 10, 16, 16}},
		{"b_16_bnd_u", "16x16, starts unaligned, crosses boundary", TYPES {25, 10, 25, 10, 16, 16}},
		{"b_48_bnd_m", "48x16, crosses multiple boundaries",        TYPES {24, 10, 24, 10, 48, 16}},

		// -- overlapping regions (overlap)
		// The most critical cases for blit. The implementation must correctly detect the
		// direction of overlap to copy the data without corrupting the source.
		{"b_16_ovl_rd", "16x16, overlap right-down", TYPES {12, 12, 10, 10, 16, 16}},
		{"b_16_ovl_lu", "16x16, overlap left-up",    TYPES {10, 10, 12, 12, 16, 16}},
		{"b_16_ovl_ru", "16x16, overlap right-up",   TYPES {12, 10, 10, 12, 16, 16}},
		{"b_16_ovl_ld", "16x16, overlap left-down",  TYPES {10, 12, 12, 10, 16, 16}},

		// -- clipping
		// Measures the performance of clipping operations against the image boundaries.
		// {"b_32_clp_tl",  "32x32, clipped top-left",          TYPES { -8,  -8, 10, 10, 32, 32}},
		// {"b_32_clp_br",  "32x32, clipped bottom-right",      TYPES {100, 100, 10, 10, 32, 32}}, // Assuming 128x128 canvas
		// {"b_32_clp_src", "32x32, src clipped by src bounds", TYPES {10, 10, 110, 110, 32, 32}},

		// -- edge cases
		// Confirms the robustness of the algorithm.
		{"b_1x1_a",  "1x1 blit, aligned",   TYPES {32, 32, 32, 32, 1, 1}},
		{"b_1x1_u",  "1x1 blit, unaligned", TYPES {33, 33, 33, 33, 1, 1}},
		{"b_0x16",   "Zero width blit",     TYPES {10, 10, 10, 10, 0, 16}},
		{"b_16x0",   "Zero height blit",    TYPES {10, 10, 10, 10, 16, 0}},
	},
	// clang-format on
);

APPLY(pixel_old) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(pixel_old)(&dst_, dx_, dy_, &src_, sx_, sy_, width_, height_);
	}
}

APPLY(pixel) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(pixel)(&dst_, dx_, dy_, &src_, sx_, sy_, width_, height_);
	}
}

APPLY(unaligned) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(unaligned)(&dst_, dx_, dy_, &src_, sx_, sy_, width_, height_);
	}
}

APPLY(mask) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(mask)(&dst_, dx_, dy_, &src_, sx_, sy_, width_, height_);
	}
}
