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

#include "macro.h"  // INKNIT_EXPORT, INKNIT_NONNULL, INKNIT_RESTRICT

#ifdef __cplusplus
extern "C" {
#endif

#if _DEBUG
typedef void (*inknit_assert_t)(
	const char *INKNIT_RESTRICT __message,
	const char *INKNIT_RESTRICT __condition,
	const char *INKNIT_RESTRICT __file,
	const char *INKNIT_RESTRICT __func,
	int                         __line,
	void *INKNIT_RESTRICT       data
);

struct inknit_assert_info {
	inknit_assert_t       handler;
	void *INKNIT_RESTRICT data;
};

struct inknit_assert_info INKNIT_EXPORT
inknit_hook_assert(inknit_assert_t handler, void *INKNIT_RESTRICT data) INKNIT_NONNULL(1, 2);
#endif


#ifdef __cplusplus
} /* extern "C" { */
#endif
