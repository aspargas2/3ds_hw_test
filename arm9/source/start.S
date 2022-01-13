#include "common.h"

.section .text.start
.global _start
_start:
	mov r9, r0  @ argc
	mov r10, r1 @ argv

	@ Change the stack pointer to the end of DTCM
	ldr sp, =(0xFFF00000 + 0x4000)

	@ Configure TCMs
	ldr r0, =0xFFF0000A
	ldr r1, =0x00000024
	mcr p15, 0, r0, c9, c1, 0 @ DTCM
	mcr p15, 0, r1, c9, c1, 1 @ ITCM

	@ Disable caches / mpu, enable TCMs
	mrc p15, 0, r4, c1, c0, 0 @ read control register
	orr r4, #(1<<18)          @ - itcm enable
	orr r4, #(1<<16)          @ - dtcm enable
	bic r4, #(1<<12)          @ - instruction cache disable
	bic r4, #(1<<2)           @ - data cache disable
	bic r4, #(1<<0)           @ - mpu disable
	mcr p15, 0, r4, c1, c0, 0 @ write control register

	@ Fixes mounting of SDMC
	ldr r0, =0x10000020
	mov r1, #0x340
	str r1, [r0]

	@ Clear BSS
	ldr r0, =__bss_start
	mov r1, #0
	ldr r2, =__bss_size
	bl memset

	@ Check if we're already executing from ITCM
	cmp pc, #ARM9MEM_BASE
	blo skip_relocate

	@ Copy entire binary to ITCM
	mov r0, #_start
	adr r1, _start
	ldr r2, =__loaded_size
	bl memcpy

	skip_relocate:
	mov r0, r9
	mov r1, r10
	ldr pc, =main

	die:
		b die

.ltorg
