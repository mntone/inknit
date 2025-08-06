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
void subtest_draw_line(
	Image&       image,
	std::int32_t ix1,
	std::int32_t iy1,
	std::int32_t ix2,
	std::int32_t iy2,
	color_t      color = colors::white
) noexcept {
	image.clear(colors::black);
	image.draw_line({ix1, iy1}, {ix2, iy2}, color);

	auto const list = make_bresenham_line(ix1, iy1, ix2, iy2, image.width(), image.height());
	image.test(bind_is_pixel_on_list(list));
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"draw_line",
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

#define SUBCASE_INVOKE(_IX1, _IY1, _IX2, _IY2, _MSG)                                      \
	INKNIT_SUBCASE_INVOKE(shared::subtest_draw_line(image, _IX1, _IY1, _IX2, _IY2), _MSG)

	std::int32_t const right  = image.width() - 1;
	std::int32_t const bottom = image.height() - 1;

	// 1. basic
	SUBCASE_INVOKE(2, 5, 10, 5, "basic: horizontal line (left-to-right)");
	SUBCASE_INVOKE(8, 1, 8, 6, "basic: vertical line (top-to-bottom)");
	SUBCASE_INVOKE(10, 5, 2, 5, "basic: horizontal line (right-to-left)");
	SUBCASE_INVOKE(8, 6, 8, 1, "basic: vertical line (bottom-to-top)");

	SUBCASE_INVOKE(1, 1, 5, 5, "basic: diagonal line (45°)");
	SUBCASE_INVOKE(5, 2, 7, 10, "basic: steep line (dx < dy)");
	SUBCASE_INVOKE(2, 2, 10, 4, "basic: shallow line (dx > dy)");

	// 2. degenerate
	SUBCASE_INVOKE(3, 3, 3, 3, "degenerate: single point line");

	// 3. edge
	SUBCASE_INVOKE(image.width() - 2, image.height() - 2, right, bottom, "edge: near bottom-right");

	// 4. full span
	SUBCASE_INVOKE(0, 0, right, bottom, "full: diagonal full image span");

	// 5. robust
	constexpr int cx = 8, cy = 8;
	constexpr int r = 6;
	constexpr int s = 3;

	// - Orthant I (dx > dy, x+, y+)
	SUBCASE_INVOKE(cx, cy, cx + r, cy + s, "robust: octant 1");
	SUBCASE_INVOKE(cx, cy, cx + s, cy + r, "robust: octant 2");

	// - Orthant II (dx < dy, x-, y+)
	SUBCASE_INVOKE(cx, cy, cx - s, cy + r, "robust: octant 3");
	SUBCASE_INVOKE(cx, cy, cx - r, cy + s, "robust: octant 4");

	// - Orthant III (dx > dy, x-, y-)
	SUBCASE_INVOKE(cx, cy, cx - r, cy - s, "robust: octant 5");
	SUBCASE_INVOKE(cx, cy, cx - s, cy - r, "robust: octant 6");

	// - Orthant IV (dx < dy, x+, y-)
	SUBCASE_INVOKE(cx, cy, cx + s, cy - r, "robust: octant 7");
	SUBCASE_INVOKE(cx, cy, cx + r, cy - s, "robust: octant 8");

	// - Max coordinate range
	SUBCASE_INVOKE(-2048, -2048, 2047, 2047, "robust: max coordinate range");
	SUBCASE_INVOKE(2047, -2048, -2048, 2047, "robust: max coordinate range, opposite");

#undef SUBCASE_INVOKE

	SUBCASE("single-column") {
		image.reset(1, 8);
		shared::subtest_draw_line(image, 0, 1, 0, 6);
	}

	SUBCASE("single-row") {
		image.reset(8, 1);
		shared::subtest_draw_line(image, 3, 0, 6, 0);
	}
}
