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

#include "test_image.hpp"

#include <cinttypes>  // PRId32
#include <sstream>    // ostringstream

using namespace inknit;
using namespace inknit::tests;
using namespace inknit::tests::details;

std::optional<std::string> test_image_base::test(
	inknit_image const& image, std::int32_t x, std::int32_t y, color_t actual, color_t expected
) const noexcept {
	if (actual == expected) {
		return std::nullopt;
	}

	char buffer[128];
	snprintf(
		buffer,
		sizeof(buffer) / sizeof(buffer[0]),
		"Unexpected value at (%" PRId32 ", %" PRId32 "): got %s, expected %s\n",
		x,
		y,
		formatter_->name(actual).data(),
		formatter_->name(expected).data()
	);

	std::ostringstream oss;
	oss << buffer << str(image);
	return oss.str();
}
