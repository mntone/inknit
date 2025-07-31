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

#include "../types.hpp"
#include "quantiler.hpp"

namespace inkbm::monitors {

class welford_monitor final: public monitor {
public:
	explicit welford_monitor(
		std::uint32_t min_iterations        = 128,
		std::uint32_t check_interval        = 32,
		double        convergence_threshold = 0.00025
	) noexcept;

	bool       add(std::int64_t interval) noexcept override;
	statistics get() const noexcept override;
	void       reset() noexcept override;

	inline constexpr bool is_stable() const noexcept override {
		return stable_;
	}

private:
	void check_convergence() noexcept;

private:
	std::uint32_t const min_iterations_;
	std::uint32_t const check_interval_;
	double const        convergence_threshold_;

	bool          stable_;
	std::uint32_t count_;
	std::int64_t  min_, max_;
	double        mean_, sumsq_;
	double        last_checked_mean_;

	quantiler quantiler_;
};

}  // namespace inkbm::monitors
