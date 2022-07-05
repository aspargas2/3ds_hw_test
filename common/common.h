#pragma once

#ifndef ARM9
#ifndef ARM11
#error "Unknown processor"
#endif
#endif

#ifdef ARM9
#ifdef ARM11
#error "Something is very wrong"
#endif
#endif

#define BIT(x) (1 << (x))

#ifdef __ASSEMBLER__

.macro asm_func name
.global \name
.type \name, %function
\name:
.endm

#else

#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define vu8 volatile u8
#define vu16 volatile u16
#define vu32 volatile u32
#define vu64 volatile u64

#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t

#define ALIGN(n)        __attribute__((aligned(n)))
#define PACKED_ALIGN(n) __attribute__((packed, aligned(n)))
#define PACKED_STRUCT   PACKED_ALIGN(4)

#define asm_v asm __volatile__

#define PRINTF_ARGS(n) __attribute__((format (printf, (n), (n) + 1)))

#define max(a,b) \
	(((a) > (b)) ? (a) : (b))
#define min(a,b) \
	(((a) < (b)) ? (a) : (b))
#define getbe16(d) \
	(((d)[0]<<8) | (d)[1])
#define getbe32(d) \
	((((u32) getbe16(d))<<16) | ((u32) getbe16(d+2)))
#define getbe64(d) \
	((((u64) getbe32(d))<<32) | ((u64) getbe32(d+4)))
#define getle16(d) (*((u16*) (d)))
#define getle32(d) (*((u32*) (d)))
#define getle64(d) (*((u64*) (d)))
#define align(v,a) \
	(((v) % (a)) ? ((v) + (a) - ((v) % (a))) : (v))
#define countof(x) \
	(sizeof(x) / sizeof(*(x)))

#define IS_O3DS (((*(vu16*) 0x10140FFC) & 2) == 0)

#endif //__ASSEMBLER__

// https://www.3dbrew.org/wiki/Memory_layout
#define ARM9MEM_BASE     (0x08000000)
#define ARM9MEM_SIZE     (0x00100000)
#define NEW_ARM9MEM_BASE (0x08100000)
#define NEW_ARM9MEM_SIZE (0x00080000)
#define VRAM_BASE        (0x18000000)
#define VRAM_SIZE        (0x00600000)
#define DSPMEM_BASE      (0x1FF00000)
#define DSPMEM_SIZE      (0x00080000)
#define AXIWRAM_BASE     (0x1FF80000)
#define AXIWRAM_SIZE     (0x00080000)
#define FCRAM_BASE       (0x20000000)
#define FCRAM_SIZE       (0x08000000)
#define NEW_FCRAM_BASE   (0x28000000)
#define NEW_FCRAM_SIZE   (0x08000000)
