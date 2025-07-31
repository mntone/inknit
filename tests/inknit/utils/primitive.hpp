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

#pragma once

#include <functional>  // bind
#include <vector>      // vector

#include "inknit.hpp"

namespace inknit::tests {

using pixel_list = std::vector<point_t>;

// indexed color for test
namespace colors {

	constexpr color_t black  = 0;
	constexpr color_t white  = 1;
	constexpr color_t red    = 2;
	constexpr color_t yellow = 3;
	constexpr color_t orange = 4;
	constexpr color_t green  = 5;
	constexpr color_t blue   = 6;

}  // namespace colors

namespace details {

	template<typename Float, typename G>
	constexpr auto compose(Float f, G g) {
		return [=](auto&&...args) constexpr {
			return f(g(std::forward<decltype(args)>(args)...));
		};
	}

	constexpr color_t boolean_to_color(bool flag, color_t trueColor = colors::white) noexcept {
		return flag ? trueColor : colors::black;
	}

	template<typename Number>
		requires std::is_arithmetic_v<Number>
	constexpr auto abs(Number x) noexcept {
		return x < 0 ? -x : x;
	}

}  // namespace details

namespace is_pixel_on {

	INKNIT_NODISCARD
	static constexpr bool point(uint_t x, uint_t y, uint_t ix, uint_t iy) noexcept {
		return x == ix && y == iy;
	}

	INKNIT_NODISCARD
	static constexpr bool hline(uint_t x, uint_t y, uint_t ix1, uint_t ix2, uint_t iy) noexcept {
		return ix1 <= x && x <= ix2 && y == iy;
	}

	INKNIT_NODISCARD
	static constexpr bool vline(uint_t x, uint_t y, uint_t ix, uint_t iy1, uint_t iy2) noexcept {
		return x == ix && iy1 <= y && y <= iy2;
	}

	INKNIT_NODISCARD
	static constexpr bool rect_with_point(uint_t x, uint_t y, point_t ipt1, point_t ipt2) {
		auto [x1, y1] = ipt1;
		auto [x2, y2] = ipt2;
		return (x1 <= x && x <= x2) && (y1 <= y && y <= y2);
	}

	INKNIT_NODISCARD
	static constexpr bool rect(uint_t x, uint_t y, point_t ipt, size_t isz) {
		auto [width, height] = isz;
		if (width == 0 || height == 0) {
			return false;
		}

		auto [left, top] = ipt;
		uint_t right     = left + width - 1;
		uint_t bottom    = top + height - 1;
		return rect_with_point(x, y, ipt, {right, bottom});
	}

	INKNIT_NODISCARD
	static constexpr bool rect_edge_with_point(uint_t x, uint_t y, point_t ipt1, point_t ipt2) {
		auto [x1, y1] = ipt1;
		auto [x2, y2] = ipt2;
		return (y == y1 || y == y2) && (x1 <= x && x <= x2)
			|| (y1 <= y && y <= y2) && (x == x1 || x == x2);
	}

	INKNIT_NODISCARD
	static constexpr bool rect_edge(uint_t x, uint_t y, point_t ipt, size_t isz) {
		auto [width, height] = isz;
		if (width == 0 || height == 0) {
			return false;
		}

		auto [left, top] = ipt;
		uint_t right     = left + width - 1;
		uint_t bottom    = top + height - 1;
		return rect_edge_with_point(x, y, ipt, {right, bottom});
	}

	INKNIT_NODISCARD
	static constexpr bool list(uint_t x, uint_t y, pixel_list const& il) noexcept {
		for (point_t ipt : il) {
			auto [ix, iy] = ipt;
			if (x == ix && y == iy) {
				return true;
			}
		}
		return false;
	}

}  // namespace is_pixel_on

static constexpr auto bind_all_pixels(color_t trueColor = colors::white) noexcept {
	return [trueColor](uint_t, uint_t) noexcept {
		return trueColor;
	};
}

INKNIT_NODISCARD
static constexpr auto
bind_is_pixel_on_point(uint_t ix, uint_t iy, color_t trueColor = colors::white) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::point, _1, _2, ix, iy)
	);
}

INKNIT_NODISCARD
static constexpr auto bind_is_pixel_on_hline(
	uint_t ix1, uint_t ix2, uint_t iy, color_t trueColor = colors::white
) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::hline, _1, _2, ix1, ix2, iy)
	);
}

INKNIT_NODISCARD
static constexpr auto bind_is_pixel_on_vline(
	uint_t ix, uint_t iy1, uint_t iy2, color_t trueColor = colors::white
) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::vline, _1, _2, ix, iy1, iy2)
	);
}

INKNIT_NODISCARD
static constexpr auto bind_is_pixel_on_rect_with_point(
	point_t ipt1, point_t ipt2, color_t trueColor = colors::white
) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::rect_with_point, _1, _2, ipt1, ipt2)
	);
}

INKNIT_NODISCARD
static constexpr auto
bind_is_pixel_on_rect(point_t ipt, size_t isz, color_t trueColor = colors::white) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::rect, _1, _2, ipt, isz)
	);
}

INKNIT_NODISCARD
static constexpr auto bind_is_pixel_on_rect_edge_with_point(
	point_t ipt1, point_t ipt2, color_t trueColor = colors::white
) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::rect_edge_with_point, _1, _2, ipt1, ipt2)
	);
}

INKNIT_NODISCARD
static constexpr auto
bind_is_pixel_on_rect_edge(point_t ipt, size_t isz, color_t trueColor = colors::white) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::rect_edge, _1, _2, ipt, isz)
	);
}

INKNIT_NODISCARD
static constexpr auto
bind_is_pixel_on_list(pixel_list const& il, color_t trueColor = colors::white) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::list, _1, _2, il)
	);
}

INKNIT_NODISCARD
constexpr pixel_list make_bresenham_line(int x1, int y1, int x2, int y2) noexcept {
	pixel_list points;

	bool steep = abs(x2 - x1) < abs(y2 - y1);
	if (steep) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	int const dx    = x2 - x1;
	int const dy    = abs(y2 - y1);
	int       error = dx / 2;
	int       ystep = y1 < y2 ? 1 : -1;
	int       y     = y1;

	for (int x = x1; x <= x2; ++x) {
		if (steep) {
			points.emplace_back(y, x);
		} else {
			points.emplace_back(x, y);
		}

		error -= dy;
		if (error <= 0) {
			y += ystep;
			error += dx;
		}
	}

	return points;
}

namespace details {

	constexpr void add_circle_points(pixel_list& points, int cx, int cy, int dx, int dy) noexcept {
		points.emplace_back(cx + dx, cy + dy);
		points.emplace_back(cx - dx, cy + dy);
		points.emplace_back(cx + dx, cy - dy);
		points.emplace_back(cx - dx, cy - dy);
		points.emplace_back(cx + dy, cy + dx);
		points.emplace_back(cx - dy, cy + dx);
		points.emplace_back(cx + dy, cy - dx);
		points.emplace_back(cx - dy, cy - dx);
	}

}  // namespace details

template<std::integral I>
INKNIT_NODISCARD constexpr pixel_list make_midpoint_circle(I cx, I cy, I radius) noexcept {
	pixel_list points;
	if (radius < 0) {
		return points;
	}
	if (radius == 0) {
		points.emplace_back(cx, cy);
		return points;
	}

	std::int32_t const int_cx     = static_cast<std::int32_t>(cx);
	std::int32_t const int_cy     = static_cast<std::int32_t>(cy);
	std::int32_t const int_radisu = static_cast<std::int32_t>(radius);

	std::int32_t dx = 0;
	std::int32_t dy = int_radisu;
	std::int32_t d  = 1 - int_radisu;

	do {
		details::add_circle_points(points, int_cx, int_cy, dx, dy);

		++dx;
		if (d < 0) {
			d += 2 * dx + 1;
		} else {
			--dy;
			d += 2 * (dx - dy) + 1;
		}
	} while (dx <= dy);
	return points;
}

}  // namespace inknit::tests
