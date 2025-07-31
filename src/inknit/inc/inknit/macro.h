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

#ifdef INKNIT_SHARED
#if defined(__GNUC__) || defined(__clang__)
#ifdef INKNIT_EXPORTS
#define INKNIT_EXPORT __attribute__((visibility("default")))
#else
#define INKNIT_EXPORT
#endif
#elif defined(_MSC_VER)
#ifdef INKNIT_EXPORTS
#define INKNIT_EXPORT __declspec(dllexport)
#else
#define INKNIT_EXPORT __declspec(dllimport)
#endif
#else
#define INKNIT_EXPORT
#endif
#else
#define INKNIT_EXPORT
#endif

#if defined(__GNUC__) || defined(__clang__)
#define INKNIT_NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#define INKNIT_NONNULL(...)
#endif

#ifdef __cplusplus
#if defined(_MSC_VER)
#define INKNIT_RESTRICT __restrict
#else
#define INKNIT_RESTRICT __restrict__
#endif
#else
#define INKNIT_RESTRICT restrict
#endif
