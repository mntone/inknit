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

	.intel_syntax noprefix
	.text

	.global _inknit_draw_point32_1bpp_be
	.type   _inknit_draw_point32_1bpp_be, @function
	.align 4

	# ebx = data
	# edx = stride
	# ecx = x
	# eax = y
	# esi = color
_inknit_draw_point32_1bpp_be:
	imul	edx, eax				# edx = stride * y
	add		edx, ecx				# edx = pixel = stride * y + x

	mov		ecx, edx				# ecx   = pixel
	shr		edx, 0x5				# edx   = wordidx = pixel >> 5
	lea		edx, [ebx + 4 * edx]	# edx   = ptr0 = data + (wordidx << 2)
	mov		eax, DWORD PTR [edx]	# eax   = *ptr0
	not		cl						# cl    = bitpos = ~pixel[7:0]
	shl		esi, cl					# esi   = color_wordval = color << bitpos[4:0]
	bswap	eax						# eax   = read_wordval = swap(*ptr0)
	btr		eax, ecx				# eax   = base_wordval = read_wordval & ~(0b1 << bitpos[4:0])
	or		eax, esi				# eax   = base_wordval | color_wordval
	bswap	eax						# eax   = write_wordval
	mov		DWORD PTR [edx], eax	# *ptr0 = write_wordval

	ret
