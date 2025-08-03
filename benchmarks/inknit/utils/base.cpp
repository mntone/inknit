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

#include "base.hpp"
#include "platform.h"

#include <cstdarg>  // va_list
#include <cstdlib>  // fputs, stderr
#include <string>   // string

static constexpr char const *FATAL_HEADER = INKBM_HEADER "[FATAL] ";
static constexpr char const *WARN_HEADER  = INKBM_HEADER "[WARN] ";

#ifdef _MSC_VER
#pragma comment(linker, "/alternatename:inkbm_fatal_handler=inkbm_fatal_handler_default")
#pragma comment(linker, "/alternatename:inkbm_error_handler=inkbm_error_handler_default")

#define inkbm_fatal_handler inkbm_fatal_handler_default
#define inkbm_error_handler inkbm_error_handler_default
#endif

void inkbm_fatal_handler(char const *format, ...) noexcept {
	va_list format_list;
	va_start(format_list, format);

	va_list args_list;
	va_copy(args_list, format_list);

	std::size_t size = static_cast<std::size_t>(std::vsnprintf(nullptr, 0, format, format_list));
	va_end(format_list);
	size += 1;

	std::string       message {FATAL_HEADER};
	std::size_t const offset = message.size();
	message.resize(offset + size);

	std::vsnprintf(message.data() + offset, size, format, args_list);
	va_end(args_list);

	message.back() = '\n';
	std::fputs(message.data(), stderr);

	do {
		inkbm_platform_sleep(1000);
	} while (true);
	INKBM_UNREACHABLE;
}

void inkbm_error_handler(INKBM_MAYBE_UNUSED int level, char const *format, ...) noexcept {
	va_list format_list;
	va_start(format_list, format);

	va_list args_list;
	va_copy(args_list, format_list);

	std::size_t size = static_cast<std::size_t>(std::vsnprintf(nullptr, 0, format, format_list));
	va_end(format_list);
	size += 1;

	std::string       message {WARN_HEADER};
	std::size_t const offset = message.size();
	message.resize(offset + size);

	std::vsnprintf(message.data() + offset, size, format, args_list);
	va_end(args_list);

	message.back() = '\n';
	std::fputs(message.data(), stderr);
}
