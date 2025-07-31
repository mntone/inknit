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

#define TYPES      std::tuple<uint_t, uint_t, uint_t>
#define ITERATIONS DEFAULT_ITERATIONS

#define GROUP_NAME  draw_hline
#define APPLY(name) INKBM_FIXTURE_APPLY(name, draw_hline, d_hline)
#define FUNC_NAME(name)                                                               \
	INKNIT_INTERNAL_FUNCNAME(INKNIT_X1LSB_BASE, _INKNIT_CONCAT3(GROUP_NAME, _, name))

class d_hline: public fixture {
public:
	void load(std::any const *data) noexcept override {
		auto const *ptr = std::any_cast<TYPES>(data);
		if (ptr == nullptr) {
			INKBM_FATAL("Failed to unbind args.");
		}

		auto [x1, x2, y] = *ptr;

		x1_ = x1;
		x2_ = x2;
		y_  = y;
	}

	void setup() noexcept override {
		image_.clear(COLOR_BLACK);
	}

protected:
	fixed_image<128, 8, pixel_layout::x1lsb, pixel_format::grayscale> image_;

	uint_t x1_, x2_, y_;
};

INKBM_ARGS(
	draw_hline,
	"Test Method",
	// clang-format off
	{
		{"l03_s00", "3px line, start at 0",           TYPES { 0,  2, 1}},
		{"l03_s16", "3px line, start at 16",          TYPES {16, 18, 3}},
		{"l04_s27", "4px line, start at 27",          TYPES {27, 30, 4}},
		{"l01_s00", "1px line, start at 0",           TYPES { 0,  0, 0}},
		{"l01_s63", "1px line, start at 63",          TYPES {63, 63, 7}},

		{"l21_s50", "long line, unaligned start/end", TYPES {50, 70, 5}},
		{"l15_s20", "line barely crossing boundary",  TYPES {20, 34, 7}},

		{"l32_s00", "32px line (fills one word)",     TYPES { 0, 31, 6}},
		{"l64_s00", "64px line (fills two words)",    TYPES { 0, 63, 6}},

		{"l05_c30", "5px line, cross-boundary at 30", TYPES {30, 34, 6}},
		{"l06_c30", "6px line, cross-boundary at 30", TYPES {30, 35, 6}},
		{"l07_c30", "7px line, cross-boundary at 30", TYPES {30, 36, 6}},
		{"l08_c30", "8px line, cross-boundary at 30", TYPES {30, 37, 6}},
	},
	// clang-format on
);

APPLY(arith) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(arith)(&image_, x1_, x2_, y_, COLOR_WHITE);
	}
}

APPLY(lut) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(lut)(&image_, x1_, x2_, y_, COLOR_WHITE);
	}
}
