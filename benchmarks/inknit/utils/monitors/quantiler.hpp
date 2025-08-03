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

#include <algorithm>  // sort
#include <array>      // array
#include <variant>    // variant
#include <vector>     // vector

#if _DEBUG
#include <cstdio>  // printf, puts
#endif

namespace inkbm::monitors {

struct p2_estimator final {
	inline constexpr explicit p2_estimator(double probability) noexcept
		: probability_(probability)
		, ns_ {0.0, 2.0 * probability, 4.0 * probability, 2.0 + 2.0 * probability, 4.0}
		, dns_ {0.0, probability / 2.0, probability, (1.0 + probability) / 2.0, 1.0} {}

	void add(double val) noexcept;

	inline constexpr void clear() noexcept {
		count_ = 0;
		q_     = {0.0, 0.0, 0.0, 0.0, 0.0};
		ns_    = {0.0, 2.0 * probability_, 4.0 * probability_, 2.0 + 2.0 * probability_, 4.0};
		dns_   = {0.0, probability_ / 2.0, probability_, (1.0 + probability_) / 2.0, 1.0};
		n_     = {0, 1, 2, 3, 4};
	}

	INKBM_NODISCARD
	double get() const noexcept;

#if _DEBUG
	void dump(std::string const& name) const noexcept;
#endif

private:
	double parabolic_interpolation(std::uint32_t i, double sign_d) noexcept;
	double linear_interpolation(std::uint32_t i, bool sign_d) noexcept;

private:
	double                       probability_;
	std::uint32_t                count_ {0};
	std::array<double, 5>        q_ {0.0, 0.0, 0.0, 0.0, 0.0};
	std::array<double, 5>        ns_;
	std::array<double, 5>        dns_;
	std::array<std::uint32_t, 5> n_ {0, 1, 2, 3, 4};
};

struct quantile final {
	double quant1;
	double median;
	double quant3;
	double pct95;
	double pct99;
};

namespace details {

	struct estimated_quantiler final {
		inline constexpr explicit estimated_quantiler()
			: quant1_(0.25)
			, median_(0.5)
			, quant3_(0.75)
			, pct95_(0.95)
			, pct99_(0.99) {}

		void add(stat_value val) noexcept;
		void clear() noexcept;

		INKBM_NODISCARD
		inline quantile get() const noexcept {
			quantile quantile {
				quant1_.get(),
				median_.get(),
				quant3_.get(),
				pct95_.get(),
				pct99_.get(),
			};
#if _DEBUG
			if (quantile.quant1 > quantile.median) {
				puts("!!! ANOMALY DETECTED: quant1 > median !!!");
				printf("quant1 = %6.1f, median = %6.1f\n", quantile.quant1, quantile.median);

				quant1_.dump("quant1_estimator");
				median_.dump("median_estimator");
			}
#endif
			return std::move(quantile);
		}

	private:
		p2_estimator quant1_, median_, quant3_, pct95_, pct99_;
	};

	struct exactness_quantiler final {
		inline constexpr explicit exactness_quantiler() {
			data_.reserve(256);
		}

		inline constexpr void add(stat_value val) noexcept {
			data_.emplace_back(val);
		}

		inline constexpr void clear() noexcept {
			data_.clear();
		}

		INKBM_NODISCARD
		inline quantile get() const noexcept {
			std::sort(data_.begin(), data_.end());

			quantile quantile {
				percentile(0.25),
				percentile(0.5),
				percentile(0.75),
				percentile(0.95),
				percentile(0.99),
			};
			return std::move(quantile);
		}

	private:
		double percentile(double percentile) const noexcept;

	private:
		mutable std::vector<stat_value> data_;
	};

}  // namespace details

class quantiler final {
#if INKBM_USE_ESTIMATED_QUANTILER
	using quantiler_impl = details::estimated_quantiler;
#else
	using quantiler_impl = details::exactness_quantiler;
#endif

public:
	constexpr explicit quantiler() noexcept
		: impl_(quantiler_impl {}) {}

	inline void add(stat_value val) noexcept {
		std::visit(
			[val](auto& impl) noexcept -> void {
				using T = std::decay_t<decltype(impl)>;

				if constexpr (std::is_same_v<T, details::estimated_quantiler>) {
					impl.add(val);
				} else if constexpr (std::is_same_v<T, details::exactness_quantiler>) {
					impl.add(val);
				}
			},
			impl_
		);
	}

	inline void clear() noexcept {
		std::visit(
			[](auto& impl) noexcept -> void {
				using T = std::decay_t<decltype(impl)>;

				if constexpr (std::is_same_v<T, details::estimated_quantiler>) {
					impl.clear();
				} else if constexpr (std::is_same_v<T, details::exactness_quantiler>) {
					impl.clear();
				}
			},
			impl_
		);
	}

	INKBM_NODISCARD
	inline quantile get() const noexcept {
		return std::visit(
			[](auto& impl) noexcept -> quantile {
				using T = std::decay_t<decltype(impl)>;

				if constexpr (std::is_same_v<T, details::estimated_quantiler>) {
					return std::move(impl.get());
				} else if constexpr (std::is_same_v<T, details::exactness_quantiler>) {
					return std::move(impl.get());
				}
			},
			impl_
		);
	}

private:
	std::variant<details::estimated_quantiler, details::exactness_quantiler> impl_;
};

}  // namespace inkbm::monitors
