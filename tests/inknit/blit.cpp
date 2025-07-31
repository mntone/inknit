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
void subtest_blit(
	Image& dst,
	uint_t dx,
	uint_t dy,
	uint_t dw,
	uint_t dh,
	Image& src,
	uint_t sx,
	uint_t sy,
	uint_t sw,
	uint_t sh,
	uint_t width,
	uint_t height
) noexcept {
	src.reset(sw, sh);
	src.clear(colors::white);

	dst.reset(dw, dh);
	dst.clear(colors::black);
	dst.blit(dx, dy, src, sx, sy, width, height);
	dst.test(bind_is_pixel_on_rect({dx, dy}, {width, height}));
}

}  // namespace inknit::tests::shared

using namespace inknit;
using namespace inknit::tests;

TEST_CASE_TEMPLATE(
	"blit",
	Image,
	test_image<pixel_layout::x1, pixel_format::grayscale>,
	test_image<pixel_layout::x1lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x2, pixel_format::grayscale>,
	test_image<pixel_layout::x2lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x4, pixel_format::grayscale>,
	test_image<pixel_layout::x4lsb, pixel_format::grayscale>
) {
	Image src, dst;
	src.reset(4 * src.ppw, 4 * src.ppw);
	dst.reset(4 * dst.ppw, 4 * dst.ppw);

#define SUBCASE_INVOKE(_IDX, _IDY, _IDW, _IDH, _ISX, _ISY, _ISW, _ISH, _IW, _IH, _MSG) \
	do {                                                                               \
		SUBCASE(_MSG) {                                                                \
			shared::subtest_blit(                                                      \
				dst, _IDX, _IDY, _IDW, _IDH, src, _ISX, _ISY, _ISW, _ISH, _IW, _IH     \
			);                                                                         \
		}                                                                              \
	} while (false)

	// SUBCASE_INVOKE(IDX, IDY, IDW, IDH, ISX, ISY, ISW, ISH, IW, IH, MSG)
	// IDX, IDY: Destination top-left coordinate (X, Y)
	// IDW, IDH: Destination image dimensions (Width, Height)
	// ISX, ISY: Source top-left coordinate (X, Y)
	// ISW, ISH: Source image dimensions (Width, Height)
	// IW, IH:  Blit region dimensions (Width, Height)
	// MSG:     Description of the test case

	uint_t const ppw = src.ppw;
	uint_t const sw  = src.width();
	uint_t const sh  = src.height();
	uint_t const smx = sw / 2;
	uint_t const smy = sh / 2;

	uint_t const dw  = dst.width();
	uint_t const dh  = dst.height();
	uint_t const dr  = dw - 1;
	uint_t const db  = dh - 1;
	uint_t const dmx = dw / 2;
	uint_t const dmy = dh / 2;

	uint_t const mw = std::min(sw, dw);
	uint_t const mh = std::min(sh, dh);

	// clang-format off

	// 1. basic
	// Simple rectangular copies fully contained within the image.
	SUBCASE_INVOKE(5, 5, dw, dh, 5, 5, sw, sh, 8, 8, "basic: small square blit");
	SUBCASE_INVOKE(dmx, dmy, dw, dh, 10, 10, sw, sh, ppw, 16, "basic: medium rect blit");
	SUBCASE_INVOKE(1, 1, dw, dh, smx, smy, sw, sh, ppw + 3, 5, "basic: wide unaligned blit");
	SUBCASE_INVOKE(dmx, 1, dw, dh, 1, smy, sw, sh, 5, ppw + 3, "basic: tall unaligned blit");

	// 2. degenerate
	// 1x1 and zero-size blits to check algorithm robustness.
	SUBCASE_INVOKE(dmx, dmy, dw, dh, smx, smy, sw, sh, 1, 1, "degenerate: 1x1 blit");
	SUBCASE_INVOKE(0, 0, dw, dh, 0, 0, sw, sh, 0, 10, "degenerate: zero width");
	SUBCASE_INVOKE(0, 0, dw, dh, 0, 0, sw, sh, 10, 0, "degenerate: zero height");

	// 3. edge and full span
	// Operations that are perfectly aligned with the image edges.
	SUBCASE_INVOKE(0, 0, dw, dh, 0, 0, sw, sh, mw, mh, "full span: entire image");
	SUBCASE_INVOKE(0, db - 10, dw, dh, 0, 0, sw, sh, mw, 10, "edge: blit to bottom edge");
	SUBCASE_INVOKE(dr - 10, 0, dw, dh, 0, 0, sw, sh, 10, mh, "edge: blit to right edge");

	// 4. alignment (testing the word-aligned fast path)
	// Cases where sx, dx, and width are all multiples of ppw to test the fastest path.
	SUBCASE_INVOKE(ppw, 5, dw, dh, 0, 5, sw, sh, ppw, 16, "align: fully aligned fast path");
	SUBCASE_INVOKE(0, 5, dw, dh, ppw, 5, sw, sh, ppw, 16, "align: fully aligned fast path (rev)");

	// 5. masking (intra-word)
	// Blits that start and end entirely within a single memory word to test Head/Tail logic.
	SUBCASE_INVOKE(5, 5, dw, dh, 5, 5, sw, sh, ppw / 2, 4, "mask: start and end within one word");
	SUBCASE_INVOKE(ppw + 2, 5, dw, dh, 2, 5, sw, sh, ppw / 2, 4, "mask: different start/end words, but intra-word");

	// 6. boundary crossing
	// Performance-sensitive cases that cross memory word boundaries.
	SUBCASE_INVOKE(ppw - 8, 5, dw, dh, ppw - 8, 5, sw, sh, 16, 16, "cross: aligned src/dst, crosses boundary");
	SUBCASE_INVOKE(ppw, 5, dw, dh, ppw - 5, 5, sw, sh, 10, 16, "cross: unaligned src to aligned dst");
	SUBCASE_INVOKE(ppw - 5, 5, dw, dh, ppw, 5, sw, sh, 10, 16, "cross: aligned src to unaligned dst");
	SUBCASE_INVOKE(ppw - 5, 5, dw, dh, ppw - 3, 5,sw, sh, 10, 16, "cross: unaligned src to unaligned dst");
	SUBCASE_INVOKE(5, 5, dw, dh, 5, 5, sw, sh, ppw * 2, 4, "cross: spans multiple words");

	// 7. clipping
	// Cases where the blit region extends beyond the source or destination boundaries.
	//SUBCASE_INVOKE(-5, -5, dw, dh, 0, 0, sw, sh, 16, 16, "clip: dst top-left");
	//SUBCASE_INVOKE(dr - 10, db - 10, dw, dh, 0, 0, sw, sh, 16, 16, "clip: dst bottom-right");
	//SUBCASE_INVOKE(5, 5, dw, dh, -5, -5, sw, sh, 16, 16, "clip: src top-left");
	//SUBCASE_INVOKE(5, 5, dw, dh, dr - 10, db - 10, sw, sh, 16, 16, "clip: src bottom-right");
	//SUBCASE_INVOKE(dw, dh, dw, dh, 0, 0, sw, sh, 16, 16, "clip: fully outside dst");

	// 8. overlapping
	// Blit-specific tests to verify correct copy direction control.
	SUBCASE_INVOKE(10, 10, dw, dh, 8, 8, sw, sh, 16, 16, "overlap: right-down");
	SUBCASE_INVOKE(8, 8, dw, dh, 10, 10, sw, sh, 16, 16, "overlap: left-up");
	SUBCASE_INVOKE(10, 8, dw, dh, 8, 10, sw, sh, 16, 16, "overlap: right-up");
	SUBCASE_INVOKE(8, 10, dw, dh, 10, 8, sw, sh, 16, 16, "overlap: left-down");

	// clang-format on

#undef SUBCASE_INVOKE
}

TEST_CASE_TEMPLATE(
	"blit_old",
	Image,
	test_image<pixel_layout::x1, pixel_format::grayscale>,
	test_image<pixel_layout::x1lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x2, pixel_format::grayscale>,
	test_image<pixel_layout::x2lsb, pixel_format::grayscale>,
	test_image<pixel_layout::x4, pixel_format::grayscale>,
	test_image<pixel_layout::x4lsb, pixel_format::grayscale>
) {
	Image dst, src;

	SUBCASE("copy full image") {
		src.reset(4, 4);
		src.clear(colors::black);
		src.draw_rect({0, 0}, {4, 4}, colors::white);

		dst.reset(4, 4);
		dst.clear(colors::black);
		dst.blit(0, 0, src, 0, 0, 4, 4);
		dst.test(bind_is_pixel_on_rect_edge({0, 0}, {4, 4}));
	}

	SUBCASE("copy partial region") {
		src.reset(8, 8);
		src.clear(colors::black);
		src.draw_line({0, 0}, {7, 7}, colors::white);

		dst.reset(4, 4);
		dst.clear(colors::black);
		dst.blit(0, 0, src, 2, 2, 4, 4);

		auto const list = make_bresenham_line(0, 0, 4, 4);
		dst.test(bind_is_pixel_on_list(list));
	}

	SUBCASE("copy single pixel") {
		src.reset(4, 4);
		src.clear(colors::black);
		src.draw_line({0, 0}, {4, 4}, colors::white);

		dst.reset(4, 4);
		dst.clear(colors::black);
		dst.blit(0, 0, src, 2, 2, 1, 1);
		dst.test(bind_is_pixel_on_point(0, 0));
	}

	SUBCASE("copy onto offset destination") {
		src.reset(4, 4);
		src.clear(colors::white);

		dst.reset(8, 8);
		dst.clear(colors::black);
		dst.blit(2, 3, src, 0, 0, 4, 4);
		dst.test(bind_is_pixel_on_rect({2, 3}, {4, 4}));
	}

	SUBCASE("copy single column") {
		src.reset(8, 8);
		src.clear(colors::white);

		dst.reset(1, 8);
		dst.clear(colors::black);
		dst.blit(0, 0, src, 0, 0, 1, 8);
		dst.test(bind_all_pixels(colors::white));
	}

	SUBCASE("copy single row") {
		src.reset(8, 8);
		src.clear(colors::white);

		dst.reset(8, 1);
		dst.clear(colors::black);
		dst.blit(0, 0, src, 0, 0, 8, 1);
		dst.test(bind_all_pixels(colors::white));
	}
}
