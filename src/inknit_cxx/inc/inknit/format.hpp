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

#include <memory>  // unique_ptr
#include <string>  // string

#include "base.hpp"

namespace inknit { namespace format {

	class image_formatter {
	public:
		virtual ~image_formatter() noexcept {}

		virtual std::string_view name(color_t color) const noexcept               = 0;
		virtual std::string      format(inknit_image const& image) const noexcept = 0;
	};


	// -- MARK: character_formatter

	template<pixel_layout PIXEL_LAYOUT, pixel_format PIXEL_FORMAT>
	class character_formatter final: public image_formatter {
	public:
		character_formatter() noexcept = default;

		std::string_view name(color_t color) const noexcept override;
		std::string      format(inknit_image const& image) const noexcept override;
	};


	// -- MARK: block_formatter

	template<pixel_layout PIXEL_LAYOUT, pixel_format PIXEL_FORMAT>
	class block_formatter final: public image_formatter {
	public:
		block_formatter() noexcept = default;

		std::string_view name(color_t color) const noexcept override;
		std::string      format(inknit_image const& image) const noexcept override;
	};


	// -- MARK: auto_formatter

	template<pixel_layout PIXEL_LAYOUT, pixel_format PIXEL_FORMAT>
	class auto_formatter final: public image_formatter {
	public:
		auto_formatter() noexcept;

		std::string_view name(color_t color) const noexcept override {
			return formatter_->name(color);
		}

		std::string format(inknit_image const& image) const noexcept override {
			return formatter_->format(image);
		}

	private:
		std::shared_ptr<format::image_formatter> formatter_;
	};

}}  // namespace inknit::format
