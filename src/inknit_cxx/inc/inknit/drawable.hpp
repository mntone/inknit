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

#include "base.hpp"

namespace inknit { namespace details {

	template<std::int32_t BITS_PER_PIXEL>
	struct image_helpers: std::false_type {
		static_assert(!value, "This class has to be implemented for desired type");
	};

	template<>
	struct image_helpers<1>: std::true_type {
	protected:
		static constexpr color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b1;
#ifdef INKNIT_SLOW_MULTIPLY
			word |= word << 1;
			word |= word << 2;
			word |= word << 4;
			word |= word << 8;
			word |= word << 16;
#else
			word *= 0xFFFFFFFFu;
#endif
			return word;
		}
	};

	template<>
	struct image_helpers<2>: std::true_type {
	protected:
		static constexpr color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b11;
#ifdef INKNIT_SLOW_MULTIPLY
			word |= word << 2;
			word |= word << 4;
			word |= word << 8;
			word |= word << 16;
#else
			word *= 0x55555555u;
#endif
			return word;
		}
	};

	template<>
	struct image_helpers<4>: std::true_type {
	protected:
		static constexpr color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b1111;
#ifdef INKNIT_SLOW_MULTIPLY
			word |= word << 4;
			word |= word << 8;
			word |= word << 16;
#else
			word *= 0x11111111u;
#endif
			return word;
		}
	};

	template<pixel_layout PIXEL_LAYOUT>
	struct drawable_image: std::false_type {
		static_assert(!value, "This class has to be implemented for desired type");
	};

}}  //namespace inknit::details


#include "inknit/prefix.h"

// Pixel Layout: x1 (32-bit width/align, 1-bpp, BE)
#define INKNIT_CURRENT_LAYOUT x1
#define INKNIT_CURRENT_BPP    1
#include "drawable.hpp.in"
#undef INKNIT_CURRENT_LAYOUT
#undef INKNIT_CURRENT_BPP

// Pixel Layout: x1lsb (32-bit width/align, 1-bpp, LE)
#define INKNIT_CURRENT_LAYOUT x1lsb
#define INKNIT_CURRENT_BPP    1
#include "drawable.hpp.in"
#undef INKNIT_CURRENT_LAYOUT
#undef INKNIT_CURRENT_BPP

// Pixel Layout: x2 (32-bit width/align, 2-bpp, BE)
#define INKNIT_CURRENT_LAYOUT x2
#define INKNIT_CURRENT_BPP    2
#include "drawable.hpp.in"
#undef INKNIT_CURRENT_LAYOUT
#undef INKNIT_CURRENT_BPP

// Pixel Layout: x2lsb (32-bit width/align, 2-bpp, LE)
#define INKNIT_CURRENT_LAYOUT x2lsb
#define INKNIT_CURRENT_BPP    2
#include "drawable.hpp.in"
#undef INKNIT_CURRENT_LAYOUT
#undef INKNIT_CURRENT_BPP

// Pixel Layout: x4 (32-bit width/align, 4-bpp, BE)
#define INKNIT_CURRENT_LAYOUT x4
#define INKNIT_CURRENT_BPP    4
#include "drawable.hpp.in"
#undef INKNIT_CURRENT_LAYOUT
#undef INKNIT_CURRENT_BPP

// Pixel Layout: x4lsb (32-bit width/align, 4-bpp, LE)
#define INKNIT_CURRENT_LAYOUT x4lsb
#define INKNIT_CURRENT_BPP    4
#include "drawable.hpp.in"
#undef INKNIT_CURRENT_LAYOUT
#undef INKNIT_CURRENT_BPP
