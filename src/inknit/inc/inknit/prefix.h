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

#define __INKNIT_PUBLIC_FUNCBASE   inknit_
#define __INKNIT_INTERNAL_FUNCBASE _inknit_
#define __INKNIT_INTERNAL_VARBASE  __inknit_

#define _INKNIT_CONCAT_IMPL(x, y)   x##y
#define _INKNIT_CONCAT2(x, y)       _INKNIT_CONCAT_IMPL(x, y)
#define _INKNIT_CONCAT3(x, y, z)    _INKNIT_CONCAT2(x, _INKNIT_CONCAT2(y, z))
#define _INKNIT_CONCAT4(x, y, z, w) _INKNIT_CONCAT2(x, _INKNIT_CONCAT3(y, z, w))

#define INKNIT_INTERNAL_GROUP(width_and_align, _bpp, endian)                   \
	_INKNIT_CONCAT4(width_and_align, _, _bpp, _INKNIT_CONCAT3(bpp, _, endian))

#define INKNIT_PUBLIC_FUNC(name, layout)  _INKNIT_CONCAT4(__INKNIT_PUBLIC_FUNCBASE, layout, _, name)
#define INKNIT_INTERNAL_FUNC(name, group) _INKNIT_CONCAT3(__INKNIT_INTERNAL_FUNCBASE, name, group)
#define INKNIT_INTERNAL_VAR(name, group)  _INKNIT_CONCAT3(__INKNIT_INTERNAL_VARBASE, name, group)

#define INKNIT_CURRENT_PUBLIC_FUNC(name)   INKNIT_PUBLIC_FUNC(name, INKNIT_CURRENT_LAYOUT)
#define INKNIT_CURRENT_INTERNAL_FUNC(name) INKNIT_INTERNAL_FUNC(name, INKNIT_CURRENT_GROUP)
#define INKNIT_CURRENT_INTERNAL_VAR(name)  INKNIT_INTERNAL_VAR(name, INKNIT_CURRENT_GROUP)
