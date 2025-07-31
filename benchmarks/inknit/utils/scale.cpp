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

#include "scale_impl.hpp"

namespace inkbm::scale::details {

constexpr std::string_view UNIT_NANOSECONDS_WITH_SPACE  = " ns";
constexpr std::string_view UNIT_MICROSECONDS_WITH_SPACE = " μs";
constexpr std::string_view UNIT_MILLISECONDS_WITH_SPACE = " ms";
constexpr std::string_view UNIT_SECONDS_WITH_SPACE      = " s";

constexpr std::size_t BUFFER_LENGTH = 8;

template<int Precision>
static inline constexpr std::string format(double val, std::string_view unit) noexcept {
	std::string buf(BUFFER_LENGTH, '\0');

	auto begin = buf.data();
	auto [ptr, ec]
		= std::to_chars(begin, begin + BUFFER_LENGTH, val, std::chars_format::fixed, Precision);
	if (ec == std::errc::value_too_large) {
		INKBM_FATAL("Missing number conversion.");
	}

	std::size_t const val_pos  = static_cast<std::size_t>(ptr - begin);
	std::size_t const unit_len = unit.size();
	buf.replace(val_pos, unit_len, unit);
	buf.resize(val_pos + unit_len);
	return std::move(buf);
}

}  // namespace inkbm::scale::details

using namespace inkbm;

std::string inkbm::scale::format_string(double val) noexcept {
	if (val >= 1'000'000'000'000 /* 100.0s */) {
		std::string str {
			details::format<1>(scale_seconds<double>(val), details::UNIT_SECONDS_WITH_SPACE)
		};
		return std::move(str);
	}
	if (val >= 100'000'000'000 /* 10.00s */) {
		std::string str {
			details::format<2>(scale_seconds<double>(val), details::UNIT_SECONDS_WITH_SPACE)
		};
		return std::move(str);
	}
	if (val >= 10'000'000'000 /* 1.000s */) {
		std::string str {
			details::format<3>(scale_seconds<double>(val), details::UNIT_SECONDS_WITH_SPACE)
		};
		return std::move(str);
	}
	if (val >= 100'000'000 /* 100.0ms */) {
		std::string str {details::format<1>(
			scale_milliseconds<double>(val), details::UNIT_MILLISECONDS_WITH_SPACE
		)};
		return std::move(str);
	}
	if (val >= 10'000'000 /* 10.00ms */) {
		std::string str {details::format<2>(
			scale_milliseconds<double>(val), details::UNIT_MILLISECONDS_WITH_SPACE
		)};
		return std::move(str);
	}
	if (val >= 1'000'000 /* 1.000ms */) {
		std::string str {details::format<3>(
			scale_milliseconds<double>(val), details::UNIT_MILLISECONDS_WITH_SPACE
		)};
		return std::move(str);
	}
	if (val >= 100000 /* 100.0us */) {
		std::string str {details::format<1>(
			scale_microseconds<double>(val), details::UNIT_MICROSECONDS_WITH_SPACE
		)};
		return std::move(str);
	}
	if (val >= 10000 /* 10.00us */) {
		std::string str {details::format<2>(
			scale_microseconds<double>(val), details::UNIT_MICROSECONDS_WITH_SPACE
		)};
		return std::move(str);
	}
	if (val >= 1000 /* 1.000us */) {
		std::string str {details::format<3>(
			scale_microseconds<double>(val), details::UNIT_MICROSECONDS_WITH_SPACE
		)};
		return std::move(str);
	}
	std::string str {
		details::format<0>(scale_nanoseconds<double>(val), details::UNIT_NANOSECONDS_WITH_SPACE)
	};
	return std::move(str);
}
