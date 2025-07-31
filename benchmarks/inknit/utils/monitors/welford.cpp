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

#include "welford.hpp"

#include "../base.hpp"
#include "quantiler.hpp"

#include <cmath>  // sqrt

constexpr double ZERO_TOLERANCE = 1e-9;

using namespace inkbm;
using namespace inkbm::monitors;

welford_monitor::welford_monitor(
	std::uint32_t min_iterations, std::uint32_t check_interval, double convergence_threshold
) noexcept
	: min_iterations_(min_iterations)
	, check_interval_(check_interval)
	, convergence_threshold_(convergence_threshold)
	, stable_(false)
	, count_(0)
	, min_(std::numeric_limits<std::int64_t>::max())
	, max_(std::numeric_limits<std::int64_t>::min())
	, mean_(0.0)
	, sumsq_(0.0)
	, last_checked_mean_(0.0) {
	if (check_interval == 0) {
		INKBM_FATAL(
			"Failed to initialize monitor: " INKBM_STRINGIFY(check_interval) " cannot be zero."
		);
	}
}

bool welford_monitor::add(std::int64_t interval) noexcept {
	if (stable_) {
		return true;
	}
	if (interval < 0) {
		INKBM_WARN("Failed to add value: interval cannot be negative.");
		return stable_;
	}

	count_++;

	if (min_ > interval) {
		min_ = interval;
	}
	if (max_ < interval) {
		max_ = interval;
	}

	double const intvl = static_cast<double>(interval);
	double const delta = intvl - mean_;
	mean_ += delta / count_;
	sumsq_ += (intvl - mean_) * delta;

	if (count_ >= min_iterations_ && count_ % check_interval_ == 0) {
		check_convergence();
	}

	quantiler_.add(intvl);
	return stable_;
}

void welford_monitor::check_convergence() noexcept {
	// Check for convergence using relative difference, but only if the mean is
	// large enough to avoid division by zero.
	if (last_checked_mean_ > ZERO_TOLERANCE) {
		double const relative_diff = std::abs(mean_ - last_checked_mean_) / last_checked_mean_;
		if (relative_diff < convergence_threshold_) {
			stable_ = true;
		}
	}

	// If the mean is close to zero, consider it converged if the absolute value
	// is smaller than the convergence threshold.
	else if (std::abs(mean_) < convergence_threshold_) {
		stable_ = true;
	}

	// Save the current mean for the next check.
	last_checked_mean_ = mean_;
}

statistics welford_monitor::get() const noexcept {
	double const min    = static_cast<double>(min_);
	double const max    = static_cast<double>(max_);
	double const stddev = count_ > 1 ? std::sqrt(sumsq_ / static_cast<double>(count_ - 1)) : 0.0;

	quantile     quantile {quantiler_.get()};
	double const iqr    = quantile.quant3 - quantile.quant1;
	double const lfence = quantile.quant1 - 1.5 * iqr;
	double const ufence = quantile.quant3 + 1.5 * iqr;

	statistics stats {
		count_,

		min,
		max,
		mean_,
		stddev,

		std::max(min, lfence),
		quantile.quant1,
		quantile.median,
		quantile.quant3,
		std::min(max, ufence),
		quantile.pct95,
		quantile.pct99
	};
	return std::move(stats);
}

void welford_monitor::reset() noexcept {
	count_             = 0;
	min_               = std::numeric_limits<std::int64_t>::max();
	max_               = std::numeric_limits<std::int64_t>::min();
	mean_              = 0.0;
	sumsq_             = 0.0;
	last_checked_mean_ = 0.0;

	quantiler_.clear();

	stable_ = false;
}
