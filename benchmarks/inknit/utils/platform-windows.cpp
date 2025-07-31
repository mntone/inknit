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

#include <synchapi.h>  // Sleep
#include <windows.h>   // GetConsoleOutputCP, SetConsoleOutputCP

static UINT __inkbm_windows_codepage = 0;

static INKBM_THREAD_LOCAL int __inkbm_windows_prio = 0;

void inkbm_platform_init() noexcept {
	UINT oldcp = GetConsoleOutputCP();
	if (oldcp == 0) {
		INKBM_FATAL(
			"Failed to get the current console output codepage: last_error=%d.", GetLastError()
		);
	}
	if (!SetConsoleOutputCP(CP_UTF8)) {
		INKBM_FATAL(
			"Failed to set the console output codepage to UTF-8: last_error=%d.", GetLastError()
		);
	}
	__inkbm_windows_codepage = oldcp;
}

void inkbm_platform_uninit() noexcept {
	if (!SetConsoleOutputCP(__inkbm_windows_codepage)) {
		INKBM_WARN(
			"Failed to restore the previous console codepage: last_error=%d.", GetLastError()
		);
		SetLastError(NO_ERROR);
	}
}

void inkbm_platform_sleep(int milliseconds) noexcept {
	Sleep(milliseconds);
}

void inkbm_platform_benchmark_start() noexcept {
	HANDLE const thread = GetCurrentThread();
	int const    prio   = GetThreadPriority(thread);
	if (prio == THREAD_PRIORITY_ERROR_RETURN) {
		INKBM_FATAL("Failed to get process priority: last_error=%d.", GetLastError());
	}

	BOOL const ret = SetThreadPriority(thread, THREAD_PRIORITY_TIME_CRITICAL);
	if (!ret) {
		INKBM_FATAL(
			"Failed to set process priority to THREAD_PRIORITY_TIME_CRITICAL: last_error=%d.",
			GetLastError()
		);
	}

	__inkbm_windows_prio = prio;
}

void inkbm_platform_benchmark_stop() noexcept {
	HANDLE const thread = GetCurrentThread();

	BOOL const ret = SetThreadPriority(thread, __inkbm_windows_prio);
	if (!ret) {
		INKBM_WARN(
			"Failed to set process priority to THREAD_PRIORITY_TIME_CRITICAL: last_error=%d.",
			GetLastError()
		);
		SetLastError(NO_ERROR);
	}

	__inkbm_windows_prio = 0;
}
