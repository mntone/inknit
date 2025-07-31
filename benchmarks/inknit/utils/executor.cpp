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

#include "platform.h"
#include "timer.hpp"

#include "monitors/welford.hpp"

#include <chrono>  // chrono

using namespace inkbm;

static inline constexpr std::int64_t
convert_duration(std::chrono::high_resolution_clock::duration duration) noexcept {
	return duration_cast<std::chrono::nanoseconds>(duration).count();
}

template<std::uint32_t N>
static inline constexpr statistics
calc_stats(std::string name, std::chrono::high_resolution_clock::duration (&data)[N]) noexcept {
	return calc_stats(std::move(name), std::move(data), N);
}

statistics
unit_executor::execute(monitor *monitor, INKBM_MAYBE_UNUSED std::any const *data) const noexcept {
	using namespace std::chrono;

	monitor->reset();

	execute_();  // dispose

	timer executor_timer;
	inkbm_platform_benchmark_start();
	do {
		executor_timer.reset();
		{
			timer_guard<timer> guard(executor_timer);
			execute_();
		}

		monitor->add(executor_timer.duration().count());
	} while (!monitor->is_stable());
	inkbm_platform_benchmark_stop();

	statistics stats {monitor->get()};
	return stats;
}

statistics fixture_executor::execute(monitor *monitor, std::any const *data) const noexcept {
	using namespace std::chrono;

	monitor->reset();

	std::unique_ptr<fixture> fixture {get_()};
	fixture->setup();
	fixture->load(data);
	fixture->execute();  // dispose

	timer executor_timer;
	inkbm_platform_benchmark_start();
	do {
		executor_timer.reset();
		{
			timer_guard<timer> guard(executor_timer);
			fixture->execute();
		}

		monitor->add(executor_timer.duration().count());
	} while (!monitor->is_stable());
	inkbm_platform_benchmark_stop();

	fixture->teardown();

	statistics stats {monitor->get()};
	return stats;
}
