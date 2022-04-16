#include "memtests.h"

#include "pxi.h"
#include "arm.h"

#ifdef ARM9
#include <debug.h>
#include <timer.h>
#else
#include <ui/debug.h>
#include <pxi/timer.h>

// TODO: use the mpcore timer stuff for this on ARM11
//       ... and find a nice way to not have totally different bit fade code on ARM9
#define getFullTimerTicks pxiGetTimerTicks

#endif //ARM9

#include <stdlib.h>

extern MemtestError errors_buffer[MAX_ERRORS];

u32 memtest_walking_ones(u32 start_addr, u32 end_addr);
u32 memtest_own_address(u32 start_addr, u32 end_addr);
u32 memtest_moving_inversions_ones_zeros(u32 start_addr, u32 end_addr);
u32 memtest_moving_inversions_8bit_walking(u32 start_addr, u32 end_addr);
u32 memtest_bit_fade_check(u32 start_addr, u32 end_addr, u32 pattern);

const MemestRegionInfo memtestRegions[] = {
	{ ARM9MEM_BASE,     ARM9MEM_SIZE,     "ARM9 Internal Memory" },
	{ NEW_ARM9MEM_BASE, NEW_ARM9MEM_SIZE, "New3DS Extended ARM9 Memory" },
	{ DSPMEM_BASE,      DSPMEM_SIZE,      "DSP Memory" },
	{ AXIWRAM_BASE,     AXIWRAM_SIZE,     "AXI Work RAM" },
	{ FCRAM_BASE,       FCRAM_SIZE,       "FCRAM" },
	{ NEW_FCRAM_BASE,   NEW_FCRAM_SIZE,   "New3DS Extended FCRAM" },
};

const MemtestInfo memtests[] = {
	{ memtest_walking_ones,                   "Walking Ones Test" },
	{ memtest_own_address,                    "Own Address Test" },
	{ memtest_moving_inversions_ones_zeros,   "Moving Inversions Test, Ones & Zeros" },
	{ memtest_moving_inversions_8bit_walking, "Moving inversions Test, 8 Bit Patterns" },
	{ NULL,                                   "Bit Fade Test" },
};

_Static_assert(N_MEMTEST_REGIONS == countof(memtestRegions));
_Static_assert(N_MEMTESTS == countof(memtests));

#ifndef ARM9
static u64 __attribute__((section(".shared"))) errorsFromArm9;
#endif

static u64 totalErrors = 0;
static u64 bitFadeTimers[N_MEMTEST_REGIONS] = { 0 };

static void logErrors(u32 nErrors) {
	for (u32 i = 0; i < min(nErrors, MAX_ERRORS); i++) {
		MemtestError* e = errors_buffer + i;
		logDebugPrintf(true, "%lx: %lx -> %lx\n", e->address, e->original, e->observed);
	}
}

static bool checkBitFade(u32 maxRegion) {
	bool ret = false;

	for (u32 regionIndex = 0; regionIndex < maxRegion; regionIndex++) {
		u64 timer = bitFadeTimers[regionIndex];
		if (timer != 0) {
			if (timer < getFullTimerTicks()) {
				const u32 startAddr = memtestRegions[regionIndex].base,
				          size      = memtestRegions[regionIndex].size,
				          endAddr   = startAddr + size,
				          pattern   = (timer & 1) ? 0 : ~0;

				debugPrintf("Processing bit fade pattern %lx for %s...\n", pattern, memtestRegions[regionIndex].name);

				u32 nErrors = memtest_bit_fade_check(startAddr, endAddr, pattern);

				if (nErrors) {
					logErrors(nErrors);
					totalErrors += nErrors;
				}

				if (pattern == 0) {
					memset((void*) startAddr, ~0, size);
					bitFadeTimers[regionIndex] = (getFullTimerTicks() + BIT_FADE_DELAY) & ~1ULL;
					ret = true;
				} else {
					bitFadeTimers[regionIndex] = 0;
				}
			} else {
				ret = true;
			}
		}
	}

	return ret;
}

u64 testMemory(u32 regions, u32 tests) {
	totalErrors = 0;

	#ifndef ARM9
	bool watingForArm9 = false;

	// If we have any ARM9-only regions, tell ARM9 to test them
	if (regions & MEMTEST_REGIONS_ARM9_ONLY) {
		errorsFromArm9 = ~0ULL;

		PXI_Send((3 << 16) | PXICMD_RUN_MEMTEST);
		PXI_Send((u32) &errorsFromArm9);
		PXI_Send(regions & MEMTEST_REGIONS_ARM9_ONLY);
		PXI_Send(tests);

		watingForArm9 = true;
		regions &= ~MEMTEST_REGIONS_ARM9_ONLY;
	}
	#endif

	for (u32 regionIndex = 0; regionIndex < N_MEMTEST_REGIONS; regionIndex++) {
		if (!(regions & BIT(regionIndex)))
			continue;

		if (IS_O3DS && (BIT(regionIndex) & MEMTEST_REGIONS_NEW3DS_ONLY)) {
			// TODO UI
			debugPrintf("Skipping %s\n", memtestRegions[regionIndex].name);

			continue;
		}

		// TODO: proper UI stuff
		debugPrintf("Testing %s ...\n", memtestRegions[regionIndex].name);

		const u32 startAddr = memtestRegions[regionIndex].base,
		          size      = memtestRegions[regionIndex].size,
		          endAddr   = startAddr + size;

		for (u32 testIndex = 0; testIndex < N_MEMTESTS; testIndex++) {
			if (!(tests & BIT(testIndex)))
				continue;

			// Check bit fade timers on previous regions
			checkBitFade(regionIndex);

			if (testIndex == MEMTEST_BIT_FADE) {
				debugPrintf("Setting up %s...\n", memtests[MEMTEST_BIT_FADE].name);
				memset((void*) startAddr, 0, size);
				bitFadeTimers[regionIndex] = (getFullTimerTicks() + BIT_FADE_DELAY) | 1ULL;
				break;
			}

			// TODO UI
			debugPrintf("Running %s...\n", memtests[testIndex].name);

			// TODO split this up to expose pattern etc
			u32 nErrors = memtests[testIndex].func(startAddr, endAddr);

			if (nErrors) {
				logErrors(nErrors);
				totalErrors += nErrors;
			}
		}

		debugPrint("\n");
	}

	debugPrint("Waiting on remaining bit fades to finish ...\n");
	while (checkBitFade(N_MEMTEST_REGIONS))
		ARM_WFI();

	#ifndef ARM9
	if (watingForArm9) {
		debugPrint("Waiting on ARM9-side tests to finish ...\n");
		while (errorsFromArm9 == ~0ULL)
			ARM_WFI();

		totalErrors += errorsFromArm9;
		watingForArm9 = false;
	}
	#endif

	debugPrintf("Done. %lld total errors detected.\n", totalErrors);

	return totalErrors;
}