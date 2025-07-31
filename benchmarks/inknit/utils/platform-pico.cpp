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

#include "pico/stdio.h"  // stdio_init_all, stdio_deinit_all
#include "pico/time.h"   // sleep_ms

void inkbm_platform_init() noexcept {
	stdio_init_all();
}

void inkbm_platform_uninit() noexcept {
	stdio_deinit_all();
}

void inkbm_platform_sleep(int milliseconds) noexcept {
	sleep_ms(milliseconds);
}

void inkbm_platform_benchmark_start() noexcept {}

void inkbm_platform_benchmark_stop() noexcept {}
