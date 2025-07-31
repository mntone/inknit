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
#include "types.hpp"
#include "scale.hpp"

#include <optional>  // optional
#include <string>    // string
#include <vector>    // vector

namespace inkbm::charts {

class boxplot_chart final {
public:
	using stat_type         = stat_value;
	using ticks_type        = std::uint32_t;
	using intermediate_type = double;

	using scale_type = scale::scale_type<stat_type, intermediate_type>;

	boxplot_chart() noexcept {}

	INKBM_ALWAYS_INLINE
	constexpr boxplot_chart& add_dataset(std::string name, statistics dataset) noexcept {
		datasets_.emplace_back(std::move(name), std::move(dataset));
		return *this;
	}

	INKBM_ALWAYS_INLINE
	constexpr std::uint32_t chart_width() const noexcept {
		return chart_width_.value_or(64);
	}
	INKBM_ALWAYS_INLINE
	constexpr boxplot_chart& chart_width(std::uint32_t width) noexcept {
		chart_width_ = width;
		return *this;
	}

	INKBM_ALWAYS_INLINE
	constexpr bool high_precision() const noexcept {
		return use_high_precision_;
	}
	INKBM_ALWAYS_INLINE
	constexpr boxplot_chart& high_precision(bool high_precision) noexcept {
		use_high_precision_ = high_precision;
		return *this;
	}

	INKBM_ALWAYS_INLINE
	constexpr std::optional<std::uint32_t> label_width() const noexcept {
		return label_width_;
	}
	INKBM_ALWAYS_INLINE
	constexpr boxplot_chart& label_width(std::optional<std::uint32_t> width) noexcept {
		label_width_ = width;
		return *this;
	}

	INKBM_ALWAYS_INLINE
	constexpr std::optional<stat_type> max() const noexcept {
		return max_val_;
	}
	INKBM_ALWAYS_INLINE
	constexpr boxplot_chart& max(std::optional<stat_type> max) noexcept {
		max_val_ = max;
		return *this;
	}

	INKBM_ALWAYS_INLINE
	constexpr std::optional<stat_type> min() const noexcept {
		return min_val_;
	}
	INKBM_ALWAYS_INLINE
	constexpr boxplot_chart& min(std::optional<stat_type> min) noexcept {
		min_val_ = min;
		return *this;
	}

	INKBM_ALWAYS_INLINE
	constexpr boxplot_chart& range(stat_type min, stat_type max) noexcept {
		min_val_ = min;
		max_val_ = max;
		return *this;
	}

	INKBM_ALWAYS_INLINE
	constexpr void reset() noexcept {
		datasets_.clear();
	}

	void render(std::ostream& os) const;

	INKBM_ALWAYS_INLINE
	constexpr std::optional<ticks_type> ticks() const noexcept {
		return ticks_;
	}
	INKBM_ALWAYS_INLINE
	constexpr boxplot_chart& ticks(std::optional<ticks_type> ticks) noexcept {
		ticks_ = ticks;
		return *this;
	}

private:
	bool                         use_high_precision_ {true};
	std::optional<std::uint32_t> chart_width_, label_width_;
	std::optional<stat_type>     min_val_, max_val_;
	std::optional<ticks_type>    ticks_;

	std::vector<std::pair<std::string, statistics>> datasets_;
};

}  // namespace inkbm::charts
