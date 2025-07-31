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

#include "boxplot.hpp"
#include "scale_impl.hpp"

#include <algorithm>  // min_element, max_element, transform
#include <span>       // span
#include <iostream>   // iostream

namespace inkbm::charts {

// -- MARK: ANSI escape codes for styling
constexpr std::string_view COLOR_SUCCESS  = "\x1b[32m";
constexpr std::string_view COLOR_ERROR    = "\x1b[31m";
constexpr std::string_view COLOR_DARKGRAY = "\x1b[90m";
constexpr std::string_view COLOR_INVERT   = "\x1b[7m";
constexpr std::string_view COLOR_RESET    = "\x1b[0m";


// -- MARK: unicode box drawing character constants
constexpr std::string_view STRING_X                = "\u00D7";
constexpr std::string_view STRING_HLINE            = "\u2500";
constexpr std::string_view STRING_HLINE_WITH_TICK  = "\u2534";
constexpr std::string_view STRING_VLINE            = "\u2502";
constexpr std::string_view STRING_VLINE_WITH_COLOR = "\x1b[90m\u2502\x1b[0m";
constexpr std::string_view STRING_TOP_LEFT         = "\u250C";
constexpr std::string_view STRING_TOP_RIGHT        = "\u2510";
constexpr std::string_view STRING_BOTTOM_RIGHT     = "\u256F";
constexpr std::string_view STRING_BOTTOM_LEFT      = "\u2570";


// -- MARK: configuration base template
// Provides common functionality for different precision configurations using CRTP.
template<class D>
struct boxplot_configuration_base {
	// Returns a view to the array of block-drawing characters.
	INKBM_ALWAYS_INLINE
	constexpr std::span<std::string_view const> blocks() const noexcept {
		return D::blocks;
	}

	// Snaps a floating-point value (0.0 to 1.0) to a block character index.
	INKBM_ALWAYS_INLINE
	constexpr std::uint32_t snap_block(double val) const noexcept {
		return static_cast<std::uint32_t>(
			static_cast<std::int32_t>(std::round(D::block_precision * val)) + D::block_offset
		);
	}

	// Snaps a floating-point value (0.0 to 1.0) to a vertical line character index.
	INKBM_ALWAYS_INLINE
	constexpr std::uint32_t snap_vline(double val) const noexcept {
		return static_cast<std::uint32_t>(std::round(D::vline_precision * val));
	}

	// Returns a view to the array of vertical-line-drawing characters.
	INKBM_ALWAYS_INLINE
	constexpr std::span<std::string_view const> vlines() const noexcept {
		return D::vlines;
	}
};

// -- MARK: high precision configuration
// Uses Unicode 13.0 characters for 1/8-cell precision.
struct high_precision_boxplot_configuration final
	: boxplot_configuration_base<high_precision_boxplot_configuration> {
	static constexpr std::double_t    block_precision = 8.0;
	static constexpr std::int32_t     block_offset    = 8;
	static constexpr std::string_view blocks[]        = {
        "\u2588",      // -8/8 (full)
        "\U0001FB8B",  // -7/8
        "\U0001FB8A",  // -6/8
        "\U0001FB89",  // -5/8
        "\u2590",      // -4/8 (right half)
        "\U0001FB88",  // -3/8
        "\U0001FB87",  // -2/8
        "\u2595",      // -1/8
        "\x20",        //  0/8 (empty)
        "\u258F",      //  1/8
        "\u258E",      //  2/8
        "\u258D",      //  3/8
        "\u258C",      //  4/8 (left half)
        "\u258B",      //  5/8
        "\u258A",      //  6/8
        "\u2589",      //  7/8
        "\u2588",      //  8/8 (full)
    };

	static constexpr std::double_t    vline_precision = 8.0;
	static constexpr std::string_view vlines[]        = {
        "\u258F",      // 0/8
        "\U0001FB70",  // 1/8
        "\U0001FB71",  // 2/8
        "\U0001FB72",  // 3/8
        STRING_VLINE,  // 4/8
        "\U0001FB73",  // 5/8
        "\U0001FB74",  // 6/8
        "\U0001FB75",  // 7/8
        "\u2595",      // 8/8
    };
};

struct compatible_boxplot_configuration final
	: boxplot_configuration_base<compatible_boxplot_configuration> {
	static constexpr std::double_t    block_precision = 2.0;
	static constexpr std::int32_t     block_offset    = 2;
	static constexpr std::string_view blocks[]        = {
        "\u2588",  // full
        "\u2590",  // right half
        "\u2595",  // right 1/8 (approximation for quarter)
        "\x20",    // empty
        "\u258F",  // left 1/8 (approximation for quarter)
        "\u258C",  // left half
        "\u2588",  // full
    };

	static constexpr std::double_t    vline_precision = 2.0;
	static constexpr std::string_view vlines[]        = {
        "\u258F",      // left edge
        STRING_VLINE,  // center
        "\u2595",      // right edge
    };
};

// std::variant to select a configuration at runtime.
using boxplot_configuration_variant = std::variant<
	boxplot_configuration_base<high_precision_boxplot_configuration>,
	boxplot_configuration_base<compatible_boxplot_configuration>>;


// -- MARK: visitor functions for std::variant
// These functions allow calling the correct configuration's method from the variant.
template<typename... Configs>
static inline constexpr std::uint32_t
snap_block(std::variant<Configs...> const& conf, double val) noexcept {
	return std::visit(
		[val](auto const& c) noexcept -> std::uint32_t {
			return c.snap_block(val);
		},
		conf
	);
}
template<typename... Configs>
static inline constexpr std::span<std::string_view const>
blocks(std::variant<Configs...> const& conf) noexcept {
	return std::visit(
		[](auto const& c) noexcept -> std::span<std::string_view const> {
			return c.blocks();
		},
		conf
	);
}
template<typename... Configs>
static inline constexpr std::span<std::string_view const>
vlines(std::variant<Configs...> const& conf) noexcept {
	return std::visit(
		[](auto const& c) noexcept -> std::span<std::string_view const> {
			return c.vlines();
		},
		conf
	);
}
template<typename... Configs>
static inline constexpr std::uint32_t
snap_vline(std::variant<Configs...> const& conf, double val) noexcept {
	return std::visit(
		[val](auto const& c) noexcept -> std::uint32_t {
			return c.snap_vline(val);
		},
		conf
	);
}

// Represents the state of a single character cell in the plot.
enum class cell_type : std::uint8_t {
	empty,
	block,
	whisker,
	marker,
	median,
	best,
	worst,
};

// A struct to hold the state for one character cell on the graph line.
// Uses a bitfield for memory efficiency.
struct cell final {
	cell_type    type   : 3 {cell_type::empty};  // 3 bits for CellType
	std::uint8_t offset : 5 {0};                 // 5 bits for offset/value
};

}  // namespace inkbm::charts

// Helper to efficiently build a string by repeating a smaller string.
static inline std::string build_repeating_string(std::string_view str, std::size_t count) noexcept {
	std::string result;
	result.reserve(count * str.size());
	for (std::size_t i = 0; i < count; ++i) {
		result.append(str);
	}
	return std::move(result);
}

// Helper to place a numerical label onto a character buffer, centered at a given position.
// It checks for collisions and will not draw if space is already occupied.
static inline void build_value_label(
	std::vector<char>& buffer, std::string label, double pos, int delta = 3
) noexcept {
	int const buffer_length = static_cast<int>(buffer.size());
	int const label_length  = static_cast<int>(label.length());
	int       start_pos     = std::max(static_cast<int>(round(pos)) - (label_length >> 1), 0);

	int const max_pos = buffer_length - label_length;
	if (max_pos < start_pos && start_pos <= max_pos + delta) {
		start_pos = max_pos;
	} else if (max_pos + delta < start_pos) {
		return;
	}

	for (int i = 0; i <= label_length; ++i) {
		if (start_pos + i < buffer_length
			&& buffer[static_cast<std::size_t>(start_pos + i)] != '\x20') {
			return;
		}
	}
	std::copy(label.cbegin(), label.cend(), buffer.begin() + start_pos);
}

// Determines the default number of ticks based on chart width.
static inline inkbm::charts::boxplot_chart::ticks_type
preferred_ticks(std::uint32_t width) noexcept {
	if (width >= 50) {
		return 4;
	} else {
		return 2;
	}
}

using namespace inkbm;
using namespace inkbm::charts;

void boxplot_chart::render(std::ostream& os) const {
	if (datasets_.empty()) {
		return;
	}

	using namespace std;

	// 0. setup variables
	boxplot_configuration_variant const conf
		= use_high_precision_
			? boxplot_configuration_variant {high_precision_boxplot_configuration {}}
			: boxplot_configuration_variant {compatible_boxplot_configuration {}};

	uint32_t const chart_width = this->chart_width();
	uint32_t const label_width = label_width_.value_or(
		static_cast<uint32_t>(
			max_element(
				datasets_.cbegin(), datasets_.cend(), [](auto const& a, auto const& b) noexcept {
					return a.first.length() < b.first.length();
				}
			)->first.length()
		)
	);

	// Determine the min/max range across all datasets if not specified manually.
	stat_type const min_base = min_val_.value_or(
		min_element(
			datasets_.cbegin(), datasets_.cend(), [](auto const& a, auto const& b) noexcept {
				return a.second.min < b.second.min;
			}
		)->second.min
	);
	stat_type const max_base = max_val_.value_or(
		max_element(
			datasets_.cbegin(), datasets_.cend(), [](auto const& a, auto const& b) noexcept {
				return a.second.max < b.second.max;
			}
		)->second.max
	);
	ticks_type const ticks = ticks_.value_or(preferred_ticks(chart_width));

	// Determine preferred unit and create scaling/formatting functions.
	auto const [base_scale, axis_format, label_format, unit]
		= scale::preferred_formatter<intermediate_type>(max_base);
	auto const [min_val, max_val]    = scale::nice(min_base, max_base, ticks, base_scale);
	intermediate_type const diff_val = max_val - min_val;

	auto scale
		= [scale = base_scale, chart_width, min = min_val, diff_val](stat_type val) noexcept {
			  return 0.5 + (chart_width - 1) * (scale(val) - min) / diff_val;
		  };

	// 1. render axis on the top
	string const min_label = axis_format(min_val);
	string const max_label = axis_format(max_val);
	string const label_padding(label_width + 1, '\x20');

	// 1.1 axis value labels
	vector<char> value_content(chart_width + 2, '\x20');
	for (uint32_t t = ticks + 1; t > 0; --t) {
		double const pct   = static_cast<double>(t - 1) / static_cast<double>(ticks);
		double const pos   = 1.0 + round(pct * (chart_width - 1));
		string const label = axis_format(min_val + pct * diff_val);
		build_value_label(value_content, label, pos);
	}
	os
		<< label_padding
		<< COLOR_DARKGRAY
		<< string_view(value_content)
		<< " ["
		<< unit
		<< ']'
		<< COLOR_RESET
		<< endl;

	// 1.2 axis line with ticks
	string const hline     = build_repeating_string(STRING_HLINE, chart_width);
	string       tick_line = hline;
	for (uint32_t t = 0; t <= ticks; ++t) {
		double const pct = static_cast<double>(t) / static_cast<double>(ticks);
		size_t const pos = static_cast<size_t>(round(pct * (chart_width - 1)));
		if (0 <= pos && pos <= chart_width) {
			tick_line.replace(
				pos * STRING_HLINE_WITH_TICK.length(),
				STRING_HLINE_WITH_TICK.length(),
				STRING_HLINE_WITH_TICK
			);
		}
	}
	os
		<< label_padding
		<< COLOR_DARKGRAY
		<< STRING_TOP_LEFT
		<< tick_line
		<< STRING_TOP_RIGHT
		<< COLOR_RESET
		<< endl;

	// 2. render datasets
	auto const& blocks = inkbm::charts::blocks(conf);
	auto const& vlines = inkbm::charts::vlines(conf);

	value_content.resize(chart_width);

	string const space_header(label_width + 1, '\x20');
	for (auto const& [name, data] : datasets_) {
		double const min_pos    = scale(data.min);
		double const lfence_pos = scale(data.lfence);
		double const quant1_pos = scale(data.quant1);
		double const median_pos = scale(data.median);
		double const quant3_pos = scale(data.quant3);
		double const ufence_pos = scale(data.ufence);
		double const max_pos    = scale(data.max);

		size_t const lfence_index = static_cast<size_t>(ceil(lfence_pos));
		size_t const quant1_index = static_cast<size_t>(quant1_pos);
		size_t const quant3_index = static_cast<size_t>(quant3_pos);
		size_t const ufence_index = static_cast<size_t>(ceil(ufence_pos));

		vector<cell> buffer(chart_width);

		// 2.1 Priority-based drawing into buffer: whiskers (lowest), box, markers (highest)

		// (a) whiskers
		for (size_t i = lfence_index; i < quant1_index; ++i) {
			if (0 <= i && i < chart_width) {
				buffer[i].type = cell_type::whisker;
			}
		}
		for (size_t i = quant3_index + 1; i < ufence_index; ++i) {
			if (0 <= i && i < chart_width) {
				buffer[i].type = cell_type::whisker;
			}
		}

		// (b) box
		if (quant1_index == quant3_index && 0 <= quant1_index && quant1_index <= chart_width) {
			buffer[quant1_index].type   = cell_type::block;
			buffer[quant1_index].offset = snap_block(conf, quant3_pos - quant1_pos) & 0x1F;
		} else {
			if (0 <= quant1_index && quant1_index < chart_width) {
				buffer[quant1_index].type = cell_type::block;
				buffer[quant1_index].offset
					= snap_block(conf, (quant1_pos - static_cast<double>(quant1_index)) - 1.0)
					& 0x1F;
			}
			for (size_t i = quant1_index + 1; i < quant3_index; ++i) {
				if (0 <= i && i < chart_width) {
					buffer[i].type   = cell_type::block;
					buffer[i].offset = 0u;
				}
			}
			if (0 <= quant3_index && quant3_index < chart_width) {
				buffer[quant3_index].type = cell_type::block;
				buffer[quant3_index].offset
					= snap_block(conf, quant3_pos - static_cast<double>(quant3_index)) & 0x1F;
			}
		}

		// (c) markers
		uint32_t const max_index_floor    = static_cast<uint32_t>(max_pos);
		uint32_t const ufence_index_floor = static_cast<uint32_t>(ufence_pos);
		uint32_t const median_index_floor = static_cast<uint32_t>(median_pos);
		uint32_t const lfence_index_floor = static_cast<uint32_t>(lfence_pos);
		uint32_t const min_index_floor    = static_cast<uint32_t>(min_pos);
		if (0 <= min_index_floor && min_index_floor < chart_width) {
			buffer[min_index_floor].type = cell_type::best;
		}
		if (0 <= lfence_index_floor && lfence_index_floor < chart_width) {
			buffer[lfence_index_floor].type = cell_type::marker;
			buffer[lfence_index_floor].offset
				= snap_vline(conf, lfence_pos - lfence_index_floor) & 0x1F;
		}
		if (0 <= max_index_floor && max_index_floor < chart_width) {
			buffer[max_index_floor].type = cell_type::worst;
		}
		if (0 <= ufence_index_floor && ufence_index_floor < chart_width) {
			buffer[ufence_index_floor].type = cell_type::marker;
			buffer[ufence_index_floor].offset
				= snap_vline(conf, ufence_pos - ufence_index_floor) & 0x1F;
		}
		if (0 <= median_index_floor && median_index_floor < chart_width) {
			buffer[median_index_floor].type = cell_type::median;
			buffer[median_index_floor].offset
				= snap_vline(conf, median_pos - median_index_floor) & 0x1F;
		}

		// 2.2 render the buffer

		// (a) value row
		uint32_t const label_length = static_cast<uint32_t>(name.length());
		if (label_length > label_width) {
			os << string_view(name.data(), label_width) << "\u2026";
		} else {
			uint32_t const padding = label_width - label_length;
			os << string(padding, '\x20') << name << '\x20';
		}
		os << STRING_VLINE_WITH_COLOR;

		fill(value_content.begin(), value_content.end(), '\x20');  // clear
		build_value_label(value_content, label_format(base_scale(data.median)), median_pos);
		build_value_label(value_content, label_format(base_scale(data.ufence)), ufence_pos);
		build_value_label(value_content, label_format(base_scale(data.max)), max_pos);
		build_value_label(value_content, label_format(base_scale(data.lfence)), lfence_pos);
		build_value_label(value_content, label_format(base_scale(data.min)), min_pos);
		os << string_view(value_content) << STRING_VLINE_WITH_COLOR << endl;

		// (b) box row
		os << space_header << STRING_VLINE_WITH_COLOR;
		for (auto const& cell : buffer) {
			switch (cell.type) {
			case cell_type::empty:
				os << '\x20';
				break;
			case cell_type::block:
				os << blocks[cell.offset];
				break;
			case cell_type::whisker:
				os << STRING_HLINE;
				break;
			case cell_type::marker:
				os << vlines[cell.offset];
				break;
			case cell_type::median:
				os << COLOR_INVERT << vlines[cell.offset] << COLOR_RESET;
				break;
			case cell_type::worst:
				os << COLOR_ERROR << STRING_X << COLOR_RESET;
				break;
			case cell_type::best:
				os << COLOR_SUCCESS << STRING_X << COLOR_RESET;
				break;
			}
		}
		os << STRING_VLINE_WITH_COLOR << std::endl;
	}

	// 3. render axis on the bottom
	os
		<< label_padding
		<< COLOR_DARKGRAY
		<< STRING_BOTTOM_LEFT
		<< hline
		<< STRING_BOTTOM_RIGHT
		<< COLOR_RESET
		<< std::endl;
}
