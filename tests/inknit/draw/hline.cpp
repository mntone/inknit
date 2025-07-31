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

namespace inknit::tests::shared {

template<typename Image>
	requires std::convertible_to<Image, inknit::details::image_primitive>
void subtest_draw_hline(
	Image& image, uint_t ix1, uint_t ix2, uint_t iy, color_t color = colors::white
) noexcept {
	image.clear(colors::black);
	image.draw_hline(ix1, ix2, iy, color);
	image.test(bind_is_pixel_on_hline(ix1, ix2, iy));
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"draw_hline",
	Image,
	test_image<pixel_layout::x1, pixel_format::grayscale>,
	test_image<pixel_layout::x1lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x2, pixel_format::grayscale>,
	test_image<pixel_layout::x2lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x4, pixel_format::grayscale>,
	test_image<pixel_layout::x4lsb, pixel_format::grayscale>
) {
	Image image;
	image.reset(4 * image.ppw, 4 * image.ppw);

#define SUBCASE_INVOKE(_IX1, _IX2, _IY, _MSG)                   \
	do {                                                        \
		SUBCASE(_MSG) {                                         \
			shared::subtest_draw_hline(image, _IX1, _IX2, _IY); \
		}                                                       \
	} while (false)

	uint_t const right  = image.width() - 1;
	uint_t const bottom = image.height() - 1;

	// 1. basic
	SUBCASE_INVOKE(0, 7, 1, "basic: short line near top");
	SUBCASE_INVOKE(14, 17, 6, "basic: short line in mid-bottom");
	SUBCASE_INVOKE(11, 23, 2, "basic: medium-length line");

	// 2. degenerate
	SUBCASE_INVOKE(0, 0, 0, "degenerate: top-left single-pixel line");
	SUBCASE_INVOKE(3, 3, 5, "degenerate: single-pixel line");
	SUBCASE_INVOKE(right, right, bottom, "degenerate: bottom-right single-pixel line");

	// 3. full span
	SUBCASE_INVOKE(0, right, 5, "full: full-width line");

	// 4. edge (boundary)
	SUBCASE_INVOKE(0, right, bottom, "edge: bottommost row");

	// 5. alignment
	SUBCASE_INVOKE(0, image.ppw - 1, 6, "align: word-aligned full-width line");
	SUBCASE_INVOKE(5, 10, 2, "align: partial single-word span");

	// 6. mask (intra-word masks)
	SUBCASE_INVOKE(0, image.ppw / 4 - 1, 2, "mask: first quarter");
	SUBCASE_INVOKE(image.ppw / 2, image.ppw * 3 / 4 - 1, 2, "mask: third quarter");

	// 7. crossing
	SUBCASE_INVOKE(image.ppw - 1, image.ppw, 2, "cross: minimal cross-word span (boundary edge)");
	SUBCASE_INVOKE(image.ppw - 1, image.ppw, 4, "cross: 1-pixel overlap");
	SUBCASE_INVOKE(10, 20, 3, "cross: spans multiple words");

#undef SUBCASE_INVOKE

	SUBCASE("single-column") {
		image.reset(1, 8);
		shared::subtest_draw_hline(image, 0, 0, 1);
	}

	SUBCASE("single-row") {
		image.reset(8, 1);
		shared::subtest_draw_hline(image, 3, 6, 0);
	}
}
