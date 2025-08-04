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

	template<pixel_layout PIXEL_LAYOUT>
	struct drawable_image: std::false_type {
		static_assert(!value, "This class has to be implemented for desired type");
	};

	template<>
	struct drawable_image<pixel_layout::x1>
		: public image_primitive
		, std::true_type {
		drawable_image(
			pixel_format       format,
			inknit::colorspace colorspace,
			inknit::colorrange colorrange,
			inknit::alignment  align,
			std::uint16_t      width,
			std::uint16_t      height,
			std::uint16_t      stride,
			void              *data
		)
			: image_primitive(
				  pixel_layout::x1,
				  format,
				  colorspace,
				  colorrange,
				  align,
				  width,
				  height,
				  stride,
				  data
			  ) {}

		color_t at(std::int32_t x, std::int32_t y) const noexcept {
			return INKNIT_X1_NAME(get_pixel)(get(), x, y);
		}

		void blit(
			std::int32_t           dx,
			std::int32_t           dy,
			image_primitive const& source,
			std::int32_t           sx,
			std::int32_t           sy,
			std::int32_t           width,
			std::int32_t           height
		) noexcept {
			INKNIT_X1_NAME(blit)(
				get(),
				static_cast<uint_t>(dx),
				static_cast<uint_t>(dy),
				&source,
				static_cast<uint_t>(sx),
				static_cast<uint_t>(sy),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height)
			);
		}

		void clear(color_t color) noexcept {
			INKNIT_X1_NAME(clear)(get(), color);
		}

		void draw_point(std::int32_t x, std::int32_t y, color_t color) noexcept {
			INKNIT_X1_NAME(draw_point)(get(), x, y, color);
		}

		void draw_hline(std::int32_t x1, std::int32_t x2, std::int32_t y, color_t color) noexcept {
			INKNIT_X1_NAME(draw_hline)(get(), x1, x2, y, color);
		}

		void draw_vline(std::int32_t x, std::int32_t y1, std::int32_t y2, color_t color) noexcept {
			INKNIT_X1_NAME(draw_vline)(get(), x, y1, y2, color);
		}

		void draw_line(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X1_NAME(draw_line)(get(), x1, y1, x2, y2, color);
		}

		void draw_rect(point_t pt, size_t sz, color_t color) noexcept {
			auto [x, y]          = pt;
			auto [width, height] = sz;
			INKNIT_X1_NAME(draw_rect)(
				get(),
				static_cast<uint_t>(x),
				static_cast<uint_t>(y),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height),
				color
			);
		}

		void draw_rectp(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X1_NAME(draw_rectp)(
				get(),
				static_cast<uint_t>(x1),
				static_cast<uint_t>(y1),
				static_cast<uint_t>(x2),
				static_cast<uint_t>(y2),
				color
			);
		}

		void draw_circle(point_t ct, std::int32_t radius, color_t color) noexcept {
			auto [x, y] = ct;
			INKNIT_X1_NAME(draw_circle)(get(), x, y, radius, color);
		}

	protected:
		static color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b1;
#ifdef __ARM_ARCH_6M__
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
	struct drawable_image<pixel_layout::x1lsb>
		: public image_primitive
		, std::true_type {
		drawable_image(
			pixel_format       format,
			inknit::colorspace colorspace,
			inknit::colorrange colorrange,
			inknit::alignment  align,
			std::uint16_t      width,
			std::uint16_t      height,
			std::uint16_t      stride,
			void              *data
		)
			: image_primitive(
				  pixel_layout::x1lsb,
				  format,
				  colorspace,
				  colorrange,
				  align,
				  width,
				  height,
				  stride,
				  data
			  ) {}

		color_t at(std::int32_t x, std::int32_t y) const noexcept {
			return INKNIT_X1LSB_NAME(get_pixel)(get(), x, y);
		}

		void blit(
			std::int32_t           dx,
			std::int32_t           dy,
			image_primitive const& source,
			std::int32_t           sx,
			std::int32_t           sy,
			std::int32_t           width,
			std::int32_t           height
		) noexcept {
			INKNIT_X1LSB_NAME(blit)(
				get(),
				static_cast<uint_t>(dx),
				static_cast<uint_t>(dy),
				&source,
				static_cast<uint_t>(sx),
				static_cast<uint_t>(sy),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height)
			);
		}

		void clear(color_t color) noexcept {
			INKNIT_X1LSB_NAME(clear)(get(), color);
		}

		void draw_point(std::int32_t x, std::int32_t y, color_t color) noexcept {
			INKNIT_X1LSB_NAME(draw_point)(get(), x, y, color);
		}

		void draw_hline(std::int32_t x1, std::int32_t x2, std::int32_t y, color_t color) noexcept {
			INKNIT_X1LSB_NAME(draw_hline)(get(), x1, x2, y, color);
		}

		void draw_vline(std::int32_t x, std::int32_t y1, std::int32_t y2, color_t color) noexcept {
			INKNIT_X1LSB_NAME(draw_vline)(get(), x, y1, y2, color);
		}

		void draw_line(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X1LSB_NAME(draw_line)(get(), x1, y1, x2, y2, color);
		}

		void draw_rect(point_t pt, size_t sz, color_t color) noexcept {
			auto [x, y]          = pt;
			auto [width, height] = sz;
			INKNIT_X1LSB_NAME(draw_rect)(
				get(),
				static_cast<uint_t>(x),
				static_cast<uint_t>(y),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height),
				color
			);
		}

		void draw_rectp(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X1LSB_NAME(draw_rectp)(
				get(),
				static_cast<uint_t>(x1),
				static_cast<uint_t>(y1),
				static_cast<uint_t>(x2),
				static_cast<uint_t>(y2),
				color
			);
		}

		void draw_circle(point_t ct, std::int32_t radius, color_t color) noexcept {
			auto [x, y] = ct;
			INKNIT_X1LSB_NAME(draw_circle)(get(), x, y, radius, color);
		}

	protected:
		static color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b1;
#ifdef __ARM_ARCH_6M__
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
	struct drawable_image<pixel_layout::x2>
		: public image_primitive
		, std::true_type {
		drawable_image(
			pixel_format       format,
			inknit::colorspace colorspace,
			inknit::colorrange colorrange,
			inknit::alignment  align,
			std::uint16_t      width,
			std::uint16_t      height,
			std::uint16_t      stride,
			void              *data
		)
			: image_primitive(
				  pixel_layout::x2,
				  format,
				  colorspace,
				  colorrange,
				  align,
				  width,
				  height,
				  stride,
				  data
			  ) {}

		color_t at(std::int32_t x, std::int32_t y) const noexcept {
			return INKNIT_X2_NAME(get_pixel)(get(), x, y);
		}

		void blit(
			std::int32_t           dx,
			std::int32_t           dy,
			image_primitive const& source,
			std::int32_t           sx,
			std::int32_t           sy,
			std::int32_t           width,
			std::int32_t           height
		) noexcept {
			INKNIT_X2_NAME(blit)(
				get(),
				static_cast<uint_t>(dx),
				static_cast<uint_t>(dy),
				&source,
				static_cast<uint_t>(sx),
				static_cast<uint_t>(sy),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height)
			);
		}

		void clear(color_t color) noexcept {
			INKNIT_X2_NAME(clear)(get(), color);
		}

		void draw_point(std::int32_t x, std::int32_t y, color_t color) noexcept {
			INKNIT_X2_NAME(draw_point)(get(), x, y, color);
		}

		void draw_hline(std::int32_t x1, std::int32_t x2, std::int32_t y, color_t color) noexcept {
			INKNIT_X2_NAME(draw_hline)(get(), x1, x2, y, color);
		}

		void draw_vline(std::int32_t x, std::int32_t y1, std::int32_t y2, color_t color) noexcept {
			INKNIT_X2_NAME(draw_vline)(get(), x, y1, y2, color);
		}

		void draw_line(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X2_NAME(draw_line)(get(), x1, y1, x2, y2, color);
		}

		void draw_rect(point_t pt, size_t sz, color_t color) noexcept {
			auto [x, y]          = pt;
			auto [width, height] = sz;
			INKNIT_X2_NAME(draw_rect)(
				get(),
				static_cast<uint_t>(x),
				static_cast<uint_t>(y),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height),
				color
			);
		}

		void draw_rectp(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X2_NAME(draw_rectp)(
				get(),
				static_cast<uint_t>(x1),
				static_cast<uint_t>(y1),
				static_cast<uint_t>(x2),
				static_cast<uint_t>(y2),
				color
			);
		}

		void draw_circle(point_t ct, std::int32_t radius, color_t color) noexcept {
			auto [x, y] = ct;
			INKNIT_X2_NAME(draw_circle)(get(), x, y, radius, color);
		}

	protected:
		static color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b11;
#ifdef __ARM_ARCH_6M__
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
	struct drawable_image<pixel_layout::x2lsb>
		: public image_primitive
		, std::true_type {
		drawable_image(
			pixel_format       format,
			inknit::colorspace colorspace,
			inknit::colorrange colorrange,
			inknit::alignment  align,
			std::uint16_t      width,
			std::uint16_t      height,
			std::uint16_t      stride,
			void              *data
		)
			: image_primitive(
				  pixel_layout::x2lsb,
				  format,
				  colorspace,
				  colorrange,
				  align,
				  width,
				  height,
				  stride,
				  data
			  ) {}

		color_t at(std::int32_t x, std::int32_t y) const noexcept {
			return INKNIT_X2LSB_NAME(get_pixel)(get(), x, y);
		}

		void blit(
			std::int32_t           dx,
			std::int32_t           dy,
			image_primitive const& source,
			std::int32_t           sx,
			std::int32_t           sy,
			std::int32_t           width,
			std::int32_t           height
		) noexcept {
			INKNIT_X2LSB_NAME(blit)(
				get(),
				static_cast<uint_t>(dx),
				static_cast<uint_t>(dy),
				&source,
				static_cast<uint_t>(sx),
				static_cast<uint_t>(sy),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height)
			);
		}

		void clear(color_t color) noexcept {
			INKNIT_X2LSB_NAME(clear)(get(), color);
		}

		void draw_point(std::int32_t x, std::int32_t y, color_t color) noexcept {
			INKNIT_X2LSB_NAME(draw_point)(get(), x, y, color);
		}

		void draw_hline(std::int32_t x1, std::int32_t x2, std::int32_t y, color_t color) noexcept {
			INKNIT_X2LSB_NAME(draw_hline)(get(), x1, x2, y, color);
		}

		void draw_vline(std::int32_t x, std::int32_t y1, std::int32_t y2, color_t color) noexcept {
			INKNIT_X2LSB_NAME(draw_vline)(get(), x, y1, y2, color);
		}

		void draw_line(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X2LSB_NAME(draw_line)(get(), x1, y1, x2, y2, color);
		}

		void draw_rect(point_t pt, size_t sz, color_t color) noexcept {
			auto [x, y]          = pt;
			auto [width, height] = sz;
			INKNIT_X2LSB_NAME(draw_rect)(
				get(),
				static_cast<uint_t>(x),
				static_cast<uint_t>(y),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height),
				color
			);
		}

		void draw_rectp(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X2LSB_NAME(draw_rectp)(
				get(),
				static_cast<uint_t>(x1),
				static_cast<uint_t>(y1),
				static_cast<uint_t>(x2),
				static_cast<uint_t>(y2),
				color
			);
		}

		void draw_circle(point_t ct, std::int32_t radius, color_t color) noexcept {
			auto [x, y] = ct;
			INKNIT_X2LSB_NAME(draw_circle)(get(), x, y, radius, color);
		}

	protected:
		static color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b11;
#ifdef __ARM_ARCH_6M__
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
	struct drawable_image<pixel_layout::x4>
		: public image_primitive
		, std::true_type {
		drawable_image(
			pixel_format       format,
			inknit::colorspace colorspace,
			inknit::colorrange colorrange,
			inknit::alignment  align,
			std::uint16_t      width,
			std::uint16_t      height,
			std::uint16_t      stride,
			void              *data
		)
			: image_primitive(
				  pixel_layout::x4,
				  format,
				  colorspace,
				  colorrange,
				  align,
				  width,
				  height,
				  stride,
				  data
			  ) {}

		color_t at(std::int32_t x, std::int32_t y) const noexcept {
			return INKNIT_X4_NAME(get_pixel)(get(), x, y);
		}

		void blit(
			std::int32_t           dx,
			std::int32_t           dy,
			image_primitive const& source,
			std::int32_t           sx,
			std::int32_t           sy,
			std::int32_t           width,
			std::int32_t           height
		) noexcept {
			INKNIT_X4_NAME(blit)(
				get(),
				static_cast<uint_t>(dx),
				static_cast<uint_t>(dy),
				&source,
				static_cast<uint_t>(sx),
				static_cast<uint_t>(sy),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height)
			);
		}

		void clear(color_t color) noexcept {
			INKNIT_X4_NAME(clear)(get(), color);
		}

		void draw_point(std::int32_t x, std::int32_t y, color_t color) noexcept {
			INKNIT_X4_NAME(draw_point)(get(), x, y, color);
		}

		void draw_hline(std::int32_t x1, std::int32_t x2, std::int32_t y, color_t color) noexcept {
			INKNIT_X4_NAME(draw_hline)(get(), x1, x2, y, color);
		}

		void draw_vline(std::int32_t x, std::int32_t y1, std::int32_t y2, color_t color) noexcept {
			INKNIT_X4_NAME(draw_vline)(get(), x, y1, y2, color);
		}

		void draw_line(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X4_NAME(draw_line)(get(), x1, y1, x2, y2, color);
		}

		void draw_rect(point_t pt, size_t sz, color_t color) noexcept {
			auto [x, y]          = pt;
			auto [width, height] = sz;
			INKNIT_X4_NAME(draw_rect)(
				get(),
				static_cast<uint_t>(x),
				static_cast<uint_t>(y),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height),
				color
			);
		}

		void draw_rectp(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X4_NAME(draw_rectp)(
				get(),
				static_cast<uint_t>(x1),
				static_cast<uint_t>(y1),
				static_cast<uint_t>(x2),
				static_cast<uint_t>(y2),
				color
			);
		}

		void draw_circle(point_t ct, std::int32_t radius, color_t color) noexcept {
			auto [x, y] = ct;
			INKNIT_X4_NAME(draw_circle)(get(), x, y, radius, color);
		}

	protected:
		static color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b11;
#ifdef __ARM_ARCH_6M__
			word |= word << 4;
			word |= word << 8;
			word |= word << 16;
#else
			word *= 0x11111111u;
#endif
			return word;
		}
	};

	template<>
	struct drawable_image<pixel_layout::x4lsb>
		: public image_primitive
		, std::true_type {
		drawable_image(
			pixel_format       format,
			inknit::colorspace colorspace,
			inknit::colorrange colorrange,
			inknit::alignment  align,
			std::uint16_t      width,
			std::uint16_t      height,
			std::uint16_t      stride,
			void              *data
		)
			: image_primitive(
				  pixel_layout::x4lsb,
				  format,
				  colorspace,
				  colorrange,
				  align,
				  width,
				  height,
				  stride,
				  data
			  ) {}

		color_t at(std::int32_t x, std::int32_t y) const noexcept {
			return INKNIT_X4LSB_NAME(get_pixel)(get(), x, y);
		}

		void blit(
			std::int32_t           dx,
			std::int32_t           dy,
			image_primitive const& source,
			std::int32_t           sx,
			std::int32_t           sy,
			std::int32_t           width,
			std::int32_t           height
		) noexcept {
			INKNIT_X4LSB_NAME(blit)(
				get(),
				static_cast<uint_t>(dx),
				static_cast<uint_t>(dy),
				&source,
				static_cast<uint_t>(sx),
				static_cast<uint_t>(sy),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height)
			);
		}

		void clear(color_t color) noexcept {
			INKNIT_X4LSB_NAME(clear)(get(), color);
		}

		void draw_point(std::int32_t x, std::int32_t y, color_t color) noexcept {
			INKNIT_X4LSB_NAME(draw_point)(get(), x, y, color);
		}

		void draw_hline(std::int32_t x1, std::int32_t x2, std::int32_t y, color_t color) noexcept {
			INKNIT_X4LSB_NAME(draw_hline)(get(), x1, x2, y, color);
		}

		void draw_vline(std::int32_t x, std::int32_t y1, std::int32_t y2, color_t color) noexcept {
			INKNIT_X4LSB_NAME(draw_vline)(get(), x, y1, y2, color);
		}

		void draw_line(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X4LSB_NAME(draw_line)(get(), x1, y1, x2, y2, color);
		}

		void draw_rect(point_t pt, size_t sz, color_t color) noexcept {
			auto [x, y]          = pt;
			auto [width, height] = sz;
			INKNIT_X4LSB_NAME(draw_rect)(
				get(),
				static_cast<uint_t>(x),
				static_cast<uint_t>(y),
				static_cast<uint_t>(width),
				static_cast<uint_t>(height),
				color
			);
		}

		void draw_rectp(point_t pt1, point_t pt2, color_t color) noexcept {
			auto [x1, y1] = pt1;
			auto [x2, y2] = pt2;
			INKNIT_X4LSB_NAME(draw_rectp)(
				get(),
				static_cast<uint_t>(x1),
				static_cast<uint_t>(y1),
				static_cast<uint_t>(x2),
				static_cast<uint_t>(y2),
				color
			);
		}

		void draw_circle(point_t ct, std::int32_t radius, color_t color) noexcept {
			auto [x, y] = ct;
			INKNIT_X4LSB_NAME(draw_circle)(get(), x, y, radius, color);
		}

	protected:
		static color_t get_fill_word(color_t color) noexcept {
			color_t word = color & 0b11;
#ifdef __ARM_ARCH_6M__
			word |= word << 4;
			word |= word << 8;
			word |= word << 16;
#else
			word *= 0x11111111u;
#endif
			return word;
		}
	};

}}  // namespace inknit::details
