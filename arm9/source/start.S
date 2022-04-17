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
	b (xrqUndefined     + 0x8000)
	b (xrqSVC           + 0x8000)
	b (xrqPrefetchAbort + 0x8000)
	b (xrqDataAbort     + 0x8000)
	b . @ ignore the reserved exception
	b (xrqIRQ           + 0x8000)
	b (xrqFIQ           + 0x8000)
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
	mrc p15, 0, r0, c1, c0, 0 @ read control register
	orr r0, #(CR_ITCM | CR_DTCM)
	bic r0, #(CR_ICACHE | CR_ALT_VECTORS)
	bic r0, #(CR_MPU | CR_DCACHE)
	mcr p15, 0, r0, c1, c0, 0 @ write control register

	@ Check if we're already executing from ITCM
	cmp pc, #ARM9MEM_BASE
	ldrlo pc, =relocate_done

	@ Copy entire binary to ITCM
	adr r0, xrqVectorTable
	mov r1, #xrqVectorTable
	ldr r2, =__loaded_size
	ldr lr, =relocate_done
	ldr pc, =BFN_CPUCPY

	relocate_done:

	adr lr, mpu_regions
	ldm lr, {r0-r8}
	mcr p15, 0, r0, c6, c0, 0
	mcr p15, 0, r1, c6, c1, 0
	mcr p15, 0, r2, c6, c2, 0
	mcr p15, 0, r3, c6, c3, 0
	mcr p15, 0, r4, c6, c4, 0
	mcr p15, 0, r5, c6, c5, 0
	mcr p15, 0, r6, c6, c6, 0

	mcr p15, 0, r7, c5, c0, 2
	mcr p15, 0, r8, c5, c0, 3

	mov r0, #0
	mcr p15, 0, r0, c6, c7, 0
	mcr p15, 0, r0, c3, c0, 0 @ Write bufferable
	mcr p15, 0, r0, c2, c0, 0 @ Data cacheable
	mcr p15, 0, r0, c2, c0, 1 @ Inst cacheable

	bl BFN_ENABLE_MPU

	@ Fixes mounting of SDMC
	ldr r0, =0x10000020
	mov r1, #0x340
	str r1, [r0]

	@ Clear BSS
	mov r0, #0
	ldr r1, =__bss_start
	ldr r2, =__bss_size
	bl BFN_CPUSET

	mov r0, r9
	mov r1, r10
	blx main

	b .

.ltorg

#define REGION_4KiB   (0b01011)
#define REGION_8KiB   (0b01100)
#define REGION_16KiB  (0b01101)
#define REGION_32KiB  (0b01110)
#define REGION_64KiB  (0b01111)
#define REGION_128KiB (0b10000)
#define REGION_256KiB (0b10001)
#define REGION_512KiB (0b10010)
#define REGION_1MiB   (0b10011)
#define REGION_2MiB   (0b10100)
#define REGION_4MiB   (0b10101)
#define REGION_8MiB   (0b10110)
#define REGION_16MiB  (0b10111)
#define REGION_32MiB  (0b11000)
#define REGION_64MiB  (0b11001)
#define REGION_128MiB (0b11010)
#define REGION_256MiB (0b11011)
#define REGION_512MiB (0b11100)
#define REGION_1GiB   (0b11101)
#define REGION_2GiB   (0b11110)
#define REGION_4GiB   (0b11111)
#define MAKE_REGION(adr, size) ((adr) | ((size)<<1) | 1)

mpu_regions:
@ Execute-only region for exception table, so null derefs will still abort
.word MAKE_REGION(0,          REGION_4KiB)
@ Next ITCM mirror for main excecution
.word MAKE_REGION(0x8000,     REGION_32KiB)
@ Bootrom
.word MAKE_REGION(0xFFFF0000, REGION_64KiB)
@ DTCM
.word MAKE_REGION(0xFFF00000, REGION_16KiB)
@ IO mem + VRAM + DSPMEM + AXIWRAM
.word MAKE_REGION(0x10000000, REGION_256MiB)
@ FCRAM
.word MAKE_REGION(0x20000000, REGION_256MiB)
@ ARM9 Memory
.word MAKE_REGION(0x08000000, REGION_2MiB)
mpu_permissions:
.word 0x01111510
.word 0x00000555

.section .bss
.align 4
.global xrqStackBottom
xrqStackBottom:
	.space 512 @ hopefully this is enough room for the abort stack
.global xrqStackTop
xrqStackTop: