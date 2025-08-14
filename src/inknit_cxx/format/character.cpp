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

#include "../inc/inknit/format.hpp"

#include <vector>  // vector

#include "inknit.h"

using get_pixel_t = inknit_color_t (*)(inknit_image const *, std::int32_t, std::int32_t);

static std::string
_inknit_block_format(inknit_image const& image, get_pixel_t get_pixel, char const dict[]) noexcept {
	std::int32_t const width  = image.width;
	std::int32_t const height = image.height;

	std::size_t       offset = 0;
	std::vector<char> buffer(static_cast<std::size_t>((width + 1) * height));
	for (std::int32_t y = 0; y < height; ++y) {
		for (std::int32_t x = 0; x < width; ++x) {
			inknit::color_t color = get_pixel(&image, x, y);
			buffer[offset++]      = dict[color];
		}
		buffer[offset++] = '\n';
	}

	std::string new_string(buffer.data(), buffer.size());
	return std::move(new_string);
}

#define FORMAT_IMPL(layout, dict_name)                                                            \
	std::string new_string = _inknit_block_format(image, inknit_##layout##_get_pixel, dict_name); \
	return std::move(new_string)


// -- MARK: names dictionary

constexpr std::string_view NAMES_1BPP[] = {
	"BLACK (0)",
	"WHITE (1)",
};

constexpr std::string_view NAMES_2BPP[] = {
	"BLACK (0)",
	"DARK_GRAY (1)",
	"LIGHT_GRAY (2)",
	"WHITE (3)",
};

constexpr std::string_view NAMES_4BPP[] = {
	"0% (0)",
	"7% (1)",
	"13% (2)",
	"20% (3)",
	"27% (4)",
	"33% (5)",
	"40% (6)",
	"47% (7)",
	"53% (8)",
	"60% (9)",
	"67% (A)",
	"73% (B)",
	"80% (C)",
	"87% (D)",
	"93% (E)",
	"100% (F)",
};


// -- MARK: colors dictionary

constexpr char COLORS_1BPP[] = {'.', '#'};
constexpr char COLORS_2BPP[] = {'.', '1', '2', '3'};
constexpr char COLORS_4BPP[]
	= {'.', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

using namespace inknit;
using namespace inknit::format;


// -- MARK: layout=x1, format=grayscale

template<>
std::string_view
character_formatter<pixel_layout::x1, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_1BPP[color];
}

template<>
std::string character_formatter<pixel_layout::x1, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x1, COLORS_1BPP);
}


// -- MARK: layout=x1lsb, format=grayscale

template<>
std::string_view character_formatter<pixel_layout::x1lsb, pixel_format::grayscale>::name(
	color_t color
) const noexcept {
	return NAMES_1BPP[color];
}

template<>
std::string character_formatter<pixel_layout::x1lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x1lsb, COLORS_1BPP);
}


// -- MARK: layout=x2, format=grayscale

template<>
std::string_view
character_formatter<pixel_layout::x2, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_2BPP[color];
}

template<>
std::string character_formatter<pixel_layout::x2, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x2, COLORS_2BPP);
}


// -- MARK: layout=x2lsb, format=grayscale

template<>
std::string_view character_formatter<pixel_layout::x2lsb, pixel_format::grayscale>::name(
	color_t color
) const noexcept {
	return NAMES_2BPP[color];
}

template<>
std::string character_formatter<pixel_layout::x2lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x2lsb, COLORS_2BPP);
}


// -- MARK: layout=x4, format=grayscale

template<>
std::string_view
character_formatter<pixel_layout::x4, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_4BPP[color];
}

template<>
std::string character_formatter<pixel_layout::x4, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x4, COLORS_4BPP);
}


// -- MARK: layout=x4lsb, format=grayscale

template<>
std::string_view character_formatter<pixel_layout::x4lsb, pixel_format::grayscale>::name(
	color_t color
) const noexcept {
	return NAMES_4BPP[color];
}

template<>
std::string character_formatter<pixel_layout::x4lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x4lsb, COLORS_4BPP);
}
