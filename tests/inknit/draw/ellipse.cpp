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
void subtest_draw_ellipse(
	Image&       image,
	std::int32_t cx,
	std::int32_t cy,
	std::int32_t rx,
	std::int32_t ry,
	color_t      color     = colors::white,
	bool         skip_test = false
) noexcept {
	image.clear(colors::black);
	image.draw_ellipse({cx, cy}, {rx, ry}, color);

	if (skip_test) {
		pixel_list const& list = make_midpoint_ellipse(cx, cy, rx, ry);
		image.test(bind_is_pixel_on_list(list));
	}
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"draw_ellipse",
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

#define SUBCASE_INVOKE(_ICX, _ICY, _IRX, _IRY, _MSG)                                         \
	INKNIT_SUBCASE_INVOKE(shared::subtest_draw_ellipse(image, _ICX, _ICY, _IRX, _IRY), _MSG)

#define SUBCASE_EXPECT_ASSERT(_ICX, _ICY, _IRX, _IRY, _MSG, _EXPECTED_MESSAGE)                     \
	INKNIT_SUBCASE_EXPECT_ASSERT(                                                                  \
		shared::subtest_draw_ellipse(image, _ICX, _ICY, _IRX, _IRY, true), _MSG, _EXPECTED_MESSAGE \
	)

	// 1. basic
	SUBCASE_INVOKE(15, 15, 12, 8, "basic: standard horizontal ellipse");
	SUBCASE_INVOKE(15, 15, 8, 12, "basic: standard vertical ellipse");
	SUBCASE_INVOKE(7, 9, 6, 4, "basic: offset ellipse");

	// 2. degenerate
	SUBCASE_INVOKE(10, 10, 10, 0, "degenerate: zero-ry ellipse (a horizontal line)");
	SUBCASE_INVOKE(10, 10, 0, 10, "degenerate: zero-rx ellipse (a vertical line)");
	SUBCASE_INVOKE(10, 10, 0, 0, "degenerate: zero-radii ellipse (a single point)");
	SUBCASE_INVOKE(15, 15, 1, 8, "degenerate: 1-rx ellipse (a thin vertical line)");
	SUBCASE_INVOKE(15, 15, 8, 1, "degenerate: 1-ry ellipse (a thin horizontal line)");

	// 3. edge
	SUBCASE_INVOKE(21, 15, 10, 8, "edge: right edge tangent ellipse");
	SUBCASE_INVOKE(15, 23, 8, 8, "edge: bottom edge tangent ellipse (circle)");
	SUBCASE_INVOKE(5, 5, 10, 8, "edge: partially off-screen ellipse");

	// 4. large
	SUBCASE_INVOKE(15, 15, 16, 12, "large: ellipse covering most of the image");

	// 5. circle-like
	SUBCASE_INVOKE(15, 15, 10, 10, "circle-like: rx == ry");

	// 6. robustness (or invalid_input)
	SUBCASE_EXPECT_ASSERT(15, 15, -1, 10, "robustness: negative rx", "ERROR: rx < 0");
	SUBCASE_EXPECT_ASSERT(15, 15, 10, -1, "robustness: negative ry", "ERROR: ry < 0");
	SUBCASE_EXPECT_ASSERT(15, 15, -5, -10, "robustness: both radii negative", "ERROR: rx < 0");
	SUBCASE_EXPECT_ASSERT(
		15, 15, 1024, 100, "robustness: rx exceeds implementation limit", "ERROR: rx > 2^10 - 1"
	);
	SUBCASE_EXPECT_ASSERT(
		15, 15, 100, 1024, "robustness: ry exceeds implementation limit", "ERROR: ry > 2^10 - 1"
	);
	SUBCASE_EXPECT_ASSERT(
		15,
		15,
		1024,
		1024,
		"robustness: both radii exceed implementation limit",
		"ERROR: rx > 2^10 - 1"
	);

#undef SUBCASE_INVOKE
#undef SUBCASE_EXPECT_ASSERT
}
