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

#if !NDEBUG

#include <string>

namespace inknit::tests {

class assert_helper final {
public:
	static assert_helper& instance() noexcept {
		static assert_helper instance;
		return instance;
	}

	void hook() noexcept;
	void reset() noexcept;

	constexpr bool fired() const noexcept {
		return fired_;
	}
	constexpr std::string message() const noexcept {
		return message_;
	}
	constexpr std::string condition() const noexcept {
		return condition_;
	}
	constexpr std::string file() const noexcept {
		return file_;
	}
	constexpr std::string func() const noexcept {
		return func_;
	}
	constexpr int line() const noexcept {
		return line_;
	}

private:
	assert_helper() noexcept                       = default;
	assert_helper(assert_helper const&)            = delete;
	assert_helper& operator=(assert_helper const&) = delete;

	~assert_helper() noexcept;

	void assert_callback(
		char const *message, char const *condition, char const *file, char const *func, int line
	) noexcept;

	static void assert_callback_static(
		char const *message,
		char const *condition,
		char const *file,
		char const *func,
		int         line,
		void       *data
	) noexcept {
		static_cast<assert_helper *>(data)->assert_callback(message, condition, file, func, line);
	}

private:
	void *handler_, *data_;

	bool        fired_;
	std::string message_, condition_, file_, func_;
	int         line_;
};

}  // namespace inknit::tests

#endif
