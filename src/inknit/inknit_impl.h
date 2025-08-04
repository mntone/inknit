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

#include <stddef.h>  // NULL, size_t

#include "inc/inknit/macro.h"  // load base macro
#include "inc/inknit/proto.h"  // load base proto
#include "inc/inknit/types.h"  // load base types

#include "macro_impl.h"  // load internal macro

typedef uint32_t inknit_unit_t;

#if !NDEBUG
void _inknit_assert(
	const char *INKNIT_RESTRICT __message,
	const char *INKNIT_RESTRICT __condition,
	const char *INKNIT_RESTRICT __file,
	const char *INKNIT_RESTRICT __func,
	int                         __line
) INKNIT_NONNULL(1, 2, 3, 4);
#endif
