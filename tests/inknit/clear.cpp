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

#include "inknit_test.hpp"

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"clear",
	Image,
	test_image<pixel_layout::x1, pixel_format::grayscale>,
	test_image<pixel_layout::x1lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x2, pixel_format::grayscale>,
	test_image<pixel_layout::x2lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x4, pixel_format::grayscale>,
	test_image<pixel_layout::x4lsb, pixel_format::grayscale>
) {
	color_t const color = colors::white;

	Image image;

	SUBCASE("full image") {
		image.reset(32, 4);
		image.clear(color);
		image.test(bind_all_pixels(color));
	}

	if (image.bpp > 1) {
		SUBCASE("full image with another color") {
			image.reset(32, 4);
			image.clear(colors::red);
			image.test(bind_all_pixels(colors::red));
		}
	}

	SUBCASE("single-column") {
		image.reset(1, 8);
		image.clear(color);
		image.test(bind_all_pixels(color));
	}

	SUBCASE("single-row") {
		image.reset(8, 1);
		image.clear(color);
		image.test(bind_all_pixels(color));
	}
}
