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

#include "base.hpp"  // INKBM_NODISCARD

#include <any>          // any
#include <cstdint>      // uint32_t, int64_t
#include <string_view>  // string_view
#include <type_traits>  // is_arithmetic_v

namespace inkbm {

template<typename T>
concept number = std::is_arithmetic_v<T>;

template<number N>
struct minmax final {
	N min;
	N max;
};

using stat_value = double;

struct statistics final {
	std::uint32_t count;

	stat_value min;
	stat_value max;
	stat_value mean;
	stat_value stddev;

	stat_value lfence;  // lower fence
	stat_value quant1;  // first quartile
	stat_value median;  // median
	stat_value quant3;  // third quartile
	stat_value ufence;  // upper fence
	stat_value pct95;   // 95th percentile
	stat_value pct99;   // 99th percentile
};

struct fixture {
	virtual ~fixture() noexcept = default;
	virtual void setup() noexcept {}
	virtual void teardown() noexcept {}

	virtual void load(INKBM_MAYBE_UNUSED std::any const *data) noexcept {}
	virtual void execute() noexcept = 0;
};

class monitor {
public:
	virtual ~monitor() noexcept = default;

	virtual bool add(std::int64_t interval) noexcept = 0;

	INKBM_NODISCARD
	virtual statistics get() const noexcept = 0;

	virtual void reset() noexcept = 0;

	virtual bool is_stable() const noexcept = 0;
};

class executor {
public:
	virtual ~executor() noexcept = default;

	INKBM_NODISCARD
	virtual statistics execute(monitor *monitor, std::any const *data) const noexcept = 0;

	INKBM_NODISCARD
	virtual std::string_view name() const noexcept = 0;
};

}  // namespace inkbm
