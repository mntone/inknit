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

#include <termios.h>  // termios, tcgetattr, tcsetattr
#include <unistd.h>   // isatty, STDIN_FILENO

// A class to reliably restore the terminal mode using the RAII pattern.
struct termmode_guard final {
	termmode_guard() noexcept {
		if (isatty(STDIN_FILENO)) {
			tcgetattr(STDIN_FILENO, &prev_settings_);
			valid_ = true;
		}
	}

	~termmode_guard() noexcept {
		if (valid_) {
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &prev_settings_);
		}
	}

	inline constexpr termios prev_settings() const noexcept {
		return prev_settings_;
	}

private:
	termmode_guard(termmode_guard const&)            = delete;
	termmode_guard& operator=(termmode_guard const&) = delete;

private:
	bool    valid_ {false};
	termios prev_settings_;
};
