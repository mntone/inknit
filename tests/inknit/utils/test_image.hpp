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

#include <concepts>  // requires
#include <optional>  // optional

#include "inknit.hpp"
#include "inknit/format.hpp"

#include "macro.hpp"
#include "primitive.hpp"

namespace inknit::tests {

namespace details {

	template<class Float, class... Args>
	concept color_invocable = std::regular_invocable<Float, Args...>
						   && std::same_as<std::invoke_result_t<Float, Args...>, color_t>;

	class test_image_base {
	public:
		test_image_base() noexcept;

		explicit test_image_base(std::unique_ptr<format::image_formatter> formatter) noexcept
			: formatter_(std::move(formatter)) {}

	protected:
		inline std::string str(inknit_image const& image) const noexcept {
			std::string text = formatter_->format(image);
			return std::move(text);
		}

		std::optional<std::string> test(
			inknit_image const& image,
			std::int32_t        x,
			std::int32_t        y,
			color_t             actual,
			color_t             expected
		) const noexcept;

	private:
		std::unique_ptr<format::image_formatter> formatter_;
	};

}  // namespace details


template<pixel_layout PIXEL_LAYOUT, pixel_format PIXEL_FORMAT>
struct test_image
	: public image<PIXEL_LAYOUT, PIXEL_FORMAT>
	, details::test_image_base {
	test_image() noexcept
		: image<PIXEL_LAYOUT, PIXEL_FORMAT>()
		, details::test_image_base(
			  std::make_unique<format::auto_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>()
		  ) {}

	constexpr explicit test_image(std::uint16_t width, std::uint16_t height, color_t color = 0)
		: image<PIXEL_LAYOUT, PIXEL_FORMAT>(width, height, color) {}

	inline std::string str() const noexcept {
		std::string text = details::test_image_base::str(*this->get());
		return std::move(text);
	}

	constexpr rect_t rect() const noexcept {
		rect_t rect {
			0,
			0,
			static_cast<std::int32_t>(this->width_),
			static_cast<std::int32_t>(this->height_),
		};
		return rect;
	}

	template<class Function>
		requires details::color_invocable<Function, std::int32_t, std::int32_t>
	void test(Function fn) const noexcept {
		inknit_image const& image  = *this->get();
		std::int32_t const  height = this->height();
		std::int32_t const  width  = this->width();
		for (std::int32_t y = 0; y < height; ++y) {
			for (std::int32_t x = 0; x < width; ++x) {
				color_t actual   = this->at(x, y);
				color_t expected = fn(x, y);

				auto const& msg = details::test_image_base::test(image, x, y, actual, expected);
				if (msg) {
					FAIL(msg.value());
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
	, details::test_image_base {
	fixed_test_image() noexcept
		: fixed_image<WIDTH, HEIGHT, PIXEL_LAYOUT, PIXEL_FORMAT>()
		, details::test_image_base(
			  std::make_unique<format::auto_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>()
		  ) {}

	explicit fixed_test_image(color_t color) noexcept
		: fixed_image<WIDTH, HEIGHT, PIXEL_LAYOUT, PIXEL_FORMAT>(color)
		, details::test_image_base(
			  std::make_unique<format::auto_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>()
		  ) {}

	inline std::string str() const noexcept {
		std::string text = details::test_image_base::str(*this->get());
		return std::move(text);
	}

	constexpr rect_t rect() const noexcept {
		rect_t rect {
			0,
			0,
			WIDTH,
			HEIGHT,
		};
		return rect;
	}

	template<class Function>
		requires details::color_invocable<Function, std::int32_t, std::int32_t>
	void test(Function fn) const noexcept {
		inknit_image const& image = *this->get();
		for (std::int32_t y = 0; y < HEIGHT; ++y) {
			for (std::int32_t x = 0; x < WIDTH; ++x) {
				color_t actual   = this->at(x, y);
				color_t expected = fn(x, y);

				auto const& msg = details::test_image_base::test(image, x, y, actual, expected);
				if (msg) {
					FAIL(msg.value());
					return;
				}
			}
		}
	}
};

}  // namespace inknit::tests
