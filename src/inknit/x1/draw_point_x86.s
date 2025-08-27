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

	mov		ecx, edx				# ecx = pixel
	shr		edx, 0x3				# edx = byteidx = pixel >> 3
	add		edx, ebx				# edx = ptr0 = data + byteidx
	movzx	eax, BYTE PTR [edx]		# eax = *ptr0

	and		ecx, 0x07				# ecx = x & 0x07
	xor		ecx, 0x07				# ecx = bitpos = (x & 0x07) ^ 0x07
	shl		esi, cl					# esi = color_byteval = color << bitpos

	btr		eax, ecx				# eax   = base_wordval = read_byteval & ~(0b1 << bitpos)
	or		eax, esi				# eax   = base_wordval | color_byteval
	mov		BYTE PTR [edx], al		# *ptr0 = write_wordval

	ret
