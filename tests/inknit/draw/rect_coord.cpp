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
void subtest_draw_rect_coord(
	Image& image, point_t pt1, point_t pt2, color_t color = colors::white
) noexcept {
	image.clear(colors::black);
	image.draw_rect_coord(pt1, pt2, color);
	image.test(bind_is_pixel_on_rect_edge_with_point(pt1, pt2));
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"draw_rect_coord",
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

#define SUBCASE_INVOKE(_IX, _IY, _IW, _IH, _MSG)                            \
	do {                                                                    \
		SUBCASE(_MSG) {                                                     \
			shared::subtest_draw_rect_coord(image, {_IX, _IY}, {_IW, _IH}); \
		}                                                                   \
	} while (false)

	// 1. basic
	SUBCASE_INVOKE(0, 0, 3, 3, "basic: 4x4 rectangle at origin");
	SUBCASE_INVOKE(5, 2, 7, 3, "basic: offset 3x2 rectangle");

	// 2. degenerate
	SUBCASE_INVOKE(7, 5, 7, 5, "degenerate: 1x1 rectangle");

	// 3. edge (boundary)
	SUBCASE_INVOKE(28, 28, 31, 31, "edge: bottom-right edge");

	// 4. line
	SUBCASE_INVOKE(2, 3, 6, 3, "line: horizontal 1-row rectangle");
	SUBCASE_INVOKE(10, 5, 10, 10, "line: vertical 1-column rectangle");

	// 5. full
	SUBCASE_INVOKE(0, 0, image.width() - 1, image.height() - 1, "full: full-image rectangle");

#undef SUBCASE_INVOKE

	SUBCASE("single-column") {
		image.reset(1, 8);
		shared::subtest_draw_rect_coord(image, {0, 1}, {0, 3});
	}

	SUBCASE("single-row") {
		image.reset(8, 1);
		shared::subtest_draw_rect_coord(image, {3, 0}, {5, 0});
	}
}
