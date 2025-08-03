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

#define TYPES      std::tuple<std::int32_t, std::int32_t, std::int32_t>
#define ITERATIONS DEFAULT_ITERATIONS

#define GROUP_NAME  draw_circle
#define APPLY(name) INKBM_FIXTURE_APPLY(name, draw_circle, d_circle)
#define FUNC_NAME() INKNIT_PUBLIC_FUNCNAME(INKNIT_X1LSB_BASE, GROUP_NAME)

class d_circle: public fixture {
public:
	void load(std::any const *data) noexcept override {
		auto const *ptr = std::any_cast<TYPES>(data);
		if (ptr == nullptr) {
			INKBM_FATAL("Failed to unbind args.");
		}

		auto [cx, cy, radius] = *ptr;

		cx_     = cx;
		cy_     = cy;
		radius_ = radius;
	}

	void setup() noexcept override {
		image_.clear(COLOR_BLACK);
	}

protected:
	fixed_image<128, 80, pixel_layout::x1lsb, pixel_format::grayscale> image_;

	std::int32_t cx_, cy_, radius_;
};

INKBM_ARGS(
	draw_circle,
	"Test Method",
	// clang-format off
	{
		// {"short_name", "description", TYPES {cx, cy, radius}},

		// -- basic circles
		{"cr05_small",  "5px radius circle, offset",  TYPES {10, 12,  5}},
		{"cr15_medium", "15px radius circle, offset", TYPES {20, 22, 15}},
		{"cr35_large",  "35px radius large circle",   TYPES {40, 40, 35}},

		// -- boundary crossing tests
		// These tests check performance when a circle's pixels cross 32-bit word boundaries,
		// which can be a bottleneck for memory access.
		{"cr10_bnd_t_y31", "10px radius, bottom tangent to y=31", TYPES {20, 21, 10}},
		{"cr10_bnd_x_y32", "10px radius, bottom crosses y=32",    TYPES {20, 22, 10}},
		{"cr10_bnd_t_x31", "10px radius, right tangent to x=31",  TYPES {21, 20, 10}},
		{"cr10_bnd_x_x32", "10px radius, right crosses x=32",     TYPES {22, 20, 10}},
		{"cr16_bnd_span",  "16px radius, center on boundary",     TYPES {32, 32, 16}},

		// -- clipping & edge cases
		{"c_r0",         "Zero radius circle (single point)",  TYPES {10, 10,  0}},
		{"c_r1",         "1-pixel radius circle",              TYPES {12, 12,  1}},
		{"cr20_q1_clip", "20px radius, center in Q1, clipped", TYPES { 8,  8, 20}},
		//{"cr30_most_off", "30px radius, mostly off-screen",    TYPES {-15, -15, 30}},
	},
	// clang-format on
);

APPLY(midpoint) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME()(&image_, cx_, cy_, radius_, COLOR_WHITE);
	}
}
