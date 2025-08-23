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
	.text

	.global _inknit_draw_vline32_4bpp_le
	.type   _inknit_draw_vline32_4bpp_le, %function
	.balign 4

	// r0      = data
	// r1      = stride
	// r2      = x
	// r3      = y1
	// sp[#16] = y2
	// sp[#20] = color
_inknit_draw_vline32_4bpp_le:
	push	{r4, r5, r6, lr}

	ldr		r5, [sp, #20]		// r5 = color

	and.w	r6, r2, #7			// r6 = x & 0x07
	lsls	r6, r6, #1			// r6 = bitpos = (x & 0x07) << 1

	movs	r4, #15				// r4 = 0b1111
	lsls	r4, r6				// r4 = mask = 0b1111 << bitpos
	lsls	r5, r6				// r5 = color_wordval = color << bitpos

	ldr		r6, [sp, #16]		// r6 = y2
	subs	r6, r6, r3			// r6 = y2 - y1
	adds	r6, r6, #1			// r6 = height = y2 - y1 + 1
	ble.n	.Ldone

	lsrs	r2, #3				// r2 = colidx_in_byte = x >> 3
	lsrs	r1, #2				// r1 = stride_in_byte = stride >> 2
	mla		r0, r1, r3, r0		// r0 = data + (rowidx = stride_in_byte * y1)
	add.w	r0, r0,	r2, lsl #2	// r0 = ptr = data + rowidx + (colidx_in_word = (x >> 3) << 2)

.Lloop:
	ldr		r3, [r0]			// r3 = read_wordval = *ptr
	bics	r3, r4				// r3 = read_wordval & ~mask
	orrs	r3, r5				// r3 = (read_wordval & ~mask) | color_wordval
	str		r3, [r0]			// *ptr = r3
	adds	r0, r0, r1			// r0 = ptr = data + stride_in_byte
	subs	r6, r6, #1			// r6 = r6 - 1
	bne.n	.Lloop

.Ldone:
	pop		{r4, r5, r6, pc}
