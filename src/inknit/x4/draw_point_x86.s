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
	shr		edx, 0x01				# edx = byteidx = pixel >> 1
	add		edx, ebx				# edx = ptr0 = data + byteidx
	movzx	eax, BYTE PTR [edx]		# eax = *ptr0

	and		ecx, 0x04				# ecx = (x << 2) & 0x04
	xor		ecx, 0x04				# ecx = bitpos = ((x << 2) & 0x04) ^ 0x04
	mov		ebx, 0xf				# ebx = 0b1111
	shl		ebx, cl					# ebx = mask = 0b1111 << bitpos
	not		ebx						# ebx = ~mask

	and		eax, ebx				# eax   = read_byteval & ~mask
	shl		esi, cl					# esi   = color_byteval = color << bitpos
	or		eax, esi				# eax   = (read_byteval & ~mask) | color_byteval
	mov		BYTE PTR [edx], al		# *ptr0 = eax

	ret
