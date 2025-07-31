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

#include "platform.h"

#include "base.hpp"

#include <errno.h>         // errno
#include <sys/resource.h>  // getpriority

#define INKBM_NICE_MAX     19
#define INKBM_NICE_MAX_STR "19"

static int __inkbm_linux_nice = 0;

void inkbm_platform_init() noexcept {}

void inkbm_platform_uninit() noexcept {}

void inkbm_platform_sleep(int milliseconds) noexcept {
#if _POSIX_C_SOURCE >= 199309L
	struct timespec ts;
	ts.tv_sec  = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1'000'000;
	nanosleep(&ts, nullptr);
#else
	if (milliseconds >= 1000) {
		sleep(milliseconds / 1000);
	}
	usleep((milliseconds % 1000) * 1000);
#endif
}

void inkbm_platform_benchmark_start() noexcept {
	id_t const thread_id = static_cast<id_t>(gettid());

	errno = 0;  // require to reset 0

	int old_nice = getpriority(PRIO_PROCESS, thread_id);
	if (errno != 0) {
		INKBM_FATAL("Failed to get process priority: errno=%d.", errno);
	}

	int ret = setpriority(PRIO_PROCESS, thread_id, INKBM_NICE_MAX);
	if (ret != 0) {
		INKBM_FATAL("Failed to set process priority to INKBM_NICE_MAX (" INKBM_NICE_MAX_STR ").");
	}

	__inkbm_linux_nice = old_nice;
}

void inkbm_platform_benchmark_stop() noexcept {
	id_t const thread_id = static_cast<id_t>(gettid());

	int ret = setpriority(PRIO_PROCESS, thread_id, __inkbm_linux_nice);
	if (ret != 0) {
		INKBM_WARN("Failed to restore process priority.");
	}

	__inkbm_linux_nice = 0;
}
