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

#define MSG_DST_IS_NULL   "ERROR: dst is NULL"
#define MSG_IMAGE_IS_NULL "ERROR: image is NULL"
#define MSG_SRC_IS_NULL   "ERROR: src is NULL"

#define MSG_DST_IS_INVALID_LAYOUT   "ERROR: dst is invalid pixel layout"
#define MSG_IMAGE_IS_INVALID_LAYOUT "ERROR: image is invalid pixel layout"
#define MSG_SRC_IS_INVALID_LAYOUT   "ERROR: src is invalid pixel layout"

#define MSG_DST_IS_INVALID_FORMAT   "ERROR: dst is invalid pixel format"
#define MSG_IMAGE_IS_INVALID_FORMAT "ERROR: image is invalid pixel format"
#define MSG_SRC_IS_INVALID_FORMAT   "ERROR: src is invalid pixel format"

#define MSG_DST_IS_INVALID_ALIGN   "ERROR: dst is invalid alignment"
#define MSG_IMAGE_IS_INVALID_ALIGN "ERROR: image is invalid alignment"
#define MSG_SRC_IS_INVALID_ALIGN   "ERROR: src is invalid alignment"

#define MSG_CX_GREATER_THAN_MIN_VALUE "ERROR: cx < -2^12 - 1"
#define MSG_CX_GREATER_THAN_MAX_VALUE "ERROR: cx > 2^12"
#define MSG_CY_GREATER_THAN_MIN_VALUE "ERROR: cy < -2^12 - 1"
#define MSG_CY_GREATER_THAN_MAX_VALUE "ERROR: cy > 2^12"

#define MSG_DX_GREATER_THAN_MAX_VALUE "ERROR: dx > 2^12"
#define MSG_DY_GREATER_THAN_MAX_VALUE "ERROR: dy > 2^12"

#define MSG_RADIUS_LESS_THAN_ZERO         "ERROR: radius < 0"
#define MSG_RADIUS_GREATER_THAN_MAX_VALUE "ERROR: radius > 2^13 - 1"

#define MSG_SX_GREATER_THAN_MAX_VALUE "ERROR: sx > 2^12"
#define MSG_SY_GREATER_THAN_MAX_VALUE "ERROR: sx > 2^12"

#define MSG_X_LESS_THAN_MIN_VALUE     "ERROR: x < -2^12 - 1"
#define MSG_X_LESS_THAN_ZERO          "ERROR: x < 0"
#define MSG_X_GREATER_THAN_MAX_VALUE  "ERROR: x > 2^12"
#define MSG_X1_LESS_THAN_MIN_VALUE    "ERROR: x1 < -2^12 - 1"
#define MSG_X1_LESS_THAN_ZERO         "ERROR: x1 < 0"
#define MSG_X1_GREATER_THAN_X2        "ERROR: x1 > x2"
#define MSG_X2_GREATER_THAN_MAX_VALUE "ERROR: x2 > 2^12"

#define MSG_Y_LESS_THAN_MIN_VALUE     "ERROR: y < -2^12 - 1"
#define MSG_Y_LESS_THAN_ZERO          "ERROR: y < 0"
#define MSG_Y_GREATER_THAN_MAX_VALUE  "ERROR: y > 2^12"
#define MSG_Y1_LESS_THAN_MIN_VALUE    "ERROR: y1 < -2^12 - 1"
#define MSG_Y1_LESS_THAN_ZERO         "ERROR: y1 < 0"
#define MSG_Y1_GREATER_THAN_Y2        "ERROR: y1 > y2"
#define MSG_Y2_GREATER_THAN_MAX_VALUE "ERROR: y2 > 2^12"

#define MSG_COLOR_GREATER_THAN_MAX_VALUE "ERROR: color > 2^bpp - 1"
