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

#include <cstring>        // strcmp
#include <functional>     // functional
#include <memory>         // unique_ptr
#include <string>         // string
#include <unordered_map>  // unordered_map
#include <vector>         // vector

#include "base.hpp"
#include "types.hpp"  // includes <any>, <cstdint>, <string_view>

namespace inkbm {

struct argument final {
	char const *const short_name;
	char const *const name;
	std::any          value;
};

struct arguments final {
	char const           *header {nullptr};
	std::vector<argument> items;
};

class unit_executor final: public executor {
public:
	constexpr explicit unit_executor(char const *const name, void (*const execute)()) noexcept
		: name_(name)
		, execute_(execute) {}

	INKBM_NODISCARD
	statistics execute(monitor *monitor, std::any const *data) const noexcept override;

	INKBM_NODISCARD
	INKBM_ALWAYS_INLINE
	constexpr std::string_view name() const noexcept override {
		return name_;
	}

private:
	char const *const name_;
	void (*const execute_)();
};

class fixture_executor final: public executor {
public:
	constexpr explicit fixture_executor(
		char const *const fixture_name,
		char const *const name,
		std::unique_ptr<fixture> (*const get)()
	) noexcept
		: fixture_name_(fixture_name)
		, name_(name)
		, test_name_(std::string(fixture_name) + '/' + name)
		, get_(get) {}

	INKBM_NODISCARD
	statistics execute(monitor *monitor, std::any const *data) const noexcept override;

	INKBM_NODISCARD
	INKBM_ALWAYS_INLINE
	constexpr std::string_view name() const noexcept override {
		return test_name_;
	}

private:
	char const *const fixture_name_;
	char const *const name_;
	std::string       test_name_;
	std::unique_ptr<fixture> (*const get_)();
};

class group final {
public:
	using item_type = std::unique_ptr<executor>;

	constexpr void add(item_type executor) noexcept {
		cases_.emplace_back(std::move(executor));
	}

	INKBM_NODISCARD
	constexpr inkbm::arguments arguments() const noexcept {
		return args_;
	}
	constexpr void arguments(inkbm::arguments args) noexcept {
		args_ = std::move(args);
	}

	INKBM_NODISCARD
	constexpr std::vector<item_type> const& cases() const noexcept {
		return cases_;
	}

private:
	inkbm::arguments       args_;
	std::vector<item_type> cases_;
};

class registry final {
public:
	static registry& instance() noexcept {
		static registry instance;
		return instance;
	}

	constexpr void add(std::unique_ptr<executor> executor) noexcept {
		cases_.emplace_back(std::move(executor));
	}

	void run_all(bool compatible_mode = false) noexcept;
	void print_stats(statistics const& stats, std::uint32_t offset) noexcept;

	INKBM_NODISCARD
	inline inkbm::group& group(char const *const name) noexcept {
		auto it = groups_.find(name);
		if (it == groups_.end()) {
			it = groups_.emplace(name, inkbm::group()).first;
		}
		return it->second;
	}

	INKBM_NODISCARD
	inline inkbm::group const& group(char const *const name) const noexcept {
		auto it = groups_.find(name);
		if (it == groups_.cend()) {
			INKBM_FATAL("Missing group.");
		}
		return it->second;
	}

private:
	struct cstr_hash {
		using is_transparent = void;
		INKBM_NODISCARD
		inline std::size_t operator()(char const *str) const {
			return std::hash<std::string_view> {}(str);
		}
	};
	struct cstr_equal {
		using is_transparent = void;
		INKBM_NODISCARD
		inline bool operator()(char const *lhs, char const *rhs) const {
			return std::strcmp(lhs, rhs) == 0;
		}
	};

	registry() noexcept                  = default;
	registry(registry const&)            = delete;
	registry& operator=(registry const&) = delete;

	std::vector<std::unique_ptr<executor>>                                cases_;
	std::unordered_map<char const *, inkbm::group, cstr_hash, cstr_equal> groups_;
};

namespace details {

	class registrar final {
	public:
		explicit registrar(char const *const name, void (*const execute)()) noexcept {
			std::unique_ptr<executor> executor {std::make_unique<unit_executor>(name, execute)};
			registry::instance().add(std::move(executor));
		}
	};

	class fixture_registrar final {
	public:
		explicit fixture_registrar(
			char const *const fixture_name,
			char const *const name,
			std::unique_ptr<fixture> (*const get)()
		) {
			std::unique_ptr<executor> executor {
				std::make_unique<fixture_executor>(fixture_name, name, get)
			};
			registry::instance().add(std::move(executor));
		}
	};

	class arguments_registrar final {
	public:
		explicit arguments_registrar(
			char const *const group_name, char const *const name, std::vector<argument> args
		) {
			registry::instance().group(group_name).arguments({name, args});
		}
	};

	class fixture_group_registrar final {
	public:
		explicit fixture_group_registrar(
			char const *const group_name,
			char const *const fixture_name,
			char const *const name,
			std::unique_ptr<fixture> (*const get)()
		) noexcept {
			std::unique_ptr<executor> executor {
				std::make_unique<fixture_executor>(fixture_name, name, get)
			};
			registry::instance().group(group_name).add(std::move(executor));
		}
	};

}  // namespace details

}  // namespace inkbm

#define INKBM(name)                                                                   \
	extern "C" void                        _inkbm__bm__##name() noexcept;             \
	static inkbm::details::registrar const __reg__##name(#name, &_inkbm__bm__##name); \
	void                                   _inkbm__bm__##name() noexcept

#define INKBM_FIXTURE(name, fixture_name)                                             \
	struct _inkbm__##fixture_name##__##name final: public fixture_name {              \
		void execute() noexcept override;                                             \
	};                                                                                \
	static inkbm::details::fixture_registrar const __regf__##fixture_name##__##name { \
		#fixture_name, #name, []() noexcept -> std::unique_ptr<inkbm::fixture> {      \
			return std::make_unique<_inkbm__##fixture_name##__##name>();              \
		}                                                                             \
	};                                                                                \
	void _inkbm__##fixture_name##__##name::execute() noexcept

#define INKBM_ARGS(args_name, header, ...)                                     \
	static inkbm::details::arguments_registrar const __reg_args__##args_name { \
		#args_name, header, __VA_ARGS__                                        \
	}

#define INKBM_FIXTURE_APPLY(name, args_name, fixture_name)                                       \
	struct _inkbm__##args_name##__##name##__##fixture_name final: public fixture_name {          \
		void execute() noexcept override;                                                        \
	};                                                                                           \
	static inkbm::details::fixture_group_registrar const                                         \
		__regf_apply__##args_name##__##name##__##fixture_name {                                  \
			#args_name, #fixture_name, #name, []() noexcept -> std::unique_ptr<inkbm::fixture> { \
				return std::make_unique<_inkbm__##args_name##__##name##__##fixture_name>();      \
			}                                                                                    \
		};                                                                                       \
	void _inkbm__##args_name##__##name##__##fixture_name::execute() noexcept
