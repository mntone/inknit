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

#define MSG_ERROR_IMAGE        " == NULL"
#define MSG_ERROR_IMAGE_LAYOUT "->pixel_layout != "
#define MSG_ERROR_IMAGE_ALIGN  "->alignment < "
#define MSG_ERROR_IMAGE_WIDTH  "->width > 2^11 - 1"
#define MSG_ERROR_IMAGE_HEIGHT "->height > 2^11 - 1"

#define MSG_CX_LESS_THAN_MIN_VALUE    "ERROR: cx < -2^11"
#define MSG_CX_GREATER_THAN_MAX_VALUE "ERROR: cx > 2^11 - 1"
#define MSG_CY_LESS_THAN_MIN_VALUE    "ERROR: cy < -2^11"
#define MSG_CY_GREATER_THAN_MAX_VALUE "ERROR: cy > 2^11 - 1"

#define MSG_DX_GREATER_THAN_MAX_VALUE "ERROR: dx > 2^11 - 1"
#define MSG_DY_GREATER_THAN_MAX_VALUE "ERROR: dy > 2^11 - 1"

#define MSG_MIN_X_LESS_THAN_ZERO         "ERROR: min_x < 0"
#define MSG_MIN_X_GREATER_THAN_MAX_X     "ERROR: min_x > max_x"
#define MSG_MAX_X_GREATER_THAN_MAX_VALUE "ERROR: max_x > 2^11 - 1"

#define MSG_MIN_Y_LESS_THAN_ZERO         "ERROR: min_y < 0"
#define MSG_MIN_Y_GREATER_THAN_MAX_Y     "ERROR: min_y > max_y"
#define MSG_MAX_Y_GREATER_THAN_MAX_VALUE "ERROR: max_y > 2^11 - 1"

#define MSG_RADIUS_LESS_THAN_ZERO         "ERROR: radius < 0"
#define MSG_RADIUS_GREATER_THAN_MAX_VALUE "ERROR: radius > 2^12 - 1"

#define MSG_RX_LESS_THAN_ZERO         "ERROR: rx < 0"
#define MSG_RX_GREATER_THAN_MAX_VALUE "ERROR: rx > 2^10 - 1"
#define MSG_RY_LESS_THAN_ZERO         "ERROR: ry < 0"
#define MSG_RY_GREATER_THAN_MAX_VALUE "ERROR: ry > 2^10 - 1"

#define MSG_SX_GREATER_THAN_MAX_VALUE "ERROR: sx > 2^11 - 1"
#define MSG_SY_GREATER_THAN_MAX_VALUE "ERROR: sy > 2^11 - 1"

#define MSG_WIDTH_LESS_THAN_ZERO          "ERROR: width < 0"
#define MSG_WIDTH_GREATER_THAN_MAX_VALUE  "ERROR: width > 2^11 - 1"
#define MSG_HEIGHT_LESS_THAN_ZERO         "ERROR: height < 0"
#define MSG_HEIGHT_GREATER_THAN_MAX_VALUE "ERROR: height > 2^11 - 1"

#define MSG_X_LESS_THAN_MIN_VALUE     "ERROR: x < -2^11"
#define MSG_X_LESS_THAN_ZERO          "ERROR: x < 0"
#define MSG_X_GREATER_THAN_MAX_VALUE  "ERROR: x > 2^11 - 1"
#define MSG_X1_LESS_THAN_MIN_VALUE    "ERROR: x1 < -2^11"
#define MSG_X1_LESS_THAN_ZERO         "ERROR: x1 < 0"
#define MSG_X1_GREATER_THAN_MAX_VALUE "ERROR: x1 > 2^11 - 1"
#define MSG_X1_GREATER_THAN_X2        "ERROR: x1 > x2"
#define MSG_X2_LESS_THAN_MIN_VALUE    "ERROR: x2 < -2^11"
#define MSG_X2_GREATER_THAN_MAX_VALUE "ERROR: x2 > 2^11 - 1"

#define MSG_Y_LESS_THAN_MIN_VALUE     "ERROR: y < -2^11"
#define MSG_Y_LESS_THAN_ZERO          "ERROR: y < 0"
#define MSG_Y_GREATER_THAN_MAX_VALUE  "ERROR: y > 2^11 - 1"
#define MSG_Y1_LESS_THAN_MIN_VALUE    "ERROR: y1 < -2^11"
#define MSG_Y1_LESS_THAN_ZERO         "ERROR: y1 < 0"
#define MSG_Y1_GREATER_THAN_Y2        "ERROR: y1 > y2"
#define MSG_Y2_GREATER_THAN_MAX_VALUE "ERROR: y2 > 2^11 - 1"

#define MSG_COLOR_GREATER_THAN_MAX_VALUE "ERROR: color > 2^bpp - 1"
