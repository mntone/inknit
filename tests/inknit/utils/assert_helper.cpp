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

#include "assert_helper.hpp"

#include "inknit/proto.h"

using namespace inknit::tests;

void assert_helper::hook() noexcept {
	inknit_assert_info prev_info = inknit_hook_assert(&assert_helper::assert_callback_static, this);

	handler_ = reinterpret_cast<void *>(prev_info.handler);
	data_    = prev_info.data;
}

void assert_helper::reset() noexcept {
	message_.clear();
	condition_.clear();
	file_.clear();
	func_.clear();
	line_  = 0;
	fired_ = false;
}

assert_helper::~assert_helper() noexcept {
	inknit_hook_assert(reinterpret_cast<inknit_assert_t>(handler_), data_);
}

void assert_helper::assert_callback(
	char const *message, char const *condition, char const *file, char const *func, int line
) noexcept {
	if (!fired_) {
		message_   = message;
		condition_ = condition;
		file_      = file;
		func_      = func;
		line_      = line;
		fired_     = true;
	}
}
