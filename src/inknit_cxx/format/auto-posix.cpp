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

#include "../inc/inknit/format.hpp"

#ifndef INKNIT_TARGET_IS_PICO
#include "../utils/termmode_guard.hpp"

#include <cstdlib>    // getenv
#include <termios.h>  // tcsetattr
#include <unistd.h>   // isatty
#endif


// -- MARK: use namespaces
using namespace inknit;
using namespace inknit::format;


// -- MARK: utils
#ifndef INKNIT_TARGET_IS_PICO
static bool supports_256color() noexcept {
	std::string_view term = getenv("TERM");
	return term.ends_with("256color");
}

static bool supports_sixel() noexcept {
	// If standard input is not a terminal, assume sixel is not supported.
	if (!isatty(STDIN_FILENO)) {
		return false;
	}

	// This guard object will automatically restore the settings when it goes out of scope.
	termmode_guard terminal_context;
	termios        settings = terminal_context.prev_settings();

	// Set to raw mode.
	// - ISIG:   Disables signal characters (like Ctrl-C).
	// - ICANON: Disables canonical mode (input is not processed line-by-line).
	// - ECHO:   Disables input echoing (keystrokes are not displayed).
	settings.c_lflag &= static_cast<tcflag_t>(~(ISIG | ICANON | ECHILD));

	// Timeout settings: wait for 0.1 seconds.
	settings.c_cc[VMIN]  = 0;  // Set minimum characters for read to 0.
	settings.c_cc[VTIME] = 1;  // Set timeout to 1 (unit is 1/10th of a second).

	// Apply the modified settings to the terminal.
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &settings);

	// Send the Device Attributes (DA) query.
	char const query[] = "\x1b[c";
	if (write(STDOUT_FILENO, query, 3) == -1) {
		return false;  // Write failed.
	}

	// Read the response from the terminal.
	char                  buffer[16];
	constexpr std::size_t buffer_length = sizeof(buffer) / sizeof(buffer[0]);

	std::string response;
	ssize_t     bytes_read = 0;
	while ((bytes_read = read(STDIN_FILENO, buffer, buffer_length)) > 0) {
		response.append(buffer, static_cast<std::size_t>(bytes_read));

		// The response is terminated by the character 'c'.
		if (response.back() == 'c') {
			break;
		}
	}

	// Check if the response contains ";4".
	return response.find(";4") != std::string::npos;
}
#endif

template<pixel_layout PIXEL_LAYOUT, pixel_format PIXEL_FORMAT>
static std::shared_ptr<format::image_formatter> make_formatter() noexcept {
	static std::shared_ptr<format::image_formatter> formatter;

	if (!formatter) {
#ifdef INKNIT_TARGET_IS_PICO
		formatter = std::make_unique<format::character_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>();
#else
		if (supports_sixel()) {
			formatter = std::make_unique<format::sixel_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>();
		} else if (supports_256color()) {
			formatter = std::make_unique<format::block_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>();
		} else {
			formatter = std::make_unique<format::character_formatter<PIXEL_LAYOUT, PIXEL_FORMAT>>();
		}
#endif
	}
	return formatter;
}


// -- MARK: layout=x1, format=grayscale

template<>
auto_formatter<pixel_layout::x1, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x1, pixel_format::grayscale>()) {}


// -- MARK: layout=x1lsb, format=grayscale

template<>
auto_formatter<pixel_layout::x1lsb, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x1lsb, pixel_format::grayscale>()) {}


// -- MARK: layout=x2, format=grayscale

template<>
auto_formatter<pixel_layout::x2, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x2, pixel_format::grayscale>()) {}


// -- MARK: layout=x2lsb, format=grayscale

template<>
auto_formatter<pixel_layout::x2lsb, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x2lsb, pixel_format::grayscale>()) {}


// -- MARK: layout=x4, format=grayscale

template<>
auto_formatter<pixel_layout::x4, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x4, pixel_format::grayscale>()) {}


// -- MARK: layout=x4lsb, format=grayscale

template<>
auto_formatter<pixel_layout::x4lsb, pixel_format::grayscale>::auto_formatter() noexcept
	: formatter_(make_formatter<pixel_layout::x4lsb, pixel_format::grayscale>()) {}
