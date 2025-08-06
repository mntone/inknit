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
	static constexpr bool
	point(std::int32_t x, std::int32_t y, std::int32_t ix, std::int32_t iy) noexcept {
		return x == ix && y == iy;
	}

	INKNIT_NODISCARD
	static constexpr bool hline(
		std::int32_t x, std::int32_t y, std::int32_t ix1, std::int32_t ix2, std::int32_t iy
	) noexcept {
		return ix1 <= x && x <= ix2 && y == iy;
	}

	INKNIT_NODISCARD
	static constexpr bool vline(
		std::int32_t x, std::int32_t y, std::int32_t ix, std::int32_t iy1, std::int32_t iy2
	) noexcept {
		return x == ix && iy1 <= y && y <= iy2;
	}

	INKNIT_NODISCARD
	static constexpr bool
	rect_with_point(std::int32_t x, std::int32_t y, point_t ipt1, point_t ipt2) {
		auto [x1, y1] = ipt1;
		auto [x2, y2] = ipt2;
		return (x1 <= x && x <= x2) && (y1 <= y && y <= y2);
	}

	INKNIT_NODISCARD
	static constexpr bool rect(std::int32_t x, std::int32_t y, point_t ipt, size_t isz) {
		auto [width, height] = isz;
		if (width == 0 || height == 0) {
			return false;
		}

		auto [left, top]          = ipt;
		std::int32_t const right  = left + width - 1;
		std::int32_t const bottom = top + height - 1;
		return rect_with_point(x, y, ipt, {right, bottom});
	}

	INKNIT_NODISCARD
	static constexpr bool
	rect_edge_with_point(std::int32_t x, std::int32_t y, point_t ipt1, point_t ipt2) {
		auto [x1, y1] = ipt1;
		auto [x2, y2] = ipt2;
		return (y == y1 || y == y2) && (x1 <= x && x <= x2)
			|| (y1 <= y && y <= y2) && (x == x1 || x == x2);
	}

	INKNIT_NODISCARD
	static constexpr bool rect_edge(std::int32_t x, std::int32_t y, point_t ipt, size_t isz) {
		auto [width, height] = isz;
		if (width == 0 || height == 0) {
			return false;
		}

		auto [left, top]          = ipt;
		std::int32_t const right  = left + width - 1;
		std::int32_t const bottom = top + height - 1;
		return rect_edge_with_point(x, y, ipt, {right, bottom});
	}

	INKNIT_NODISCARD
	static constexpr bool list(std::int32_t x, std::int32_t y, pixel_list const& il) noexcept {
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
	return [trueColor](std::int32_t, std::int32_t) noexcept {
		return trueColor;
	};
}

INKNIT_NODISCARD
static constexpr auto bind_is_pixel_on_point(
	std::int32_t ix, std::int32_t iy, color_t trueColor = colors::white
) noexcept {
	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::point, _1, _2, ix, iy)
	);
}

INKNIT_NODISCARD
static constexpr auto bind_is_pixel_on_hline(
	std::int32_t ix1, std::int32_t ix2, std::int32_t iy, color_t trueColor = colors::white
) noexcept {
	if (ix1 > ix2) {
		std::swap(ix1, ix2);
	}

	using namespace std::placeholders;
	return details::compose(
		std::bind(details::boolean_to_color, _1, trueColor),
		std::bind(is_pixel_on::hline, _1, _2, ix1, ix2, iy)
	);
}

INKNIT_NODISCARD
static constexpr auto bind_is_pixel_on_vline(
	std::int32_t ix, std::int32_t iy1, std::int32_t iy2, color_t trueColor = colors::white
) noexcept {
	if (iy1 > iy2) {
		std::swap(iy1, iy2);
	}

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
constexpr pixel_list make_bresenham_line(
	std::int32_t x1,
	std::int32_t y1,
	std::int32_t x2,
	std::int32_t y2,
	std::int32_t width  = INT32_MAX,
	std::int32_t height = INT32_MAX
) noexcept {
	pixel_list points;

	bool const steep = abs(x2 - x1) < abs(y2 - y1);
	if (steep) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	std::int32_t const dx    = x2 - x1;
	std::int32_t const dy    = abs(y2 - y1);
	std::int32_t       error = dx / 2;
	std::int32_t const ystep = y1 < y2 ? 1 : -1;
	std::int32_t       y     = y1;

	for (std::int32_t x = x1; x <= x2; ++x) {
		if (steep) {
			if (0 <= y && y <= width && 0 <= x && x <= height) {
				points.emplace_back(y, x);
			}
		} else {
			if (0 <= x && x <= width && 0 <= y && y <= height) {
				points.emplace_back(x, y);
			}
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

	constexpr void add_circle_points(
		pixel_list& points, std::int32_t cx, std::int32_t cy, std::int32_t dx, std::int32_t dy
	) noexcept {
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

INKNIT_NODISCARD constexpr pixel_list
make_midpoint_circle(std::int32_t cx, std::int32_t cy, std::int32_t radius) noexcept {
	pixel_list points;
	if (radius < 0) {
		return points;
	}
	if (radius == 0) {
		points.emplace_back(cx, cy);
		return points;
	}

	std::int32_t dx = 0;
	std::int32_t dy = radius;
	std::int32_t d  = 1 - radius;

	do {
		details::add_circle_points(points, cx, cy, dx, dy);

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

namespace details {

	constexpr void add_ellipse_points(
		pixel_list& points, std::int32_t cx, std::int32_t cy, std::int32_t dx, std::int32_t dy
	) noexcept {
		points.emplace_back(cx + dx, cy + dy);
		points.emplace_back(cx - dx, cy + dy);
		points.emplace_back(cx + dx, cy - dy);
		points.emplace_back(cx - dx, cy - dy);
	}

}  // namespace details

INKNIT_NODISCARD constexpr pixel_list
make_midpoint_ellipse(std::int32_t cx, std::int32_t cy, std::int32_t rx, std::int32_t ry) noexcept {
	pixel_list points;
	if (rx < 0 || ry < 0) {
		return points;
	}

	if (rx == 0 && ry == 0) {
		points.emplace_back(cx, cy);
		return points;
	}
	if (rx == 0) {
		for (std::int32_t y = -ry; y <= ry; ++y) {
			points.emplace_back(cx, cy + y);
		}
		return points;
	}
	if (ry == 0) {
		for (std::int32_t x = -rx; x <= rx; ++x) {
			points.emplace_back(cx + x, cy);
		}
		return points;
	}

	std::int64_t const rx_sq        = static_cast<std::int64_t>(rx) * rx;
	std::int64_t const ry_sq        = static_cast<std::int64_t>(ry) * ry;
	std::int64_t const double_rx_sq = rx_sq << 1;
	std::int64_t const double_ry_sq = ry_sq << 1;

	std::int32_t dx = 0;
	std::int32_t dy = ry;
	std::int64_t px = 0;
	std::int64_t py = double_rx_sq * dy;

	std::int64_t p;

	// Region 1
	p = ry_sq - rx_sq * ry + (rx_sq >> 2);
	while (px < py) {
		details::add_ellipse_points(points, cx, cy, dx, dy);

		++dx;
		px += double_ry_sq;
		if (p < 0) {
			p += px + ry_sq;
		} else {
			--dy;
			py -= double_rx_sq;
			p += px - py + ry_sq;
		}
	}

	// Region 2
	p = ry_sq * (dx * dx + dx) + rx_sq * (dy * dy - dy) - rx_sq * ry_sq;
	while (dy >= 0) {
		details::add_ellipse_points(points, cx, cy, dx, dy);

		--dy;
		py -= double_rx_sq;
		if (p > 0) {
			p += rx_sq - py;
		} else {
			++dx;
			px += double_ry_sq;
			p += px - py + rx_sq;
		}
	}
	return points;
}

}  // namespace inknit::tests
