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

	.syntax unified
	.thumb
	.thumb_func
	.global	_inknit_draw_point32_1bpp_le
	.type	_inknit_draw_point32_1bpp_le, %function
	.balign 4

	// r0 = data
	// r1 = stride
	// r2 = x
	// r3 = y
	// r4 = color
_inknit_draw_point32_1bpp_le:
	mla		r3, r3, r1, r2	// r3 = pixel = stride * y + x

	lsrs	r1, r3, #5		// r1 = wordidx = pixel >> 5
	lsls	r1, r1, #2		// r1 = byteidx = wordidx << 2
	adds	r0, r0, r1		// r0 = ptr0 = data + byteidx
	ldr		r1, [r0]		// r1 = read_wordval = *r0 = *ptr0

	movs	r2, #1			// r2 = 0b1
	and.w	r3, r3, #31		// r3 = bitpos = pixel & 0x1F
	lsls	r2, r3			// r2 = mask = 0b1 << bitpos

	bics	r1, r2			// r1  = read_wordval & ~mask
	lsls	r4, r3			// r4  = color_wordval = color << bitpos
	orrs	r1, r4			// r1  = write_wordval = (read_wordval & ~mask) | color_wordval
	str		r1, [r0]		// *r0 = write_wordval

	bx		lr
