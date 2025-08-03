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
void subtest_draw_point(
	Image& image, std::int32_t ix, std::int32_t iy, color_t color = colors::white
) noexcept {
	image.clear(colors::black);
	image.draw_point(ix, iy, color);
	image.test(bind_is_pixel_on_point(ix, iy));
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"draw_point",
	Image,
	test_image<pixel_layout::x1, pixel_format::grayscale>,
	test_image<pixel_layout::x1lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x2, pixel_format::grayscale>,
	test_image<pixel_layout::x2lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x4, pixel_format::grayscale>,
	test_image<pixel_layout::x4lsb, pixel_format::grayscale>
) {
	Image image;
	image.reset(4 * image.ppw, 4);

#define SUBCASE_INVOKE(_IX, _IY, _MSG)                   \
	do {                                                 \
		SUBCASE(_MSG) {                                  \
			shared::subtest_draw_point(image, _IX, _IY); \
		}                                                \
	} while (false)

	// 1. bit offset
	SUBCASE_INVOKE(0, 0, "bit offset: word-start (bit offset 0)");
	// clang-format off
	if (image.ppw >= 4) {
		if (image.ppw >= 8) {
			if (image.ppw >= 16) {
				if (image.ppw >= 32) {
					SUBCASE_INVOKE(image.ppw / 16 - 1, 0, "bit offset: 1/16 end");
					SUBCASE_INVOKE(image.ppw / 16    , 0, "bit offset: 1/16 position");
				}
				SUBCASE_INVOKE(image.ppw / 8 - 1, 0, "bit offset: 1/8 end");
				SUBCASE_INVOKE(image.ppw / 8    , 0, "bit offset: 1/8 position");
			}
			SUBCASE_INVOKE(image.ppw / 4 - 1, 0, "bit offset: 1/4 end");
			SUBCASE_INVOKE(image.ppw / 4    , 0, "bit offset: 1/4 position");
		}
		SUBCASE_INVOKE(image.ppw / 2 - 1, 0, "bit offset: mid-word end");
		SUBCASE_INVOKE(image.ppw / 2    , 0, "bit offset: mid-word");
	}
	// clang-format on
	SUBCASE_INVOKE(image.ppw - 2, 0, "bit offset: near word-end");
	SUBCASE_INVOKE(image.ppw - 1, 0, "bit offset: word-end (last bit)");

	// 2. row
	SUBCASE_INVOKE(0, 1, "row: leftmost in second row");
	SUBCASE_INVOKE(image.width() / 2, image.height() - 1, "row: bottom row, center column");

	// 3. edge (boundary)
	SUBCASE_INVOKE(image.ppw, 0, "edge: first pixel in second word");
	SUBCASE_INVOKE(image.width() - 1, 0, "edge: last column");

#undef SUBCASE_INVOKE
}


TEST_CASE_TEMPLATE(
	"draw_point: overwrite pixel at same location",
	Image,
	test_image<pixel_layout::x1, pixel_format::grayscale>,
	test_image<pixel_layout::x1lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x2lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x4lsb, pixel_format::grayscale>
) {
	Image image;
	image.reset(4 * image.ppw, 4);
	image.draw_point(5, 3, image.bpp == 1 ? colors::black : colors::red);
	image.draw_point(5, 3, colors::white);
	image.test(bind_is_pixel_on_point(5, 3));
}

TEST_CASE_TEMPLATE(
	"draw_point: draw multiple pixels in same word",
	Image,
	test_image<pixel_layout::x1, pixel_format::grayscale>,
	test_image<pixel_layout::x1lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x2lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x4lsb, pixel_format::grayscale>
) {
	Image image;
	image.reset(4 * image.ppw, 4);

	color_t const colorA = image.bpp == 1 ? colors::black : colors::red;
	color_t const colorB = image.bpp == 1 ? colors::black : colors::yellow;

	image.draw_point(3, 0, colorA);
	image.draw_point(4, 0, colorB);
	image.test([=](std::int32_t x, std::int32_t y) noexcept {
		if (x == 3 && y == 0) return colorA;
		if (x == 4 && y == 0) return colorB;
		return colors::black;
	});
}

TEST_CASE_TEMPLATE(
	"draw_point: with each color",
	Image,
	test_image<pixel_layout::x1, pixel_format::grayscale>,
	test_image<pixel_layout::x1lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x2lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x4lsb, pixel_format::grayscale>
) {
	Image image;
	image.reset(4, 4);

	std::vector<color_t> colors {
		colors::black,
		colors::white,
	};
	if (image.format() == pixel_format::index) {
		if (image.bpp >= 2) {
			colors.emplace_back(colors::red);
			colors.emplace_back(colors::yellow);
		}
	}

	for (auto&& color : colors) {
		CAPTURE(color);
		image.draw_point(0, 0, color);
		image.test(bind_is_pixel_on_point(0, 0, color));
	}
}
