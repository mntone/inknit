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
	.global	_inknit_draw_point32_2bpp_be
	.type	_inknit_draw_point32_2bpp_be, %function
	.balign 4

	// r0 = data
	// r1 = stride
	// r2 = x
	// r3 = y
	// r4 = color
_inknit_draw_point32_2bpp_be:
	mla		r3, r3, r1, r2	// r3 = pixel = stride * y + x

	lsrs	r1, r3, #2		// r1 = byteidx = pixel >> 2
	adds	r0, r0, r1		// r0 = ptr0 = data + byteidx
	ldrb	r1, [r0]		// r1 = *ptr0

	mvn.w	r3, r3, lsl #1	// r3 = pixoff = ~(pixel << 1)
	and.w	r3, r3, #6		// r3 = bitpos = pixoff & 0x06
	movs	r2, #3			// r2 = 0b11
	lsls	r2, r3			// r2 = mask = 0b11 << bitpos

	bics	r1, r2			// r1  = read_byteval & ~mask
	lsls	r4, r3			// r4  = color_byteval = color << bitpos
	orrs	r1, r4			// r1  = (read_byteval & ~mask) | color_byteval
	strb	r1, [r0]		// *r0 = r1

	bx		lr
