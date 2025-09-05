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
	bool         skip_test = false,
	color_t      color     = colors::white
) noexcept {
	image.clear(colors::black);
	image.draw_ellipse({cx, cy}, {rx, ry}, color);

	if (!skip_test) {
		pixel_list const& list = make_midpoint_ellipse(cx, cy, rx, ry, image.rect());
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

	std::int32_t const width  = image.width();
	std::int32_t const height = image.height();
	std::int32_t const minlen = std::min(width, height);
	std::int32_t const maxlen = std::max(width, height);

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
	SUBCASE_INVOKE(width - 11, height / 2, 10, 8, "edge: right edge tangent ellipse");
	SUBCASE_INVOKE(width / 2, height - 11, 8, 8, "edge: bottom edge tangent ellipse (circle)");
	SUBCASE_INVOKE(
		width / 4, height / 4, minlen / 2, minlen / 3, "edge: partially off-screen ellipse"
	);

	// 4. small radius
	SUBCASE_INVOKE(12, 12, 2, 3, "small: rx=2, ry=3");
	SUBCASE_INVOKE(12, 12, 3, 2, "small: rx=3, ry=2");

	// 5. large
	SUBCASE_INVOKE(width / 2, height / 2, minlen / 2, minlen / 3, "large: inscribed ellipse");

	// 6. full coverage
	SUBCASE_INVOKE(width / 2, height / 2, maxlen, maxlen, "full coverage: fills entire image");

	// 7. circle-like
	SUBCASE_INVOKE(15, 15, 10, 10, "circle-like: rx == ry");

	// 8. robust
	SUBCASE_EXPECT_ASSERT(15, 15, -1, 10, "robust: negative rx", "ERROR: rx < 0");
	SUBCASE_EXPECT_ASSERT(15, 15, 10, -1, "robust: negative ry", "ERROR: ry < 0");
	SUBCASE_EXPECT_ASSERT(15, 15, -5, -10, "robust: both radii negative", "ERROR: rx < 0");

	// 9. invalid_input
	SUBCASE_EXPECT_ASSERT(
		15, 15, 1024, 100, "invalid: rx exceeds implementation limit", "ERROR: rx > 2^10 - 1"
	);
	SUBCASE_EXPECT_ASSERT(
		15, 15, 100, 1024, "invalid: ry exceeds implementation limit", "ERROR: ry > 2^10 - 1"
	);
	SUBCASE_EXPECT_ASSERT(
		15,
		15,
		1024,
		1024,
		"invalid: both radii exceed implementation limit",
		"ERROR: rx > 2^10 - 1"
	);

#undef SUBCASE_INVOKE
#undef SUBCASE_EXPECT_ASSERT
}
