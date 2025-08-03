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

#include "inkbm.hpp"

#include "monitors/welford.hpp"
#include "scale_impl.hpp"
#include "boxplot.hpp"

#include <cmath>      // floor, log10
#include <cinttypes>  // PRIu32
#include <cstdio>     // printf, puts
#include <iostream>   // cout
#include <numeric>    // accumulate

using namespace inkbm;

template<std::integral I>
static inline constexpr I count_digits(I val) noexcept {
	if (val == 0) {
		return 1;
	}
	return static_cast<I>(std::floor(std::log10(val))) + 1;
}

static inline std::string with_commas(int64_t value) {
	std::string str {std::to_string(value)};

	int insertPosition = static_cast<int>(str.length()) - 3;
	while (insertPosition > 0) {
		str.insert(static_cast<std::size_t>(insertPosition), ",");
		insertPosition -= 3;
	}
	return str;
}


void registry::run_all(bool compatible_mode) noexcept {
	std::uint32_t const unit_count  = static_cast<std::uint32_t>(cases_.size());
	std::uint32_t const group_count = static_cast<std::uint32_t>(groups_.size());
	std::uint32_t const total       = unit_count + group_count;
	std::uint32_t const base_offset = 16 + count_digits(total);

	monitors::welford_monitor monitor;

	printf(INKBM_HEADER "Running %" PRIu32 " benchmarks...\n", total);
	if (unit_count != 0) {
		for (std::uint32_t i = 0; i < unit_count; ++i) {
			std::uint32_t const offset      = base_offset + count_digits(i + 1);
			std::uint32_t const label_width = offset - 1;

			std::unique_ptr<executor> const& executor {cases_[i]};

			std::string name {executor->name()};
			printf(
				INKBM_HEADER "[%" PRIu32 "/%" PRIu32 "] Running benchmark %s\n",
				i + 1,
				total,
				name.c_str()
			);

			statistics stats {executor->execute(&monitor, nullptr)};
			print_stats(stats, offset);

			charts::boxplot_chart bp;
			bp.add_dataset(name, stats)
				.high_precision(!compatible_mode)
				.label_width(label_width)
				.render(std::cout);
		}
	}

	if (group_count != 0) {
		std::uint32_t i = unit_count;

		std::vector<std::pair<std::string, statistics>> stats_set;
		for (auto const& [name, group] : groups_) {
			std::uint32_t const offset = base_offset + count_digits(i + 1);

			std::string spacing(offset, '\x20');
			printf(
				INKBM_HEADER "[%" PRIu32 "/%" PRIu32 "] Running benchmark %s\n", i + 1, total, name
			);

			std::size_t const case_count = group.cases().size();
			stats_set.resize(case_count * group.arguments().items.size());

			std::uint32_t j = 0;
			for (auto const& item : group.arguments().items) {
				std::transform(
					group.cases().cbegin(),
					group.cases().cend(),
					std::next(stats_set.begin(), static_cast<std::ptrdiff_t>(j++ * case_count)),
					[this, offset, &spacing, &monitor, &item](
						std::unique_ptr<executor> const& executor
					) {
						std::string name {executor->name()};
						name += '/';
						name += item.short_name;
						printf("%s[%s]\n", spacing.c_str(), name.c_str());

						statistics stats {executor->execute(&monitor, &item.value)};
						print_stats(stats, offset + 2);
						return std::make_pair(std::move(name), std::move(stats));
					}
				);
			}

			charts::boxplot_chart bp;
			for (auto const& [name, stats] : stats_set) {
				bp.add_dataset(name, stats);
			}
			bp.high_precision(!compatible_mode).label_width(24).render(std::cout);

			++i;
		}
	}

	puts(INKBM_HEADER "All benchmarks finished.");
}


void registry::print_stats(statistics const& stats, std::uint32_t offset) noexcept {
	std::string spacing(offset, '\x20');

	printf(
		"%smean: %s (±%s), n: %s\n",
		spacing.c_str(),
		scale::format_string(stats.mean).c_str(),
		scale::format_string(stats.stddev).c_str(),
		with_commas(stats.count).c_str()
	);
	printf(
		"%smin: %s, median: %s, max: %s, p95: %s, p99: %s\n",
		spacing.c_str(),
		scale::format_string(stats.min).c_str(),
		scale::format_string(stats.median).c_str(),
		scale::format_string(stats.max).c_str(),
		scale::format_string(stats.pct95).c_str(),
		scale::format_string(stats.pct99).c_str()
	);

#if _DEBUG
	printf(
		"%slower: %s, upper: %s, quant1: %s, quant3: %s\n",
		spacing.c_str(),
		scale::format_string(stats.lfence).c_str(),
		scale::format_string(stats.ufence).c_str(),
		scale::format_string(stats.quant1).c_str(),
		scale::format_string(stats.quant3).c_str()
	);
#endif
}
