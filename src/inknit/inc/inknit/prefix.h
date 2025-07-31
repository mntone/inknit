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

#define __INKNIT_PUBLIC_FUNCNAME_BASE   inknit_
#define __INKNIT_INTERNAL_FUNCNAME_BASE _ikt
#define __INKNIT_INTERNAL_VARNAME_BASE  __ikt

#define _INKNIT_CONCAT_IMPL(x, y)   x##y
#define _INKNIT_CONCAT2(x, y)       _INKNIT_CONCAT_IMPL(x, y)
#define _INKNIT_CONCAT3(x, y, z)    _INKNIT_CONCAT2(x, _INKNIT_CONCAT2(y, z))
#define _INKNIT_CONCAT4(x, y, z, w) _INKNIT_CONCAT2(x, _INKNIT_CONCAT3(y, z, w))

#define INKNIT_PUBLIC_FUNCNAME(group, name)                        \
	_INKNIT_CONCAT4(__INKNIT_PUBLIC_FUNCNAME_BASE, group, _, name)
#define INKNIT_INTERNAL_FUNCNAME(group, name)                        \
	_INKNIT_CONCAT4(__INKNIT_INTERNAL_FUNCNAME_BASE, group, _, name)
#define INKNIT_INTERNAL_VARNAME(group, name)                        \
	_INKNIT_CONCAT4(__INKNIT_INTERNAL_VARNAME_BASE, group, _, name)
