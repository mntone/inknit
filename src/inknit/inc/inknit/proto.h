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

#include "macro.h"   // INKNIT_EXPORT, INKNIT_NONNULL, INKNIT_RESTRICT
#include "prefix.h"  // __INKNIT_{access}_{type}NAME_BASE

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

#define INKNIT_PROTO(prototype) INKNIT_EXPORT prototype

#ifdef INKNIT_ENABLE_BENCHMARKS
#define INKNIT_BMPROTO(prototype) INKNIT_EXPORT prototype
#else
#define INKNIT_BMPROTO(prototype)
#endif


// -- MARK: blit
#define INKNIT_DEFPROTO_BLIT(name)                        \
	void name(                                            \
		struct inknit_image *INKNIT_RESTRICT       dst,   \
		inknit_uint_t                              dx,    \
		inknit_uint_t                              dy,    \
		const struct inknit_image *INKNIT_RESTRICT src,   \
		inknit_uint_t                              sx,    \
		inknit_uint_t                              sy,    \
		inknit_uint_t                              width, \
		inknit_uint_t                              height \
	)
#define INKNIT_PROTO_BLIT(name) INKNIT_DEFPROTO_BLIT(name) INKNIT_NONNULL(1, 4)

#ifdef __cplusplus
} /* extern "C" { */
#endif
