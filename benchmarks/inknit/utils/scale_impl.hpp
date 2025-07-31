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

#include "types.hpp"  // number, includes <string_view>
#include "scale.hpp"  // includes <concepts>

#include <charconv>  // to_chars
#include <cmath>     // ceil, floor, log10, pow
#include <cstdint>   // uint32_t
#include <string>    // string

namespace inkbm::scale {

namespace details {

	// Time unit constants
	constexpr std::string_view UNIT_NANOSECONDS  = "ns";
	constexpr std::string_view UNIT_MICROSECONDS = "μs";
	constexpr std::string_view UNIT_MILLISECONDS = "ms";
	constexpr std::string_view UNIT_SECONDS      = "s";

	// Function pointer types for scaling and formatting.
	template<std::floating_point Float>
	using format_type = std::string (*)(Float);

	// Convert a double value to a string with a specific precision.
	template<int Precision, std::floating_point Float>
	static inline constexpr std::string format(Float val) noexcept {
		std::string buf(8, '\0');

		auto begin     = buf.data();
		auto [ptr, ec] = std::to_chars(begin, begin + 8, val, std::chars_format::fixed, Precision);
		if (ec == std::errc::value_too_large) {
			INKBM_FATAL("Missing number conversion.");
		}
		buf.resize(static_cast<std::size_t>(ptr - begin));
		return std::move(buf);
	}

	template<std::floating_point Float>
	constexpr bool nearly_equal(Float a, Float b) {
		Float const min_a = std::nextafter(a, std::numeric_limits<Float>::lowest());
		Float const max_a = std::nextafter(a, std::numeric_limits<Float>::max());
		return min_a <= b && b <= max_a;
	}

}  // namespace details


// -- MARK: time scaling functions
// Convert nanoseconds to a floating point representation of a specific unit.
template<std::floating_point Float, number Number>
static inline constexpr Float scale_nanoseconds(Number val) noexcept {
	return static_cast<Float>(val);
}
template<std::floating_point Float, number Number>
static inline constexpr Float scale_microseconds(Number val) noexcept {
	return 0.001 * static_cast<Float>(val);
}
template<std::floating_point Float, number Number>
static inline constexpr Float scale_milliseconds(Number val) noexcept {
	return 0.000001 * static_cast<Float>(val);
}
template<std::floating_point Float, number Number>
static inline constexpr Float scale_seconds(Number val) noexcept {
	return 0.000000001 * static_cast<Float>(val);
}


// -- MARK: formatting functions
// Determines the preferred time unit (ns, us, ms, s) and formatting functions
// based on the magnitude of the maximum value.
std::string format_string(double val) noexcept;

template<std::floating_point Float>
static inline constexpr std::tuple<
	scale_type<double, Float>,
	details::format_type<Float>,
	details::format_type<Float>,
	std::string_view>
preferred_formatter(double val) noexcept {
	if (val >= 1'000'000'000'000 /* 100s */) {
		return {scale_seconds, details::format<0>, details::format<0>, details::UNIT_SECONDS};
	}
	if (val >= 100'000'000'000 /* 10.0s */) {
		return {scale_seconds, details::format<0>, details::format<1>, details::UNIT_SECONDS};
	}
	if (val >= 10'000'000'000 /* 1.00s */) {
		return {scale_seconds, details::format<1>, details::format<2>, details::UNIT_SECONDS};
	}
	if (val >= 100'000'000 /* 100ms */) {
		return {
			scale_milliseconds, details::format<0>, details::format<0>, details::UNIT_MILLISECONDS
		};
	}
	if (val >= 10'000'000 /* 10.0ms */) {
		return {
			scale_milliseconds, details::format<0>, details::format<1>, details::UNIT_MILLISECONDS
		};
	}
	if (val >= 1'000'000 /* 1.00ms */) {
		return {
			scale_milliseconds, details::format<1>, details::format<2>, details::UNIT_MILLISECONDS
		};
	}
	if (val >= 100000 /* 100us */) {
		return {
			scale_microseconds, details::format<0>, details::format<0>, details::UNIT_MICROSECONDS
		};
	}
	if (val >= 10000 /* 10.0us */) {
		return {
			scale_microseconds, details::format<0>, details::format<1>, details::UNIT_MICROSECONDS
		};
	}
	if (val >= 1000 /* 1.00us */) {
		return {
			scale_microseconds, details::format<1>, details::format<2>, details::UNIT_MICROSECONDS
		};
	}
	return {scale_nanoseconds, details::format<0>, details::format<0>, details::UNIT_NANOSECONDS};
}


// Calculates "nice" minimum and maximum values for a chart axis,
// ensuring the range is human-readable and covers the data range.
template<std::floating_point Float, number Number, std::integral Ticks = std::uint32_t>
static inline constexpr minmax<Float> nice(
	Number min, Number max, Ticks ticks, scale_type<Number, Float> scale, Float ratio = 0.05
) noexcept {
	if (details::nearly_equal(min, max)) {
		return {min, max};
	};

	Float const minimum = scale(min);
	Float const maximum = scale(max);

	Float const padding      = (maximum - minimum) * ratio;
	Float const extended_min = minimum - padding;
	Float const extended_max = maximum + padding;

	Float const rough_step = (extended_max - extended_min) / static_cast<Float>(ticks);
	Float const magnitude  = std::pow(10, std::floor(std::log10(rough_step)));
	Float const residual   = rough_step / magnitude;

	Float step;
	if (residual <= 1) {
		step = 1;
	} else if (residual <= 2) {
		step = 2;
	} else if (residual <= 5) {
		step = 5;
	} else {
		step = 10;
	}
	step *= magnitude;

	Float const nice_min = std::floor(minimum / step) * step;
	Float const nice_max = std::ceil(maximum / step) * step;
	return {nice_min, nice_max};
}

}  //namespace inkbm::scale
