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
void subtest_draw_vline(
	Image& image, std::int32_t ix, std::int32_t iy1, std::int32_t iy2, color_t color = colors::white
) noexcept {
	image.clear(colors::black);
	image.draw_vline(ix, iy1, iy2, color);
	image.test(bind_is_pixel_on_vline(ix, iy1, iy2));
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"draw_vline",
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

#define SUBCASE_INVOKE(_IX, _IY1, _IY2, _MSG)                                       \
	INKNIT_SUBCASE_INVOKE(shared::subtest_draw_vline(image, _IX, _IY1, _IY2), _MSG)

#define SUBCASE_EXPECT_ASSERT(_IX, _IY1, _IY2, _MSG, _EXPECTED_MESSAGE)                   \
	INKNIT_SUBCASE_EXPECT_ASSERT(                                                         \
		shared::subtest_draw_vline(image, _IX, _IY1, _IY2, true), _MSG, _EXPECTED_MESSAGE \
	)

	std::int32_t const right  = image.width() - 1;
	std::int32_t const bottom = image.height() - 1;

	// 1. basic
	SUBCASE_INVOKE(2, 0, 2, "basic: short line near top");
	SUBCASE_INVOKE(17, 2, 4, "basic: short line in mid-right");
	SUBCASE_INVOKE(13, 1, 6, "basic: medium-length line");

	// 2. degenerate
	SUBCASE_INVOKE(0, 0, 0, "degenerate: top-left single-pixel line");
	SUBCASE_INVOKE(4, 3, 3, "degenerate: single-pixel line");
	SUBCASE_INVOKE(right, bottom, bottom, "degenerate: bottom-right single-pixel line");
	SUBCASE_INVOKE(10, 9, 1, "degenerate: negative height");

	// 3. full span
	SUBCASE_INVOKE(5, 0, bottom, "full: full-height line");

	// 4. edge (boundary)
	SUBCASE_INVOKE(31, 0, 7, "edge: word-aligned column");
	SUBCASE_INVOKE(right, 0, 7, "edge: rightmost column");

	// 5. mask (intra-word masks)
	SUBCASE_INVOKE(image.ppw / 4, 0, 3, "mask: first quarter");
	SUBCASE_INVOKE(image.ppw * 3 / 4, 0, 3, "mask: third quarter");
	SUBCASE_INVOKE(image.ppw - 1, 0, 3, "mask: word-boundary edge");

	// 6. robust
	SUBCASE_INVOKE(10, -5, 5, "robust: clipped at top");
	SUBCASE_INVOKE(10, bottom - 5, bottom + 5, "robust: clipped at bottom");
	SUBCASE_INVOKE(10, -10, -5, "robust: fully clipped (top)");
	SUBCASE_INVOKE(10, bottom + 5, bottom + 10, "robust: fully clipped (bottom)");

	// 7. invalid_input
	SUBCASE_EXPECT_ASSERT(-2049, 0, 10, "invalid: negative x exceeds limit", "ERROR: x < -2^11");
	SUBCASE_EXPECT_ASSERT(2048, 0, 10, "invalid: positive x exceeds limit", "ERROR: x > 2^11 - 1");
	SUBCASE_EXPECT_ASSERT(1, -2049, 10, "invalid: negative y exceeds limit", "ERROR: y1 < -2^11");
	SUBCASE_EXPECT_ASSERT(1, 10, 2048, "invalid: positive y exceeds limit", "ERROR: y2 > 2^11 - 1");

#undef SUBCASE_INVOKE

	SUBCASE("single-column") {
		image.reset(1, 8);
		shared::subtest_draw_vline(image, 0, 1, 6);
	}

	SUBCASE("single-row") {
		image.reset(8, 1);
		shared::subtest_draw_vline(image, 3, 0, 0);
	}
}
