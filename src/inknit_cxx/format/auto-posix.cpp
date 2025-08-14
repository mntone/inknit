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

#include <cstdlib>  // getenv


// -- MARK: use namespaces
using namespace inknit;
using namespace inknit::format;


// -- MARK: utils
static bool supports_256color() noexcept {
	std::string_view term = getenv("TERM");
	return term.ends_with("256color");
}

template<pixel_layout PIXEL_LAYOUT, pixel_format PIXEL_FORMAT>
static std::shared_ptr<format::image_formatter> make_formatter() noexcept {
	static std::shared_ptr<format::image_formatter> formatter;

	if (!formatter) {
		if (supports_256color()) {
			formatter = std::make_unique<format::block_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>();
		} else {
			formatter = std::make_unique<format::character_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>();
		}
	}
	return formatter;
}


// -- MARK: layout=x1, format=grayscale

template<>
auto_formatter<pixel_layout::x1, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x1, pixel_format::grayscale>()) {}


// -- MARK: layout=x1lsb, format=grayscale

template<>
auto_formatter<pixel_layout::x1lsb, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x1lsb, pixel_format::grayscale>()) {}


// -- MARK: layout=x2, format=grayscale

template<>
auto_formatter<pixel_layout::x2, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x2, pixel_format::grayscale>()) {}


// -- MARK: layout=x2lsb, format=grayscale

template<>
auto_formatter<pixel_layout::x2lsb, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x2lsb, pixel_format::grayscale>()) {}


// -- MARK: layout=x4, format=grayscale

template<>
auto_formatter<pixel_layout::x4, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x4, pixel_format::grayscale>()) {}


// -- MARK: layout=x4lsb, format=grayscale

template<>
auto_formatter<pixel_layout::x4lsb, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x4lsb, pixel_format::grayscale>()) {}
