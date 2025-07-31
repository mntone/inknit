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

#include "../inknit_impl.h"

#include "config.h"            // load config
#include "../inc/inknit/x2.h"  // load prototype

static INKNIT_ALWAYS_INLINE uint32_t INTERNAL_FUNCNAME(fillByteWithColor)(inknit_color_t color) {
	uint32_t pattern = color;
#ifdef INKNIT_SLOW_MULTIPLY
	// Cortex-M0+: construct pattern via bitwise operations
	pattern |= pattern << 2;
	pattern |= pattern << 4;
#else
	// Faster pattern construction via multiplication (Cortex-M3/M4/M33+)
	pattern *= 0x55u;
#endif
	return pattern;
}

static INKNIT_ALWAYS_INLINE uint32_t INTERNAL_FUNCNAME(fillWordWithColor)(inknit_color_t color) {
	uint32_t pattern = color;
#ifdef INKNIT_SLOW_MULTIPLY
	// Cortex-M0+: construct pattern via bitwise operations
	pattern |= pattern << 2;
	pattern |= pattern << 4;
	pattern |= pattern << 8;
	pattern |= pattern << 16;
#else
	// Faster pattern construction via multiplication (Cortex-M3/M4/M33+)
	pattern *= 0x55555555u;
#endif
	return pattern;
}
