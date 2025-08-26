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

#include <cmath>   // floor, log10
#include <vector>  // vector

#include "inknit.h"


// -- MARK: fallthrough
#if defined(__GNUC__) && __GNUC__ >= 7
#define FALLTHROUGH __attribute__((fallthrough))
#elif defined(__clang__) && __has_attribute(fallthrough)
#define FALLTHROUGH __attribute__((fallthrough))
#elif defined(_MSC_VER)
#ifndef __fallthrough
#define __fallthrough __pragma(warning(suppress : 26819))
#endif
#define FALLTHROUGH __fallthrough
#else
#define FALLTHROUGH \
	do {            \
	} while (0)
#endif


// -- MARK: escape sequence macros
#define ESC_SIXEL_DCS  "\x1bP7;1q"
#define ESC_SIXEL_TERM "\x1b\\"


// -- MARK: utils
using get_pixel_t = inknit_color_t (*)(inknit_image const *, std::int32_t, std::int32_t);

template<std::integral I>
static inline constexpr I count_digits(I val) noexcept {
	if (val == 0) {
		return 1;
	}
	return static_cast<I>(std::floor(std::log10(val))) + 1;
}

static inline constexpr void
append_string_view(std::vector<char>::iterator& iter, std::string_view text) noexcept {
	std::copy(text.cbegin(), text.cend(), iter);
	std::advance(iter, text.size());
}

static inline constexpr void
append_number(std::vector<char>::iterator& iter, std::size_t num) noexcept {
	std::size_t       digits = count_digits(num);
	std::vector<char> text(digits);
	do {
		text[--digits] = '0' + static_cast<char>(num % 10);
		num /= 10;
	} while (num > 0);

	append_string_view(iter, std::string_view(text.data(), text.size()));
}

static inline void
append_rle_char(std::vector<char>::iterator& iter, char ch, std::size_t count) noexcept {
	switch (count) {
	case 3:
		*iter++ = ch;
		FALLTHROUGH;
	case 2:
		*iter++ = ch;
		FALLTHROUGH;
	case 1:
		*iter++ = ch;
		break;
	case 0:
		break;
	default:
		*iter++ = '!';
		append_number(iter, count);
		*iter++ = ch;
	}
}

static inline constexpr void
append_data_as_rle(std::vector<char>::iterator& iter, std::vector<char> const& data) noexcept {
	if (data.empty()) {
		return;
	}

	char          last_ch = data[0];
	std::uint32_t count   = 1;

	for (std::size_t i = 1; i < data.size(); ++i) {
		char ch = data[i];
		if (ch == last_ch) {
			++count;
		} else {
			append_rle_char(iter, last_ch, count);
			last_ch = ch;
			count   = 1;
		}
	}

	append_rle_char(iter, last_ch, count);
}

static std::string _inknit_sixel_format(
	inknit_image const&    image,
	get_pixel_t            get_pixel,
	std::string_view const pallet,
	std::size_t const      pallet_count
) noexcept {
	std::int32_t const width  = image.width;
	std::int32_t const height = image.height;

	std::vector<char> buffer(pallet.size() + static_cast<std::size_t>((width + 1) * height));
	std::vector<char>::iterator iter = buffer.begin();
	append_string_view(iter, ESC_SIXEL_DCS);
	append_string_view(iter, pallet);

	std::vector<char> row(static_cast<std::size_t>(width));
	for (std::int32_t y = 0; y < height; y += 6) {
		bool is_first_color = true;
		for (std::size_t c = 0; c < pallet_count; ++c) {
			bool color_found = false;
			for (std::int32_t x = 0; x < width; ++x) {
				std::uint8_t sixel = 0;
				for (std::int32_t i = 0; i < 6; ++i) {
					std::int32_t const iy = y + i;
					if (iy < height) {
						if (get_pixel(&image, x, iy) == c) {
							sixel |= 1 << i;
						}
					}
				}
				if (sixel > 0) {
					color_found = true;
				}
				row[static_cast<std::size_t>(x)] = '?' + static_cast<char>(sixel);
			}

			if (color_found) {
				if (!is_first_color) {
					*iter++ = '$';
				}
				is_first_color = false;

				*iter++ = '#';
				append_number(iter, c);
				append_data_as_rle(iter, row);
			}
		}
		*iter++ = '-';
	}
	append_string_view(iter, ESC_SIXEL_TERM);

	std::size_t const actual_size = static_cast<std::size_t>(std::distance(buffer.begin(), iter));
	return {buffer.data(), actual_size};
}

#define FORMAT_IMPL(layout, pallet, pallet_count)                                         \
	std::string new_string                                                                \
		= _inknit_sixel_format(image, inknit_##layout##_get_pixel, pallet, pallet_count); \
	return std::move(new_string)


// -- MARK: colors dictionary

constexpr std::size_t      PALLET_1BPP_COUNT = 2;
constexpr std::string_view PALLET_1BPP
	= "#0;2;0;0;0"
	  "#1;2;100;100;100";

constexpr std::size_t      PALLET_2BPP_COUNT = 4;
constexpr std::string_view PALLET_2BPP
	= "#0;2;0;0;0"
	  "#1;2;33;33;33"
	  "#2;2;67;67;67"
	  "#3;2;100;100;100";

constexpr std::size_t      PALLET_4BPP_COUNT = 16;
constexpr std::string_view PALLET_4BPP
	= "#0;2;0;0;0"
	  "#1;2;6;6;6"
	  "#2;2;13;13;13"
	  "#3;2;20;20;20"
	  "#4;2;27;27;27"
	  "#5;2;33;33;33"
	  "#6;2;40;40;40"
	  "#7;2;47;47;47"
	  "#8;2;53;53;53"
	  "#9;2;60;60;60"
	  "#10;2;67;67;67"
	  "#11;2;73;73;73"
	  "#12;2;80;80;80"
	  "#13;2;87;87;87"
	  "#14;2;93;93;93"
	  "#15;2;100;100;100";


// -- MARK: use namespaces
using namespace inknit;
using namespace inknit::format;


// -- MARK: layout=x1, format=grayscale

template<>
std::string sixel_formatter<pixel_layout::x1, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x1, PALLET_1BPP, PALLET_1BPP_COUNT);
}


// -- MARK: layout=x1lsb, format=grayscale

template<>
std::string sixel_formatter<pixel_layout::x1lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x1lsb, PALLET_1BPP, PALLET_1BPP_COUNT);
}


// -- MARK: layout=x2, format=grayscale

template<>
std::string sixel_formatter<pixel_layout::x2, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x2, PALLET_2BPP, PALLET_2BPP_COUNT);
}


// -- MARK: layout=x2lsb, format=grayscale

template<>
std::string sixel_formatter<pixel_layout::x2lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x2lsb, PALLET_2BPP, PALLET_2BPP_COUNT);
}


// -- MARK: layout=x4, format=grayscale

template<>
std::string sixel_formatter<pixel_layout::x4, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x4, PALLET_4BPP, PALLET_4BPP_COUNT);
}


// -- MARK: layout=x4lsb, format=grayscale

template<>
std::string sixel_formatter<pixel_layout::x4lsb, pixel_format::grayscale>::format(
	inknit_image const& image
) const noexcept {
	FORMAT_IMPL(x4lsb, PALLET_4BPP, PALLET_4BPP_COUNT);
}
