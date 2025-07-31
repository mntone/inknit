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

#define TYPES      std::tuple<uint_t, uint_t, uint_t, uint_t>
#define ITERATIONS DEFAULT_ITERATIONS

#define GROUP_NAME  draw_line
#define APPLY(name) INKBM_FIXTURE_APPLY(name, draw_line, d_line)
#define FUNC_NAME(name)                                                               \
	INKNIT_INTERNAL_FUNCNAME(INKNIT_X1LSB_BASE, _INKNIT_CONCAT3(GROUP_NAME, _, name))

class d_line: public fixture {
public:
	void load(std::any const *data) noexcept override {
		auto const *ptr = std::any_cast<TYPES>(data);
		if (ptr == nullptr) {
			INKBM_FATAL("Failed to unbind args.");
		}

		auto [x1, y1, x2, y2] = *ptr;

		x1_ = x1;
		y1_ = y1;
		x2_ = x2;
		y2_ = y2;
	}

	void setup() noexcept override {
		image_.clear(COLOR_BLACK);
	}

protected:
	fixed_image<128, 72, pixel_layout::x1lsb, pixel_format::grayscale> image_;

	uint_t x1_, y1_, x2_, y2_;
};

INKBM_ARGS(
	draw_line,
	"Test Method",
	// clang-format off
	{
		// {"short_name", "description", TYPES {x1, y1, x2, y2}},
		// s: shallow, t: steep, d: 45-degree
		// rd: right-down, ru: right-up, ld: left-down, lu: left-up

		// -- short lines, basic slopes and directions
		{"ls05_s01_rd", "5px shallow line, right-down",   TYPES {1, 2, 6, 4}},
		{"lt05_s02_rd", "5px steep line, right-down",     TYPES {2, 1, 4, 6}},
		{"ld05_s03_rd", "5px 45-degree line, right-down", TYPES {3, 3, 8, 8}},
		{"ls05_s01_ru", "5px shallow line, right-up",     TYPES {1, 8, 6, 6}},
		{"lt08_s10_lu", "8px steep line, left-up",        TYPES {10, 10, 13, 2}},

		// -- boundary crossing tests (shallow lines)
		{"ls08_c31_u", "Shallow, unaligned, crosses 31/32", TYPES {28, 10, 36, 13}},
		{"ls03_c31_s", "Shallow, starts on boundary 31",    TYPES {31, 20, 34, 21}},
		{"ls03_c32_s", "Shallow, starts on boundary 32",    TYPES {32, 30, 35, 31}},
		{"ls15_c31_b", "Shallow, barely crossing 31/32",    TYPES {20, 40, 35, 45}},

		// -- boundary crossing tests (steep lines)
		// For steep lines, memory operations when crossing x-boundaries can be a bottleneck.
		{"lt08_c31_u",   "Steep, unaligned, crosses x=31/32", TYPES {30, 10, 33, 18}},
		{"lt16_c31_u",   "Steep, wider, crosses x=31/32",     TYPES {25, 5, 35, 21}},
		{"lt32_x_const", "Steep, x stays in same word",       TYPES {40, 15, 44, 47}},

		// -- long lines & multiple boundary crossings
		{"ls65_multi_u",   "Long shallow, crosses 31/32, 63/64", TYPES {1, 1, 66, 20}},
		{"lt65_multi_u",   "Long steep, crosses 31/32, 63/64",   TYPES {1, 1, 20, 66}},
		{"ld65_multi_u",   "Long 45-deg, crosses 31/32, 63/64",  TYPES {1, 1, 66, 66}},
		{"ls90_rev_multi", "Long shallow, left-up, multi-cross", TYPES {95, 40, 5, 25}},

		// -- algorithm edge cases
		{"l_slope_gt1", "Line with steep slope > 1",      TYPES {10, 10, 15, 30}}, // dy/dx = 4
		{"l_slope_lt1", "Line with shallow slope < 1",    TYPES {10, 10, 30, 15}}, // dy/dx = 0.25
		{"l_offgrid_diag", "45-deg line, off-grid start", TYPES {2, 1, 22, 21}},
	},
	// clang-format on
);

APPLY(2loop) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(2loop)(&image_, x1_, y1_, x2_, y2_, COLOR_WHITE);
	}
}

APPLY(1loop) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(1loop)(&image_, x1_, y1_, x2_, y2_, COLOR_WHITE);
	}
}
