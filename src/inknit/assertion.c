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

#include "inc/inknit/assertion.h"

#include <stdio.h>
#include <stdlib.h>

#define ASSERT_FORMAT        \
	"Assertion failed: %s\n" \
	"- cond: %s\n"           \
	"- file: %s, line: %d\n" \
	"- func: %s\n"

void _inknit_default_assert_handler(
	const char *INKNIT_RESTRICT __message,
	const char *INKNIT_RESTRICT __condition,
	const char *INKNIT_RESTRICT __file,
	const char *INKNIT_RESTRICT __func,
	int                         __line,
	void                       *data
) {
	(void)data;

	fprintf(stderr, ASSERT_FORMAT, __message, __condition, __file, __line, __func);
	abort();
}

static struct inknit_assert_info __inknit_assert_info = {_inknit_default_assert_handler, NULL};

struct inknit_assert_info inknit_hook_assert(inknit_assert_t handler, void *INKNIT_RESTRICT data) {
	struct inknit_assert_info prev_info = __inknit_assert_info;

	__inknit_assert_info.handler = handler;
	__inknit_assert_info.data    = data;
	return prev_info;
}

void _inknit_assert(
	const char *INKNIT_RESTRICT __message,
	const char *INKNIT_RESTRICT __condition,
	const char *INKNIT_RESTRICT __file,
	const char *INKNIT_RESTRICT __func,
	int                         __line
) {
	struct inknit_assert_info info = __inknit_assert_info;

	if (info.handler != NULL) {
		info.handler(__message, __condition, __file, __func, __line, info.data);
	}
}
