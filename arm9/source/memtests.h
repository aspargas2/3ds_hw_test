#pragma once

// Must have enough space in BSS to store 3*MAX_ERRORS*sizeof(u32)
#define MAX_ERRORS 128

#ifndef __ASSEMBLER__
#include "common.h"

u32 memtest_walking_ones(void* start_addr, void* end_addr);
u32 memtest_own_address(void* start_addr, void* end_addr);
u32 memtest_moving_inversions_ones_zeros(void* start_addr, void* end_addr);
u32 memtest_moving_inversions_8bit_walking(void* start_addr, void* end_addr);
u32 memtest_bit_fade(void* start_addr, void* end_addr);

typedef struct {
	u32 (*func)(void*, void*);
	const char* name;
} testInfo;

typedef struct {
	u32 start_addr;
	u32 size;
	const char* name;
} memoryRegion;

#define N_TESTS (sizeof(tests) / sizeof(testInfo))
const testInfo tests[] = {
	{ memtest_walking_ones,                   "Walking ones address test" },
	{ memtest_own_address,                    "Own address test" },
	{ memtest_moving_inversions_ones_zeros,   "Moving inversions test, ones & zeros" },
	{ memtest_moving_inversions_8bit_walking, "Moving inversions test, 8 bit patterns" },
	{ memtest_bit_fade,                       "Bit fade test" },
};

#define N_REGIONS (sizeof(regions) / sizeof(memoryRegion))
const memoryRegion regions[] = {
	{ ARM9MEM_BASE, ARM9MEM_SIZE, "Arm9 Internal Memory" },
	//{ VRAM_BASE,    VRAM_SIZE,    "VRAM" },
	{ AXIWRAM_BASE, AXIWRAM_SIZE, "AXI WRAM" },
	//{ DSPMEM_BASE,  DSPMEM_SIZE,  "DSP Memory" },
	{ FCRAM_BASE,   FCRAM_SIZE,   "FCRAM" },
};

extern u32 errors_buffer[MAX_ERRORS * 3];
#endif //__ASSEMBLER__
