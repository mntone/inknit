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

	.global _inknit_draw_vline32_1bpp_be
	.type   _inknit_draw_vline32_1bpp_be, @function
	.align 4

_inknit_draw_vline32_1bpp_be:
	push	ebp
	mov		ebp, esp
	push	ebx
	push	edi
	push	esi

	mov		edx, [ebp+16]		# edx = x
	mov		eax, [ebp+20]		# eax = y1
	mov		esi, [ebp+28]		# esi = color

	mov		ecx, edx			# ecx = x
	and		ecx, 0x07			# ecx = x & 0x07
	xor		ecx, 0x07			# ecx = bitpos = (x & 0x07) ^ 0x07
	shl		esi, cl				# esi = color_byteval = color << bitpos[4:0]
	mov		ebx, ecx			# ebx = bitpos

	mov		ecx, [ebp+24]		# ecx = y2
	sub		ecx, eax			# ecx = y2 - y1
	inc		ecx					# ecx = y2 - y1 + 1
	jle		.Ldone				# y2 < y1

	mov		edi, [ebp+8]		# edi = data
	shr		edx, 3				# edx = colidx = x >> 3
	add		edi, edx			# edi = data + colidx

	mov		edx, [ebp+12]		# edx = stride
	shr		edx, 3				# edx = stride_in_byte = stride >> 3
	imul	eax, edx			# eax = rowidx = (stride >> 3) * y1
	add		edi, eax			# edi = ptr = data + rowidx + colidx

.align 16
.Lloop:
	movzx	eax, BYTE PTR [edi]	# eax  = read_byteval = *ptr
	btr		eax, ebx			# eax  = read_byteval & ~(0b1 << bitpos[4:0])
	or		eax, esi			# eax  = (read_byteval & ~(0b1 << bitpos[4:0])) | color_byteval
	mov		BYTE PTR [edi], al	# *ptr = al
	add		edi, edx			# edi  = ptr = data + stride_in_byte
	dec		ecx					# ecx  = ecx - 1
	jnz		.Lloop

.Ldone:
	pop		esi
	pop		edi
	pop		ebx
	mov		esp, ebp
	pop		ebp
	ret
