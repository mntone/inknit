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
	.global	_inknit_draw_point32_2bpp_le
	.type	_inknit_draw_point32_2bpp_le, %function
	.balign 4

	// r0 = data
	// r1 = stride
	// r2 = x
	// r3 = y
	// r4 = color
_inknit_draw_point32_2bpp_le:
	muls	r3, r1			// r3 = stride * y
	adds	r3, r3, r2		// r3 = pixel = stride * y + x

	movs	r1, #3			// r1 = 0b11
	movs	r2, #15			// r2 = 0x0F
	ands	r2, r3			// r2 = pixoff = pixel & 0x0F
	lsls	r2, r2, #1		// r2 = bitpos = pixoff << 1
	lsls	r1, r2			// r1 = mask = 0b11 << bitpos

	lsrs	r3, r3, #4		// r3 = wordidx = pixel >> 4
	lsls	r3, r3, #2		// r3 = byteidx = wordidx << 2
	adds	r0, r0, r3		// r0 = ptr0 = data + byteidx

	ldr		r3, [r0]		// r3 = read_wordval = *r0 = *ptr0
	bics	r3, r1			// r3 = read_wordval & ~mask

	lsls	r4, r2			// r4 = color_wordval = color << bitpos
	orrs	r3, r4			// r3 = write_wordval = (read_wordval & ~mask) | color_wordval
	str		r3, [r0]		// *r0 = write_wordval

	bx		lr
