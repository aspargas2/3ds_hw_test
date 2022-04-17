#include <arm.h>
#include "common.h"

.section .text.start
.arm

.macro TRAP_ENTRY xrq
	msr cpsr_f, #(\xrq << 29)
	b xrqMain
.endm

xrqVectorTable:
	b _start @ reset, which will never happen
	b xrqUndefined
	b xrqSVC
	b xrqPrefetchAbort
	b xrqDataAbort
	b . @ ignore the reserved exception
	b xrqIRQ
	b xrqFIQ
xrqVectorTableEnd:

@xrqReset:
@	TRAP_ENTRY 0

xrqUndefined:
	TRAP_ENTRY 1

xrqSVC:
	TRAP_ENTRY 2

xrqPrefetchAbort:
	TRAP_ENTRY 3

xrqDataAbort:
	sub lr, lr, #4
	TRAP_ENTRY 4

xrqFIQ:
	TRAP_ENTRY 7

xrqMain:
	sub lr, lr, #4

	ldr sp, =(xrqStackTop - 32*4)
	stmia sp, {r0-r7}

	mrs r1, cpsr
	orr r0, r1, #SR_NOINT
	lsr r0, r1, #29

	mrs r2, spsr
	str lr, [sp, #15*4]
	str r2, [sp, #16*4]

	ands r2, r2, #SR_PMODE_MASK
	orreq r2, r2, #SR_SYS_MODE
	orr r2, r2, #(0x10 | SR_NOINT)

	add r3, sp, #8*4
	msr cpsr_c, r2
	stmia r3!, {r8-r14}
	msr cpsr_c, r1

	mov r1, sp
	bl doException


xrqIRQ:
	sub lr, lr, #4
	stmfd sp!, {r0-r3, r12, lr}
@mov r0, lr
	bl irqHandlerMain

	ldmfd sp!, {r0-r3, r12, pc}^


asm_func _start
	mov r9, r0  @ argc
	mov r10, r1 @ argv

	@ Setup IRQ mode stack
	msr cpsr_c, #(SR_IRQ_MODE | SR_NOINT)
	ldr sp, =xrqStackTop

	@ Switch to system mode, disable interrupts
	msr cpsr_c, #(SR_SYS_MODE | SR_NOINT)

	@ Change the stack pointer to the end of DTCM
	ldr sp, =(0xFFF00000 + 0x4000)

	@ Configure TCMs
	ldr r0, =0xFFF0000A
	ldr r1, =0x00000024
	mcr p15, 0, r0, c9, c1, 0 @ DTCM
	mcr p15, 0, r1, c9, c1, 1 @ ITCM

	@ Disable caches / mpu, enable TCMs, set exception vector location
	mrc p15, 0, r4, c1, c0, 0 @ read control register
	orr r4, #(CR_ITCM | CR_DTCM)
	bic r4, #(CR_ICACHE | CR_ALT_VECTORS)
	bic r4, #(CR_MPU | CR_DCACHE)
	mcr p15, 0, r4, c1, c0, 0 @ write control register

	@ Check if we're already executing from ITCM
	cmp pc, #ARM9MEM_BASE
	ldrlo pc, =relocate_done

	@ Copy entire binary to ITCM
	mov r0, #xrqVectorTable
	adr r1, xrqVectorTable
	ldr r2, =__loaded_size
	ldr lr, =relocate_done
	b memcpy

	relocate_done:

	@ Fixes mounting of SDMC
	ldr r0, =0x10000020
	mov r1, #0x340
	str r1, [r0]

	@ Clear BSS
	ldr r0, =__bss_start
	mov r1, #0
	ldr r2, =__bss_size
	bl memset

	mov r0, r9
	mov r1, r10
	blx main

	b .

.ltorg

.section .bss
.align 4
.global xrqStackBottom
xrqStackBottom:
	.space 512 @ hopefully this is enough room for the abort stack
.global xrqStackTop
xrqStackTop: