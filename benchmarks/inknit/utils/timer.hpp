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

#include <chrono>       // chrono
#include <type_traits>  // static_assert

namespace inkbm {

/**
 * @brief A timer class for measuring elapsed time.
 */
class timer final {
public:
	using clock_type      = std::chrono::high_resolution_clock;
	using time_point_type = std::chrono::time_point<clock_type>;
	using duration_type   = std::chrono::nanoseconds;

	timer() noexcept = default;

	/**
	 * @brief Resets the timer to its initial state.
	 */
	constexpr void reset() noexcept {
		elapsed_ = duration_type {};
	}

	/**
	 * @brief Starts the timer. Can be called multiple times.
	 */
	void start() noexcept {
		start_ = clock_type::now();
	}

	/**
	 * @brief Stops the timer, calculates the duration, and resets the start time.
	 */
	void stop() noexcept {
		// Only calculate duration if the timer was actually started.
		if (start_ != time_point_type {}) {
			time_point_type const end_time = clock_type::now();
			elapsed_ += std::chrono::duration_cast<duration_type>(end_time - start_);

			// Reset start_time_ to its default (zero) state to indicate it's stopped.
			start_ = time_point_type {};
		}
	}

	/**
	 * @brief Gets the duration of the last completed start-stop interval.
	 * @return The elapsed time as a std::chrono::nanoseconds type.
	 */
	constexpr duration_type duration() const noexcept {
		return elapsed_;
	}

private:
	time_point_type start_ {};
	duration_type   elapsed_ {};
};

// Helper using C++17's std::void_t to check if a specific expression is valid.
template<typename T, typename = void>
struct has_start_method: std::false_type {};
template<typename T>
struct has_start_method<T, std::void_t<decltype(std::declval<T&>().start())>>: std::true_type {};

template<typename T, typename = void>
struct has_stop_method: std::false_type {};
template<typename T>
struct has_stop_method<T, std::void_t<decltype(std::declval<T&>().stop())>>: std::true_type {};


/**
 * @brief A guard class that automatically measures the execution time within a scope, based on the RAII pattern.
 * @tparam T A timer class that has start() and stop() methods.
 */
template<typename T>
class timer_guard {
public:
	static_assert(
		has_start_method<T>::value, "timer_guard requires the type T to have a 'start()' method."
	);
	static_assert(
		has_stop_method<T>::value, "timer_guard requires the type T to have a 'stop()' method."
	);

	/**
	 * @brief Constructor. Takes a reference to a timer and starts the measurement.
	 * @param t The timer object to be used for measurement.
	 */
	explicit timer_guard(T& t) noexcept
		: timer_ref_(t) {
		timer_ref_.start();
	}

	/**
	 * @brief Destructor. Stops the measurement when the object is destroyed.
	 */
	~timer_guard() noexcept {
		timer_ref_.stop();
	}

	// Disable copy and move to prevent unintended behavior.
	timer_guard(timer_guard const&)            = delete;
	timer_guard& operator=(timer_guard const&) = delete;
	timer_guard(timer_guard&&)                 = delete;
	timer_guard& operator=(timer_guard&&)      = delete;

private:
	T& timer_ref_;
};


}  // namespace inkbm
