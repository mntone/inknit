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

#include "base.hpp"

INKBM_DECLS_BEGIN

void inkbm_platform_init() INKBM_NOEXCEPT;
void inkbm_platform_uninit() INKBM_NOEXCEPT;

void inkbm_platform_sleep(int milliseconds) INKBM_NOEXCEPT;

void inkbm_platform_benchmark_start() INKBM_NOEXCEPT;
void inkbm_platform_benchmark_stop() INKBM_NOEXCEPT;

INKBM_DECLS_END
