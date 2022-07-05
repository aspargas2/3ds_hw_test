#pragma once

#include "common.h"

// Must have enough space in BSS to store MAX_ERRORS * ERROR_STRUCT_SIZE (struct defined below)
#define MAX_ERRORS 128
#define ERROR_STRUCT_SIZE 12

// 3 minutes
#define BIT_FADE_DELAY (3ULL * 60ULL * (67027964ULL / 1024ULL))

#ifndef __ASSEMBLER__

enum {
	MEMTEST_REGION_ARM9MEM = 0,
	MEMTEST_REGION_NEW_ARM9MEM,
	MEMTEST_REGION_DSPMEM,
	MEMTEST_REGION_AXIWRAM,
	MEMTEST_REGION_FCRAM,
	MEMTEST_REGION_NEW_FCRAM,

	N_MEMTEST_REGIONS
};

enum {
	MEMTEST_WALKING_ONES = 0,
	MEMTEST_OWN_ADDRESS,
	MEMTEST_MOV_INV_1_0,
	MEMTEST_MOV_INV_8_BIT,
	MEMTEST_BIT_FADE,

	N_MEMTESTS
};

// Bit fade must be done last for each region so other regions may begin testing while waiting
_Static_assert(MEMTEST_BIT_FADE + 1 == N_MEMTESTS);

typedef struct {
	u32 base;
	u32 size;
	const char* name;
} MemestRegionInfo;

typedef struct {
	u32 (*func)(u32, u32);
	const char* name;
} MemtestInfo;

extern const MemestRegionInfo memtestRegions[];
extern const char* memtestNames[];

#define ALL_MEMTEST_REGIONS (BIT(N_MEMTEST_REGIONS) - 1)
#define ALL_MEMTESTS (BIT(N_MEMTESTS) - 1)

#define MEMTEST_REGIONS_NEW3DS_ONLY \
	(BIT(MEMTEST_REGION_NEW_ARM9MEM) | BIT(MEMTEST_REGION_NEW_FCRAM))
#define MEMTEST_REGIONS_ARM9_ONLY \
	(BIT(MEMTEST_REGION_ARM9MEM) | BIT(MEMTEST_REGION_NEW_ARM9MEM))

typedef struct {
	u32 address;
	u32 original;
	u32 observed;
} PACKED_STRUCT MemtestError;
_Static_assert(ERROR_STRUCT_SIZE == sizeof(MemtestError));

// Returns total errors detected
u64 testMemory(u32 regions, u32 tests);

#endif //__ASSEMBLER__
