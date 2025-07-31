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

#include <algorithm>  // max, fill_n
#include <concepts>   // requires, is_arithmetic_v
#include <sstream>    // ostream, ostringstream

#include "inknit.hpp"

// -- MARK: Define meta macro
#ifdef ENABLED_COLOR_DUMP
#define META_RESET    "\e[m"
#define META_BG_BLACK "\e[48;5;16m"
#define META_BG_WHITE "\e[48;5;231m"

#define META_FG_BLACK "\e[38;5;16m"
#define META_FG_WHITE "\e[38;5;231m"

#define META_FG_RED    "\e[38;5;196m"
#define META_FG_YELLOW "\e[38;5;226m"
#define META_FG_ORANGE "\e[38;5;214m"
#define META_FG_GREEN  "\e[38;5;28m"
#define META_FG_BLUE   "\e[38;5;21m"
#endif

namespace inknit::tests {

namespace details {

	template<class Float, class... Args>
	concept color_invocable = std::regular_invocable<Float, Args...>
						   && std::same_as<std::invoke_result_t<Float, Args...>, color_t>;

	template<pixel_layout PIXEL_LAYOUT, pixel_format PIXEL_FORMAT>
	struct stringable: std::false_type {
		static_assert(!value, "This class has to be implemented for desired type");
	};

	template<>
	struct stringable<pixel_layout::x2lsb, pixel_format::index>: std::true_type {
		static constexpr std::string_view name(color_t color) noexcept {
#ifdef ENABLED_COLOR_DUMP
			constexpr std::string_view DICT[] = {
				META_FG_BLACK META_BG_WHITE "BLACK" META_RESET " (0)",
				META_FG_WHITE META_BG_BLACK "WHITE" META_RESET " (1)",
				META_FG_RED META_BG_WHITE "RED" META_RESET " (2)",
				META_FG_YELLOW META_BG_WHITE "YELLOW" META_RESET " (3)",
			};
#else
			constexpr std::string_view DICT[] = {
				"BLACK (0)",
				"WHITE (1)",
				"RED (2)",
				"YELLOW (3)",
			};
#endif
			return DICT[color];
		}

		static constexpr std::string_view visualize(color_t color) noexcept {
#ifdef ENABLED_COLOR_DUMP
			constexpr std::string_view DICT[] = {
				META_FG_BLACK "██",
				META_FG_WHITE "██",
				META_FG_RED "██",
				META_FG_YELLOW "██",
			};
#else
			constexpr std::string_view DICT[] = {"B", "W", "R", "Y"};
#endif
			return DICT[color];
		}
	};

	struct __1bpp_stringable: std::true_type {
		static constexpr std::string_view name(color_t color) noexcept {
#ifdef ENABLED_COLOR_DUMP
			constexpr std::string_view DICT[] = {
				META_FG_BLACK META_BG_WHITE "BLACK (0)" META_RESET,
				META_FG_WHITE META_BG_BLACK "WHITE (1)" META_RESET,
			};
#else
			constexpr std::string_view DICT[] = {
				"BLACK (0)",
				"WHITE (1)",
			};
#endif
			return DICT[color];
		}

		static constexpr std::string_view visualize(color_t color) noexcept {
			// #ifdef ENABLED_COLOR_DUMP
			// 			constexpr std::string_view DICT[] = {
			// 				META_FG_BLACK "██",
			// 				META_FG_WHITE "██",
			// 			};
			// #else
			constexpr std::string_view DICT[] = {".", "#"};
			//#endif
			return DICT[color];
		}
	};
	template<>
	struct stringable<pixel_layout::x1, pixel_format::grayscale>: public __1bpp_stringable {};
	template<>
	struct stringable<pixel_layout::x1lsb, pixel_format::grayscale>: public __1bpp_stringable {};

	struct __2bpp_stringable: std::true_type {
		static constexpr std::string_view name(color_t color) noexcept {
#ifdef ENABLED_COLOR_DUMP
			constexpr std::string_view DICT[] = {
				META_FG_BLACK META_BG_WHITE "BLACK (0)" META_RESET,
				"\e[38;5;240m" META_BG_WHITE "DARK_GRAY (1)" META_RESET,
				"\e[38;5;248m" META_BG_BLACK "LIGHT_GRAY (2)" META_RESET,
				META_FG_WHITE META_BG_BLACK "WHITE (3)" META_RESET,
			};
#else
			constexpr std::string_view DICT[] = {
				"BLACK (0)",
				"DARK_GRAY (1)",
				"LIGHT_GRAY (2)",
				"WHITE (3)",
			};
#endif
			return DICT[color];
		}

		static constexpr std::string_view visualize(color_t color) noexcept {
#ifdef ENABLED_COLOR_DUMP
			constexpr std::string_view DICT[] = {
				META_FG_BLACK "██",
				"\e[38;5;240m██",
				"\e[38;5;248m██",
				META_FG_WHITE "██",
			};
#else
			constexpr std::string_view DICT[] = {".", ":", "/", "#"};
#endif
			return DICT[color];
		}
	};
	template<>
	struct stringable<pixel_layout::x2, pixel_format::grayscale>: public __2bpp_stringable {};
	template<>
	struct stringable<pixel_layout::x2lsb, pixel_format::grayscale>: public __2bpp_stringable {};

	struct __4bpp_stringable: std::true_type {
		static constexpr std::string_view name(color_t color) noexcept {
#ifdef ENABLED_COLOR_DUMP
			constexpr std::string_view DICT[] = {
				"\e[38;2;0;0;0m" META_BG_WHITE "0% (0)" META_RESET,
				"\e[38;2;17;17;17m" META_BG_WHITE "7% (1)" META_RESET,
				"\e[38;2;34;34;34m" META_BG_WHITE "13% (2)" META_RESET,
				"\e[38;2;51;51;51m" META_BG_WHITE "20% (3)" META_RESET,
				"\e[38;2;68;68;68m" META_BG_WHITE "27% (4)" META_RESET,
				"\e[38;2;85;85;85m" META_BG_WHITE "33% (5)" META_RESET,
				"\e[38;2;102;102;102m" META_BG_WHITE "40% (6)" META_RESET,
				"\e[38;2;119;119;119m" META_BG_WHITE "47% (7)" META_RESET,
				"\e[38;2;136;136;136m" META_BG_BLACK "53% (8)" META_RESET,
				"\e[38;2;153;153;153m" META_BG_BLACK "60% (9)" META_RESET,
				"\e[38;2;170;170;170m" META_BG_BLACK "67% (A)" META_RESET,
				"\e[38;2;187;187;187m" META_BG_BLACK "73% (B)" META_RESET,
				"\e[38;2;204;204;204m" META_BG_BLACK "80% (C)" META_RESET,
				"\e[38;2;221;221;221m" META_BG_BLACK "87% (D)" META_RESET,
				"\e[38;2;238;238;238m" META_BG_BLACK "93% (E)" META_RESET,
				"\e[38;2;255;255;255m" META_BG_BLACK "100% (F)" META_RESET,
			};
#else
			constexpr std::string_view DICT[] = {
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
#endif
			return DICT[color];
		}

		static constexpr std::string_view visualize(color_t color) noexcept {
#ifdef ENABLED_COLOR_DUMP
			constexpr std::string_view DICT[] = {
				"\e[38;2;0;0;0m██",
				"\e[38;2;17;17;17m██",
				"\e[38;2;34;34;34m██",
				"\e[38;2;51;51;51m██",
				"\e[38;2;68;68;68m██",
				"\e[38;2;85;85;85m██",
				"\e[38;2;102;102;102m██",
				"\e[38;2;119;119;119m██",
				"\e[38;2;136;136;136m██",
				"\e[38;2;153;153;153m██",
				"\e[38;2;170;170;170m██",
				"\e[38;2;187;187;187m██",
				"\e[38;2;204;204;204m██",
				"\e[38;2;221;221;221m██",
				"\e[38;2;238;238;238m██",
				"\e[38;2;255;255;255m██",
			};
#else
			constexpr std::string_view DICT[]
				= {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
#endif
			return DICT[color];
		}
	};
	template<>
	struct stringable<pixel_layout::x4, pixel_format::grayscale>: public __4bpp_stringable {};
	template<>
	struct stringable<pixel_layout::x4lsb, pixel_format::grayscale>: public __4bpp_stringable {};

}  // namespace details


template<pixel_layout PIXEL_LAYOUT, pixel_format PIXEL_FORMAT>
struct test_image
	: public image<PIXEL_LAYOUT, PIXEL_FORMAT>
	, details::stringable<PIXEL_LAYOUT, PIXEL_FORMAT> {
	constexpr test_image()
		: image<PIXEL_LAYOUT, PIXEL_FORMAT>() {}

	constexpr explicit test_image(std::uint16_t width, std::uint16_t height, color_t color = 0)
		: image<PIXEL_LAYOUT, PIXEL_FORMAT>(width, height, color) {}

	constexpr void dump(std::ostream& os) const noexcept {
		uint_t const height = this->height();
		uint_t const width  = this->width();
		for (uint_t y = 0; y < height; ++y) {
			for (uint_t x = 0; x < width; ++x) {
				color_t color = this->at(x, y);
				os << details::stringable<PIXEL_LAYOUT, PIXEL_FORMAT>::visualize(color);
			}
#ifdef ENABLED_COLOR_DUMP
			os << META_RESET << std::endl;
#else
			os << std::endl;
#endif
		}
	}

	template<class Function>
		requires details::color_invocable<Function, uint_t, uint_t>
	void test(Function fn) const noexcept {
		uint_t const height = this->height();
		uint_t const width  = this->width();
		for (uint_t y = 0; y < height; ++y) {
			for (uint_t x = 0; x < width; ++x) {
				color_t actual   = this->at(x, y);
				color_t expected = fn(x, y);
				if (actual != expected) {
					std::ostringstream oss;
					oss
						<< "Unexpected value at ("
						<< x
						<< ", "
						<< y
						<< "): got "
						<< details::stringable<PIXEL_LAYOUT, PIXEL_FORMAT>::name(actual)
						<< ", expected "
						<< details::stringable<PIXEL_LAYOUT, PIXEL_FORMAT>::name(expected)
						<< std::endl;
					dump(oss);
					FAIL(oss.str());
					return;
				}
			}
		}
	}
};


template<
	std::uint16_t WIDTH,
	std::uint16_t HEIGHT,
	pixel_layout  PIXEL_LAYOUT,
	pixel_format  PIXEL_FORMAT>
struct fixed_test_image
	: public fixed_image<WIDTH, HEIGHT, PIXEL_LAYOUT, PIXEL_FORMAT>
	, details::stringable<PIXEL_LAYOUT, PIXEL_FORMAT> {
	constexpr fixed_test_image() noexcept
		: fixed_image<WIDTH, HEIGHT, PIXEL_LAYOUT, PIXEL_FORMAT>() {}

	constexpr explicit fixed_test_image(color_t color) noexcept
		: fixed_image<WIDTH, HEIGHT, PIXEL_LAYOUT, PIXEL_FORMAT>(color) {}

	constexpr void dump(std::ostream& os) const noexcept {
		for (uint_t y = 0; y < HEIGHT; ++y) {
			for (uint_t x = 0; x < WIDTH; ++x) {
				color_t color = this->at(x, y);
				os << details::stringable<PIXEL_LAYOUT, PIXEL_FORMAT>::visualize(color);
			}
#ifdef ENABLED_COLOR_DUMP
			os << META_RESET << std::endl;
#else
			os << std::endl;
#endif
		}
	}

	template<class Function>
		requires details::color_invocable<Function, uint_t, uint_t>
	void test(Function fn) const noexcept {
		for (uint_t y = 0; y < HEIGHT; ++y) {
			for (uint_t x = 0; x < WIDTH; ++x) {
				color_t actual   = this->at(x, y);
				color_t expected = fn(x, y);
				if (actual != expected) {
					std::ostringstream oss;
					oss
						<< "Unexpected value at ("
						<< x
						<< ", "
						<< y
						<< "): got "
						<< details::stringable<PIXEL_LAYOUT, PIXEL_FORMAT>::name(actual)
						<< ", expected "
						<< details::stringable<PIXEL_LAYOUT, PIXEL_FORMAT>::name(expected)
						<< std::endl;
					dump(oss);
					FAIL(oss.str());
					return;
				}
			}
		}
	}
};

template<std::uint16_t WIDTH, std::uint16_t HEIGHT>
using fixed_test_p2image
	= fixed_test_image<WIDTH, HEIGHT, pixel_layout::x2lsb, pixel_format::index>;

}  // namespace inknit::tests


// -- MARK: Clean up meta macro
#ifdef ENABLED_COLOR_DUMP
#undef META_RESET
#undef META_BG_BLACK
#undef META_BG_WHITE

#undef META_FG_BLACK
#undef META_FG_WHITE

#undef META_FG_RED
#undef META_FG_YELLOW
#undef META_FG_ORANGE
#undef META_FG_GREEN
#undef META_FG_BLUE
#endif
