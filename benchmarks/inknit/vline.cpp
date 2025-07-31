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

#define GROUP_NAME  draw_vline
#define APPLY(name) INKBM_FIXTURE_APPLY(name, draw_vline, d_vline)
#define FUNC_NAME(name)                                                               \
	INKNIT_INTERNAL_FUNCNAME(INKNIT_X4LSB_BASE, _INKNIT_CONCAT3(GROUP_NAME, _, name))

class d_vline: public fixture {
public:
	void load(std::any const *data) noexcept override {
		auto const *ptr = std::any_cast<TYPES>(data);
		if (ptr == nullptr) {
			INKBM_FATAL("Failed to unbind args.");
		}

		auto [x, y1, y2] = *ptr;

		x_  = x;
		y1_ = y1;
		y2_ = y2;
	}

	void setup() noexcept override {
		image_.clear(COLOR_BLACK);
	}

protected:
	fixed_image<32, 32, pixel_layout::x1lsb, pixel_format::grayscale> image_;

	uint_t x_, y1_, y2_;
};

INKBM_ARGS(
	draw_vline,
	"Test Method",
	// clang-format off
	{
		{"01px", "degenerate: 1px line (top-left)",   TYPES { 0,  0,  0}},
		{"02px", "basic: 2px line (bottom-right)",    TYPES {27, 29, 30}},
		{"03px", "basic: 3px line (top)",             TYPES { 2,  0,  2}},
		{"04px", "break-even search: 4px",            TYPES {18, 13, 17}},
		{"05px", "break-even search: 5px",            TYPES {18, 13, 17}},
		{"06px", "break-even search: 6px",            TYPES {18, 13, 18}},
		{"07px", "break-even search: 7px",            TYPES {18, 13, 19}},
		{"08px", "break-even search: 8px",            TYPES {18, 13, 20}},
		{"09px", "break-even search: 9px",            TYPES {18, 13, 21}},
		{"10px", "break-even search: 10px",           TYPES {18, 13, 22}},
		{"11px", "break-even search: 11px",           TYPES {18, 13, 23}},
		{"16px", "half: 16px line (top half)",        TYPES { 8,  3, 18}},
		{"32px", "full: 32px line (full-height)",     TYPES { 5,  0, 31}},
	},
	// clang-format on
);

APPLY(pointer) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(pointer)(&image_, x_, y1_, y2_, COLOR_WHITE);
	}
}

APPLY(pointer_unroll) {
	for (int i = 0; i < ITERATIONS; ++i) {
		FUNC_NAME(pointer_unroll)(&image_, x_, y1_, y2_, COLOR_WHITE);
	}
}
