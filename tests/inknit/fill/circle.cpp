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
void subtest_fill_circle(
	Image&       image,
	std::int32_t cx,
	std::int32_t cy,
	std::int32_t radius,
	bool         skip_test = false,
	color_t      color     = colors::white
) noexcept {
	image.clear(colors::black);
	image.fill_circle({cx, cy}, radius, color);

	if (!skip_test) {
		pixel_list const& list = make_midpoint_filled_circle(cx, cy, radius, image.rect());
		image.test(bind_is_pixel_on_list(list));
	}
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"fill_circle",
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

#define SUBCASE_INVOKE(_ICX, _ICY, _IR, _MSG)                                        \
	INKNIT_SUBCASE_INVOKE(shared::subtest_fill_circle(image, _ICX, _ICY, _IR), _MSG)

#define SUBCASE_EXPECT_ASSERT(_ICX, _ICY, _IR, _MSG, _EXPECTED_MESSAGE)                    \
	INKNIT_SUBCASE_EXPECT_ASSERT(                                                          \
		shared::subtest_fill_circle(image, _ICX, _ICY, _IR, true), _MSG, _EXPECTED_MESSAGE \
	)

	std::int32_t const width  = image.width();
	std::int32_t const height = image.height();
	std::int32_t const minlen = std::min(width, height);
	std::int32_t const maxlen = std::max(width, height);

	// 1. basic
	SUBCASE_INVOKE(15, 15, 10, "basic: standard circle at center");
	SUBCASE_INVOKE(5, 8, 4, "basic: offset circle");

	// 2. degenerate
	SUBCASE_INVOKE(10, 10, 0, "degenerate: zero-radius circle (a single point)");
	SUBCASE_INVOKE(20, 5, 1, "degenerate: 1-radius circle");

	// 3. edge
	SUBCASE_INVOKE(width - 11, height / 2, 10, "edge: right edge tangent");
	SUBCASE_INVOKE(width / 2, height - 11, 10, "edge: bottom edge tangent");
	SUBCASE_INVOKE(width / 4, height / 4, minlen / 2, "edge: partially off-screen");

	// 4. small radius
	SUBCASE_INVOKE(12, 12, 2, "small: radius 2");
	SUBCASE_INVOKE(12, 12, 3, "small: radius 3");

	// 5. large
	SUBCASE_INVOKE(width / 2, height / 2, minlen / 2, "large: inscribed circle");

	// 6. full coverage
	SUBCASE_INVOKE(width / 2, height / 2, maxlen, "full coverage: fills entire image");

	// 7. robustness (or invalid_input)
	SUBCASE_EXPECT_ASSERT(15, 15, -1, "robustness: negative radius", "ERROR: radius < 0");
	SUBCASE_EXPECT_ASSERT(15, 15, -10, "robustness: large negative radius", "ERROR: radius < 0");

#undef SUBCASE_INVOKE
#undef SUBCASE_EXPECT_ASSERT
}
