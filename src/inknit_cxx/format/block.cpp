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


// -- MARK: escape sequence macros
#define ESC_RESET    "\x1b[m"
#define ESC_BG_BLACK "\x1b[48;5;16m"
#define ESC_BG_WHITE "\x1b[48;5;231m"

#define ESC_FG_256COLOR(val)      "\x1b[38;5;" #val "m"
#define ESC_FG_FULLCOLOR(r, g, b) "\x1b[38;2;" #r ";" #g ";" #b "m"


// -- MARK: utils
using get_pixel_t = inknit_color_t (*)(inknit_image const *, std::int32_t, std::int32_t);

static inline constexpr void
append_string_view(std::vector<char>::iterator& iter, std::string_view text) noexcept {
	std::copy(text.cbegin(), text.cend(), iter);
	std::advance(iter, text.size());
}

static std::string _inknit_block_format(
	inknit_image const&    image,
	get_pixel_t            get_pixel,
	std::string_view const dict[],
	std::size_t const      max_length
) noexcept {
	std::int32_t const width  = image.width;
	std::int32_t const height = image.height;

	std::vector<char> buffer(
		(max_length * static_cast<std::size_t>(width) + 1) * static_cast<std::size_t>(height)
	);
	std::vector<char>::iterator iter = buffer.begin();
	for (std::int32_t y = 0; y < height; ++y) {
		for (std::int32_t x = 0; x < width; ++x) {
			inknit::color_t const  color      = get_pixel(&image, x, y);
			std::string_view const color_text = dict[color];
			append_string_view(iter, color_text);
		}
		*iter++ = '\n';
	}
	append_string_view(iter, ESC_RESET);

	std::size_t const actual_size = static_cast<std::size_t>(std::distance(buffer.begin(), iter));
	buffer.resize(actual_size);

	std::string new_string(buffer.data(), buffer.size());
	return std::move(new_string);
}

#define FORMAT_IMPL(layout, dict_name, max_length)                                         \
	std::string new_string                                                                 \
		= _inknit_block_format(image, inknit_##layout##_get_pixel, dict_name, max_length); \
	return std::move(new_string)


// -- MARK: names dictionary

constexpr std::string_view NAMES_1BPP[] = {
	ESC_FG_256COLOR(16) ESC_BG_WHITE "BLACK (0)" ESC_RESET,
	ESC_FG_256COLOR(231) ESC_BG_BLACK "WHITE (1)" ESC_RESET,
};

constexpr std::string_view NAMES_2BPP[] = {
	ESC_FG_256COLOR(16) ESC_BG_WHITE "BLACK (0)" ESC_RESET,
	ESC_FG_256COLOR(240) ESC_BG_WHITE "DARK_GRAY (1)" ESC_RESET,
	ESC_FG_256COLOR(248) ESC_BG_BLACK "LIGHT_GRAY (2)" ESC_RESET,
	ESC_FG_256COLOR(231) ESC_BG_BLACK "WHITE (3)" ESC_RESET,
};

constexpr std::string_view NAMES_4BPP[] = {
	ESC_FG_FULLCOLOR(0, 0, 0) ESC_BG_WHITE "0% (0)" ESC_RESET,
	ESC_FG_FULLCOLOR(17, 17, 17) ESC_BG_WHITE "7% (1)" ESC_RESET,
	ESC_FG_FULLCOLOR(34, 34, 34) ESC_BG_WHITE "13% (2)" ESC_RESET,
	ESC_FG_FULLCOLOR(51, 51, 51) ESC_BG_WHITE "20% (3)" ESC_RESET,
	ESC_FG_FULLCOLOR(68, 68, 68) ESC_BG_WHITE "27% (4)" ESC_RESET,
	ESC_FG_FULLCOLOR(85, 85, 85) ESC_BG_WHITE "33% (5)" ESC_RESET,
	ESC_FG_FULLCOLOR(102, 102, 102) ESC_BG_WHITE "40% (6)" ESC_RESET,
	ESC_FG_FULLCOLOR(119, 119, 119) ESC_BG_WHITE "47% (7)" ESC_RESET,
	ESC_FG_FULLCOLOR(136, 136, 136) ESC_BG_BLACK "53% (8)" ESC_RESET,
	ESC_FG_FULLCOLOR(153, 153, 153) ESC_BG_BLACK "60% (9)" ESC_RESET,
	ESC_FG_FULLCOLOR(170, 170, 170) ESC_BG_BLACK "67% (A)" ESC_RESET,
	ESC_FG_FULLCOLOR(187, 187, 187) ESC_BG_BLACK "73% (B)" ESC_RESET,
	ESC_FG_FULLCOLOR(204, 204, 204) ESC_BG_BLACK "80% (C)" ESC_RESET,
	ESC_FG_FULLCOLOR(221, 221, 221) ESC_BG_BLACK "87% (D)" ESC_RESET,
	ESC_FG_FULLCOLOR(238, 238, 238) ESC_BG_BLACK "93% (E)" ESC_RESET,
	ESC_FG_FULLCOLOR(255, 255, 255) ESC_BG_BLACK "100% (F)" ESC_RESET,
};


// -- MARK: colors dictionary

constexpr std::size_t      COLORS_1BPP_MAX_LENGTH = 17;
constexpr std::string_view COLORS_1BPP[]          = {
    ESC_FG_256COLOR(16) "██",
    ESC_FG_256COLOR(231) "██",
};

constexpr std::size_t      COLORS_2BPP_MAX_LENGTH = 17;
constexpr std::string_view COLORS_2BPP[]          = {
    ESC_FG_256COLOR(16) "██",
    ESC_FG_256COLOR(240) "██",
    ESC_FG_256COLOR(248) "██",
    ESC_FG_256COLOR(231) "██",
};

constexpr std::size_t      COLORS_4BPP_MAX_LENGTH = 25;
constexpr std::string_view COLORS_4BPP[]          = {
    ESC_FG_FULLCOLOR(0, 0, 0) "██",
    ESC_FG_FULLCOLOR(17, 17, 17) "██",
    ESC_FG_FULLCOLOR(34, 34, 34) "██",
    ESC_FG_FULLCOLOR(51, 51, 51) "██",
    ESC_FG_FULLCOLOR(68, 68, 68) "██",
    ESC_FG_FULLCOLOR(85, 85, 85) "██",
    ESC_FG_FULLCOLOR(102, 102, 102) "██",
    ESC_FG_FULLCOLOR(119, 119, 119) "██",
    ESC_FG_FULLCOLOR(136, 136, 136) "██",
    ESC_FG_FULLCOLOR(153, 153, 153) "██",
    ESC_FG_FULLCOLOR(170, 170, 170) "██",
    ESC_FG_FULLCOLOR(187, 187, 187) "██",
    ESC_FG_FULLCOLOR(204, 204, 204) "██",
    ESC_FG_FULLCOLOR(221, 221, 221) "██",
    ESC_FG_FULLCOLOR(238, 238, 238) "██",
    ESC_FG_FULLCOLOR(255, 255, 255) "██",
};


// -- MARK: use namespaces
using namespace inknit;
using namespace inknit::format;


// -- MARK: layout=x1, format=grayscale

template<>
std::string_view
block_formatter<pixel_layout::x1, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_1BPP[color];
}

template<>
std::string block_formatter<pixel_layout::x1, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x1, COLORS_1BPP, COLORS_1BPP_MAX_LENGTH);
}


// -- MARK: layout=x1lsb, format=grayscale

template<>
std::string_view
block_formatter<pixel_layout::x1lsb, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_1BPP[color];
}

template<>
std::string block_formatter<pixel_layout::x1lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x1lsb, COLORS_1BPP, COLORS_1BPP_MAX_LENGTH);
}


// -- MARK: layout=x2, format=grayscale

template<>
std::string_view
block_formatter<pixel_layout::x2, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_2BPP[color];
}

template<>
std::string block_formatter<pixel_layout::x2, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x2, COLORS_2BPP, COLORS_2BPP_MAX_LENGTH);
}


// -- MARK: layout=x2lsb, format=grayscale

template<>
std::string_view
block_formatter<pixel_layout::x2lsb, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_2BPP[color];
}

template<>
std::string block_formatter<pixel_layout::x2lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x2lsb, COLORS_2BPP, COLORS_2BPP_MAX_LENGTH);
}


// -- MARK: layout=x4, format=grayscale

template<>
std::string_view
block_formatter<pixel_layout::x4, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_4BPP[color];
}

template<>
std::string block_formatter<pixel_layout::x4, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x4, COLORS_4BPP, COLORS_4BPP_MAX_LENGTH);
}


// -- MARK: layout=x4lsb, format=grayscale

template<>
std::string_view
block_formatter<pixel_layout::x4lsb, pixel_format::grayscale>::name(color_t color) const noexcept {
	return NAMES_4BPP[color];
}

template<>
std::string block_formatter<pixel_layout::x4lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x4lsb, COLORS_4BPP, COLORS_4BPP_MAX_LENGTH);
}
