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

#include "quantiler.hpp"

#include <algorithm>  // sort
#include <cmath>      // signbit

using namespace inkbm::monitors;

INKBM_ALWAYS_INLINE
double p2_estimator::parabolic_interpolation(std::uint32_t i, double sign_d) noexcept {
	double const term1 = sign_d / static_cast<double>(n_[i + 1] - n_[i - 1]);
	double const term2 = (static_cast<double>(n_[i] - n_[i - 1]) + sign_d)
					   * (q_[i + 1] - q_[i])
					   / static_cast<double>(n_[i + 1] - n_[i]);
	double const term3 = (static_cast<double>(n_[i + 1] - n_[i]) - sign_d)
					   * (q_[i] - q_[i - 1])
					   / static_cast<double>(n_[i] - n_[i - 1]);
	return q_[i] + term1 * (term2 + term3);
}

INKBM_ALWAYS_INLINE
double p2_estimator::linear_interpolation(std::uint32_t i, bool sign_d) noexcept {
	if (!sign_d) {
		return q_[i] + (q_[i + 1] - q_[i]) / (n_[i + 1] - n_[i]);
	} else {
		return q_[i] + (q_[i - 1] - q_[i]) / (n_[i - 1] - n_[i]);
	}
}

void p2_estimator::add(double val) noexcept {
	// 1. initialization phase
	if (count_ < 5) {
		q_[count_++] = val;

		if (count_ == 5) {
			std::sort(q_.begin(), q_.end());
		}
		return;
	}

	// 2. observation phase

	// Find cell k where the new value lies and update extreme markers if necessary
	std::uint32_t k = 0;
	if (val < q_[0]) {
		k     = 0;
		q_[0] = val;
	} else if (val < q_[1]) {
		k = 0;
	} else if (val < q_[2]) {
		k = 1;
	} else if (val < q_[3]) {
		k = 2;
	} else if (val < q_[4]) {
		k = 3;
	} else {
		k     = 3;
		q_[4] = val;
	}

	// Increment positions of markers k+1 through 4
	for (std::uint32_t i = k + 1; i < 5; ++i) {
		++n_[i];
	}

	// Update desired positions for all markers
	for (std::uint32_t i = 1; i < 5; ++i) {
		ns_[i] += dns_[i];
	}

	// 3. marker adjustment phase
	for (std::uint32_t i = 1; i < 4; ++i) {
		double const d = ns_[i] - n_[i];

		if (1.0 <= d && 1 < (n_[i + 1] - n_[i]) || d <= -1.0 && (n_[i - 1] - n_[i]) < -1) {
			bool const sign_d = std::signbit(d);

			// Try parabolic interpolation first
			double new_q = parabolic_interpolation(i, sign_d ? -1.0 : 1.0);

			// If parabolic is not applicable or produces an out-of-order result, use linear
			if (new_q <= q_[i - 1] || q_[i + 1] <= new_q) {
				new_q = linear_interpolation(i, sign_d);
			}

			q_[i] = new_q;
			if (sign_d) {
				--n_[i];
			} else {
				++n_[i];
			}
		}
	}
}

double p2_estimator::get() const noexcept {
	if (count_ < 5) {
		// To provide a result even with few samples, we can sort and pick
		if (count_ == 0) {
			return std::numeric_limits<double>::quiet_NaN();
		}

		std::array<double, 5> temp_q(q_);
		std::sort(temp_q.begin(), temp_q.begin() + count_);

		// Find the element closest to the desired quantile
		std::uint32_t const index
			= static_cast<std::uint32_t>(std::round(probability_ * (count_ - 1)));
		return temp_q[index];
	}

	// The quantile estimate is the height of the central marker
	return q_[2];
}

#if _DEBUG
void p2_estimator::dump(std::string const& name) const noexcept {
	printf("--- State for %s ---\n", name.c_str());
	printf("p = %f, count = %d\n", probability_, count_);
	// clang-format off
	printf("q  (heights) = {%6.1f, %6.1f, %6.1f, %6.1f, %6.1f}\n", q_[0],  q_[1],  q_[2],  q_[3],  q_[4]);
	printf("n  (actual)  = {%4u  , %4u  , %4u  , %4u  , %4u  }\n",           n_[0],  n_[1],  n_[2],  n_[3],  n_[4]);
	printf("ns (desired) = {%6.1f, %6.1f, %6.1f, %6.1f, %6.1f}\n", ns_[0], ns_[1], ns_[2], ns_[3], ns_[4]);
	// clang-format on
	puts("----------------------------------");
}
#endif


using namespace inkbm::monitors::details;

// -- MARK: estimated_quantiler implementation

void estimated_quantiler::add(stat_value val) noexcept {
	double const dval = static_cast<double>(val);
	quant1_.add(dval);
	median_.add(dval);
	quant3_.add(dval);
	pct95_.add(dval);
	pct99_.add(dval);
}

void estimated_quantiler::clear() noexcept {
	quant1_.clear();
	median_.clear();
	quant3_.clear();
	pct95_.clear();
	pct99_.clear();
}

// -- MARK: exactness_quantiler implementation

double exactness_quantiler::percentile(double percentile) const noexcept {
	std::size_t const n = data_.size();
	if (n <= 1) {
		return static_cast<double>(data_[0]);
	}

	double const      index       = percentile * static_cast<double>(n - 1);
	std::size_t const lower_index = static_cast<std::size_t>(index);
	double const      fraction    = index - static_cast<double>(lower_index);
	if (lower_index + 1 >= n) {
		return static_cast<double>(data_[n - 1]);
	}

	stat_value const lower_val = data_[lower_index];
	stat_value const upper_val = data_[lower_index + 1];
	double const     pct_n_val
		= static_cast<double>(lower_val) + fraction * static_cast<double>(upper_val - lower_val);
	return pct_n_val;
}
