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

#include <algorithm>  // fill_n
#include <vector>     // vector

#include "inknit/base.hpp"      // color_t, int_t, point_t, size_t, and each primitive type
#include "inknit/drawable.hpp"  // drawable_image

#define INKNIT_TARGET_BIT_WIDTH 32

namespace inknit {

template<
	pixel_layout PIXEL_LAYOUT,
	pixel_format PIXEL_FORMAT,
	std::int32_t BITS_PER_PIXEL = ((static_cast<std::int32_t>(PIXEL_LAYOUT) & 0x3C) >> 2) + 1>
struct image
	: public details::image_traits<image<PIXEL_LAYOUT, PIXEL_FORMAT, BITS_PER_PIXEL>>
	, public details::drawable_image<PIXEL_LAYOUT> {
	static inline constexpr std::int32_t bpw = INKNIT_TARGET_BIT_WIDTH;  // bits per word
	static inline constexpr std::int32_t bpp = BITS_PER_PIXEL;           // bits per pixel
	static inline constexpr std::int32_t ppw = bpw / bpp;                // pixels per word

private:
	static inline constexpr std::int32_t pixelsPerByte = 8 / bpp;

public:
	constexpr explicit image(std::uint16_t width, std::uint16_t height, color_t color = 0)
		: buffer_(details::stride<std::size_t>(width, bpp, INKNIT_TARGET_BIT_WIDTH) * height / 4)
		, details::drawable_image<PIXEL_LAYOUT>(
			  PIXEL_FORMAT,
			  inknit::colorspace::unspecified,
			  inknit::colorrange::full,
			  details::align(INKNIT_TARGET_BIT_WIDTH),
			  width,
			  height,
			  static_cast<std::uint16_t>(
				  details::stride<std::uint32_t>(width, bpp, INKNIT_TARGET_BIT_WIDTH)
			  ),
			  nullptr
		  ) {
		std::uint32_t *const data = buffer_.data();
		std::size_t const    size = buffer_.size();
		if (size != 0) {
			std::fill_n(data, size, details::drawable_image<PIXEL_LAYOUT>::get_fill_word(color));
		}

		this->data_ = data;
	}
	constexpr image()
		: image(0, 0, 0) {}

	constexpr void reset(std::int32_t width, std::int32_t height, color_t color = 0) noexcept {
		std::size_t const stride = details::stride<std::size_t>(
			static_cast<std::size_t>(width), bpp, INKNIT_TARGET_BIT_WIDTH
		);
		std::size_t const newSize = stride * static_cast<std::size_t>(height);
		std::size_t const oldSize = 4 * buffer_.size();

		if (oldSize != newSize) {
			buffer_.resize(newSize);

			std::uint32_t *const data = buffer_.data();
			if (oldSize < newSize) {
				std::size_t const diffSize = newSize - oldSize;
				std::fill_n(
					data + oldSize,
					diffSize / 4,
					details::drawable_image<PIXEL_LAYOUT>::get_fill_word(color)
				);
			}

			this->width_  = width & 0x1FFF;
			this->height_ = static_cast<std::uint16_t>(height);
			this->stride_ = static_cast<std::uint16_t>(stride);
			this->data_   = data;
		}
	}

	inline constexpr std::uint32_t *data() noexcept {
		return buffer_.data();
	}
	inline constexpr std::uint32_t const *data() const noexcept {
		return buffer_.data();
	}

protected:
	std::vector<std::uint32_t> buffer_;
};

template<
	std::uint16_t WIDTH,
	std::uint16_t HEIGHT,
	pixel_layout  PIXEL_LAYOUT,
	pixel_format  PIXEL_FORMAT,
	std::int32_t  BITS_PER_PIXEL = ((static_cast<std::int32_t>(PIXEL_LAYOUT) & 0x3C) >> 2) + 1>
struct fixed_image
	: public details::image_traits<
		  fixed_image<WIDTH, HEIGHT, PIXEL_LAYOUT, PIXEL_FORMAT, BITS_PER_PIXEL>>
	, public details::drawable_image<PIXEL_LAYOUT> {
	// TODO: support rgb bpp
	static inline constexpr std::int32_t bpw = INKNIT_TARGET_BIT_WIDTH;  // bits per word
	static inline constexpr std::int32_t bpp = BITS_PER_PIXEL;           // bits per pixel
	static inline constexpr std::int32_t ppw = bpw / bpp;                // pixels per word

private:
	static inline constexpr std::int32_t PIXELS_PER_BYTE = 8 / bpp;
	static inline constexpr std::int32_t STRIDE
		= details::stride<std::int32_t>(WIDTH, bpp, INKNIT_TARGET_BIT_WIDTH);
	static inline constexpr std::int32_t SIZE = STRIDE * HEIGHT;

public:
	constexpr explicit fixed_image(color_t color) noexcept
		: details::drawable_image<PIXEL_LAYOUT>(
			  PIXEL_FORMAT,
			  inknit::colorspace::unspecified,
			  inknit::colorrange::full,
			  details::align(INKNIT_TARGET_BIT_WIDTH),
			  WIDTH,
			  HEIGHT,
			  STRIDE,
			  buffer_
		  ) {
		std::fill_n(buffer_, SIZE / 4, details::drawable_image<PIXEL_LAYOUT>::get_fill_word(color));
	}
	constexpr fixed_image() noexcept
		: fixed_image(0) {}

	static inline constexpr inknit::colorrange colorrange() noexcept {
		return inknit::colorrange::full;
	}
	static inline constexpr inknit::colorspace colorspace() noexcept {
		return inknit::colorspace::unspecified;
	}
	static inline constexpr pixel_format format() noexcept {
		return PIXEL_FORMAT;
	}
	static inline constexpr pixel_layout layout() noexcept {
		return PIXEL_LAYOUT;
	}

	static inline constexpr inknit::alignment alignment() noexcept {
		return inknit::alignment::align32;
	}
	static inline constexpr std::int32_t height() noexcept {
		return HEIGHT;
	}
	static inline constexpr std::int32_t stride() noexcept {
		return STRIDE;
	}
	static inline constexpr std::int32_t width() noexcept {
		return WIDTH;
	}

protected:
	std::uint32_t buffer_[SIZE / 4];
};

}  // namespace inknit
