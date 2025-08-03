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
void subtest_draw_circle(
	Image&       image,
	std::int32_t cx,
	std::int32_t cy,
	std::int32_t radius,
	color_t      color = colors::white
) noexcept {
	image.clear(colors::black);
	image.draw_circle({cx, cy}, radius, color);

	pixel_list const& list = make_midpoint_circle(cx, cy, radius);
	image.test(bind_is_pixel_on_list(list));
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"draw_circle",
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

#define SUBCASE_INVOKE(_ICX, _ICY, _IR, _MSG)                    \
	do {                                                         \
		SUBCASE(_MSG) {                                          \
			shared::subtest_draw_circle(image, _ICX, _ICY, _IR); \
		}                                                        \
	} while (false)

	// 1. basic
	SUBCASE_INVOKE(15, 15, 10, "basic: standard circle at center");
	SUBCASE_INVOKE(5, 8, 4, "basic: offset circle");

	// 2. degenerate
	SUBCASE_INVOKE(10, 10, 0, "degenerate: zero-radius circle (a single point)");
	SUBCASE_INVOKE(20, 5, 1, "degenerate: 1-radius circle");

	// 3. edge
	SUBCASE_INVOKE(21, 21, 10, "edge: bottom-right edge tangent circle");
	SUBCASE_INVOKE(5, 5, 10, "edge: partially off-screen circle");

	// 4. large
	SUBCASE_INVOKE(15, 15, 16, "large: circle covering most of the image");

#undef SUBCASE_INVOKE
}
