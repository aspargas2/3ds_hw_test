#pragma once

#ifndef __ASSEMBLER__

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define vu8 volatile u8
#define vu16 volatile u16
#define vu32 volatile u32
#define vu64 volatile u64

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

#endif //__ASSEMBLER__

/* 3DS memory layout */
#define ARM9MEM_BASE  (0x08000000)
#define ARM9MEM_SIZE  (0x00100000)
#define VRAM_BASE     (0x18000000)
#define VRAM_SIZE     (0x00600000)
#define DSPMEM_BASE   (0x1FF00000)
#define DSPMEM_SIZE   (0x00080000)
#define AXIWRAM_BASE  (0x1FF80000)
#define AXIWRAM_SIZE  (0x00080000)
#define FCRAM_BASE    (0x20000000)
#define FCRAM_SIZE    (0x08000000)
