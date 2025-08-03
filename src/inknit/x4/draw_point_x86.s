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

	.global _inknit_draw_point32_4bpp_be
	.type   _inknit_draw_point32_4bpp_be, @function
	.align 4

	# ebx = data
	# edx = stride
	# ecx = x
	# eax = y
	# esi = color
_inknit_draw_point32_4bpp_be:
	imul	edx, eax				# edx = stride * y
	add		edx, ecx				# edx = pixel = stride * y + x

	lea		ecx, [4 * edx]			# ecx = pixel << 2
	shr		edx, 0x3				# edx = wordidx = pixel >> 3
	lea		edx, [ebx + 4 * edx]	# edx = ptr0 = data + (wordidx << 2)
	mov		eax, DWORD PTR [edx]	# eax = *ptr0

	not		cl						# cl  = pixoff = ~(pixel << 2)
	and		cl, 0x1c				# cl  = bitpos = pixoff & 0x1C
	mov		ebx, 0xf				# ebx = 0b1111
	shl		ebx, cl					# ebx = mask = 0b1111 << bitpos
	not		ebx						# ebx = ~mask

	bswap	eax						# eax   = read_wordval = swap(*ptr0)
	and		eax, ebx				# eax   = read_wordval & ~mask
	shl		esi, cl					# esi   = color_wordval = color << bitpos
	or		eax, esi				# eax   = (read_wordval & ~mask) | color_wordval
	bswap	eax						# eax   = write_wordval
	mov		DWORD PTR [edx], eax	# *ptr0 = write_wordval

	ret
