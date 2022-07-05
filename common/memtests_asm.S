#include "memtests.h"
#include <arm.h>

.section .text
.arm

memtest_fail_own_address_thunk:
	mov r3, r2
memtest_fail:
	cmp r1, #MAX_ERRORS
	bhs memtest_fail_return

	push {r5,r6}
	mov r5, #ERROR_STRUCT_SIZE
	mov r6, r1
	mul r6, r5
	ldr r5, =errors_buffer
	add r5, r6
	stm r5!, {r2-r4}

	pop {r5,r6}
	memtest_fail_return:
	add r1, #1
	bx lr

asm_func memtest_walking_ones
	push {r4-r7,lr}
	mov r12, r1
	mov r1, #0
	mov r7, r0
	@ Not sure of the specfic reasoning for this pattern, but it's what memtest86+ uses
	ldr r3, =0x5555AAAA
	walking_ones_outer_loop:
		str r3, [r7]
		mvn r5, r3

		mov r6, #4
		walking_ones_forward_loop:
			add r2, r7, r6
			cmp r2, r12
			bhs walking_ones_forward_break
			str r5, [r2]
			ldr r4, [r7]
			cmp r3, r4
			blne memtest_fail
			lsl r6, #1
			b walking_ones_forward_loop
		walking_ones_forward_break:

		mov r6, #4
		walking_ones_backward_loop:
			sub r2, r7, r6
			cmp r2, r0
			blo walking_ones_backward_break
			str r5, [r2]
			ldr r4, [r7]
			cmp r3, r4
			blne memtest_fail
			lsl r6, #1
			b walking_ones_backward_loop
		walking_ones_backward_break:

		@ Advance the initial position by 8KB. memtest86+ uses 1MB steps here, but some entire
		@ memory regions on the 3DS are smaller than that. 8KB is probably much smaller than
		@ this needs to be, but the test still finishes within seconds on FCRAM so it's fine.
		add r7, #(1<<13)
		cmp r7, r12
		blo walking_ones_outer_loop

		@ Rerun the entire test with the inverse of the initial pattern
		mov r7, r0
		mvns r3, r3
		bmi walking_ones_outer_loop

	mov r0, r1
	pop {r4-r7,pc}

asm_func memtest_own_address
	push {r4,lr}
	mov r12, r1
	mov r2, r0

	own_address_fill_loop:
		str r2, [r2], #4
		cmp r2, r12
		blo own_address_fill_loop

	mov r2, r0
	mov r1, #0

	own_address_check_loop:
		ldr r4, [r2]
		cmp r2, r4
		blne memtest_fail_own_address_thunk
		add r2, #4
		cmp r2, r12
		blo own_address_check_loop

	mov r0, r1
	pop {r4,pc}

moving_inversions_fill:
	mov r2, r0
	moving_inversions_fill_loop:
		str r3, [r2], #4
		cmp r2, r12
		blo moving_inversions_fill_loop
	bx lr

moving_inversions_bottom_up:
	push {lr}
	mov r2, r0
	moving_inversions_bottom_up_loop:
		ldr r4, [r2]
		cmp r3, r4
		blne memtest_fail
		str r5, [r2], #4
		cmp r2, r12
		blo moving_inversions_bottom_up_loop
	pop {pc}

moving_inversions_top_down:
	push {lr}
	mov r2, r12
	moving_inversions_top_down_loop:
		sub r2, #4
		ldr r4, [r2]
		cmp r3, r4
		blne memtest_fail
		str r5, [r2]
		cmp r2, r0
		bhi moving_inversions_top_down_loop
	pop {pc}

asm_func memtest_moving_inversions_ones_zeros
	push {r4-r6,lr}
	mov r12, r1
	mov r1, #0
	mov r3, #0
	mvn r5, r3

	moving_inversions_ones_zeros_repeat:
	mov r6, #6
	bl moving_inversions_fill
	moving_inversions_ones_zeros_loop:
		bl moving_inversions_bottom_up
		mov r3, r5
		mvn r5, r3
		bl moving_inversions_top_down
		mov r3, r5
		mvn r5, r3
		subs r6, #1
		bne moving_inversions_ones_zeros_loop
	mov r3, r5
	mvns r5, r3
	bpl moving_inversions_ones_zeros_repeat

	mov r0, r1
	pop {r4-r6,pc}

asm_func memtest_moving_inversions_8bit_walking
	push {r4-r7,lr}
	ldr r3, =0x01010101
	mov r12, r1
	mov r1, #0

	moving_inversions_8bit_walking_outer_loop:
		mov r7, #2
		mvn r5, r3

		moving_inversions_8bit_walking_repeat:
			@mov r6, #6
			mov r6, #3
			bl moving_inversions_fill
			moving_inversions_8bit_walking_loop:
				bl moving_inversions_bottom_up
				mov r3, r5
				mvn r5, r3
				bl moving_inversions_top_down
				mov r3, r5
				mvn r5, r3
				subs r6, #1
				bne moving_inversions_8bit_walking_loop
			mov r3, r5
			mvn r5, r3
			subs r7, #1
			bne moving_inversions_8bit_walking_repeat

		lsl r3, #1
		lsls r4, r3, #24
		bne moving_inversions_8bit_walking_outer_loop

	mov r0, r1
	pop {r4-r7,pc}

asm_func memtest_bit_fade_check
	push {r4,lr}

	mov r3, r2
	mov r2, r0
	mov r12, r1
	mov r1, #0

	bit_fade_check_loop:
		ldr r4, [r2], #4
		cmp r3, r4
		blne memtest_fail
		cmp r2, r12
		blo bit_fade_check_loop

	mov r0, r1
	pop {r4,pc}

.ltorg

.section .bss

.global errors_buffer
errors_buffer:
.space (MAX_ERRORS * ERROR_STRUCT_SIZE)
