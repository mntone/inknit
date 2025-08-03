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

#include <cstdint>  // uint8_t, uint16_t
#include <type_traits>
#include <utility>  // pair

#include "inknit.h"

namespace inknit {

using color_t = inknit_color_t;
using uint_t  = inknit_uint_t;
using point_t = std::pair<std::int32_t, std::int32_t>;
using size_t  = std::pair<std::int32_t, std::int32_t>;

enum class pixel_layout : std::uint8_t {
	x1    = INKNIT_X1,
	x1lsb = INKNIT_X1LSB,
	x2    = INKNIT_X2,
	x2lsb = INKNIT_X2LSB,
	x4    = INKNIT_X4,
	x4lsb = INKNIT_X4LSB,
	x8    = INKNIT_X8,

	x3y3z2      = INKNIT_X3Y3Z2,
	x5y6z5      = INKNIT_X5Y6Z5,
	x5y6z5le    = INKNIT_X5Y6Z5LE,
	x8y8z8      = INKNIT_X8Y8Z8,
	x8y8z8le    = INKNIT_X8Y8Z8LE,
	x11y11z10   = INKNIT_X11Y11Z10,
	x11y11z10le = INKNIT_X11Y11Z10LE,

	x2y2z2w2      = INKNIT_X2Y2Z2W2,
	x4y4z4w4      = INKNIT_X4Y4Z4W4,
	x1y5z5w5      = INKNIT_X1Y5Z5W5,
	x8y8z8w8      = INKNIT_X8Y8Z8W8,
	x2y10z10w10   = INKNIT_X2Y10Z10W10,
	x2y10z10w10le = INKNIT_X2Y10Z10W10LE,
	w10z10y10x2   = INKNIT_W10Z10Y10X2,
	w10z10y10x2le = INKNIT_W10Z10Y10X2LE,
};

enum class pixel_format : std::uint8_t {
	index     = INKNIT_INDEX,
	grayscale = INKNIT_GRAYSCALE,
	bgr       = INKNIT_BGR,
	xbgr      = INKNIT_XBGR,
	abgr      = INKNIT_ABGR,
};

enum class colorrange : std::uint8_t {
	full    = INKNIT_COLORRANGE_FULL,
	limited = INKNIT_COLORRANGE_LIMITED,
};

enum class colorspace : std::uint8_t {
	unspecified = INKNIT_COLORSPACE_UNSPECIFIED,
	srgb        = INKNIT_SRGB,
	adobergb    = INKNIT_ADOBERGB,
	displayp3   = INKNIT_DISPLAYP3,
	bt601       = INKNIT_BT601,
	bt709       = INKNIT_BT709,
	dcip3       = INKNIT_DCIP3,
	bt2020      = INKNIT_BT2020,
};

enum class alignment : std::uint8_t {
	align8    = INKNIT_ALIGN8,
	align16   = INKNIT_ALIGN16,
	align32   = INKNIT_ALIGN32,
	align64   = INKNIT_ALIGN64,
	align128  = INKNIT_ALIGN128,
	align256  = INKNIT_ALIGN256,
	align512  = INKNIT_ALIGN512,
	align1024 = INKNIT_ALIGN1024,
};

namespace details {

	constexpr inknit::alignment align(std::int32_t bitsPerPixels) noexcept {
		std::int32_t bitflags  = bitsPerPixels / 4 - 1;
		std::int32_t alignment = 0;
		if (bitflags & 0xF0) {
			bitflags >>= 4;
			alignment += 4;
		}
		if (bitflags & 0x0c) {
			bitflags >>= 2;
			alignment += 2;
		}
		if (bitflags & 0x02) {
			bitflags >>= 1;
			alignment += 1;
		}
		return static_cast<inknit::alignment>(alignment);
	}

	template<std::integral I>
	constexpr I stride(I width, I bitsPerPixels, I align) noexcept {
		return ((width * bitsPerPixels + (align - 1)) & ~(align - 1)) / (align >> 3);
	}

	template<typename ImageType>
	struct image_traits {
	private:
		static inline constexpr std::uint8_t __layout_raw
			= static_cast<std::uint8_t>(ImageType::layout);

	public:
		static inline constexpr std::uint8_t channels = ((__layout_raw & 0xC0) >> 6) + 1;
		static inline constexpr std::uint8_t base     = __layout_raw & 0x0F;

		static inline constexpr bool is_single_channel    = channels == 1;
		static inline constexpr bool is_multiple_channels = false;

		static inline constexpr bool is_pallet
			= is_single_channel && ImageType::format == pixel_format::index;
		static inline constexpr bool is_grayscale
			= is_single_channel && ImageType::format == pixel_format::grayscale;
		static inline constexpr bool is_rgb    = is_multiple_channels;
		static constexpr bool        has_alpha = channels == 4;

		static inline constexpr bool is_continuous = !is_pallet;
	};

	struct image_primitive {
		image_primitive(
			pixel_layout       layout,
			pixel_format       format,
			inknit::colorspace colorspace,
			inknit::colorrange colorrange,
			inknit::alignment  alignment,
			std::uint16_t      width,
			std::uint16_t      height,
			std::uint16_t      stride,
			void              *data
		)
			: pixelLayout_(layout)
			, pixelFormat_(format)
			, colorspace_(colorspace)
			, colorrange_(colorrange)
			, stride_(stride)
			, alignment_(alignment)
			, width_(width & 0x1FFF)
			, height_(height)
			, data_(data) {}

		inline constexpr inknit::colorrange colorrange() const noexcept {
			return colorrange_;
		}
		inline constexpr inknit::colorspace colorspace() const noexcept {
			return colorspace_;
		}
		inline constexpr pixel_format format() const noexcept {
			return pixelFormat_;
		}
		inline constexpr pixel_layout layout() const noexcept {
			return pixelLayout_;
		}

		inline constexpr inknit::alignment alignment() const noexcept {
			return alignment_;
		}
		inline constexpr std::int32_t height() const noexcept {
			return static_cast<std::int32_t>(height_);
		}
		inline constexpr std::int32_t stride() const noexcept {
			return static_cast<std::int32_t>(stride_);
		}
		inline constexpr std::int32_t width() const noexcept {
			return static_cast<std::int32_t>(width_);
		}

		inline inknit_image *operator&() noexcept {
			return reinterpret_cast<inknit_image *>(this);
		}
		inline inknit_image const *operator&() const noexcept {
			return reinterpret_cast<inknit_image const *>(this);
		}

	protected:
		inline inknit_image *get() noexcept {
			return reinterpret_cast<inknit_image *>(this);
		}

		inline inknit_image const *get() const noexcept {
			return reinterpret_cast<inknit_image const *>(this);
		}

	protected:
		pixel_layout const       pixelLayout_;
		pixel_format const       pixelFormat_ : 4;
		inknit::colorspace const colorspace_  : 3;
		inknit::colorrange const colorrange_  : 1;

		std::uint16_t           stride_;
		inknit::alignment const alignment_ : 3;
		std::uint32_t           width_     : 13;
		std::uint16_t           height_;
		void                   *data_;
	};

}  // namespace details
}  // namespace inknit
