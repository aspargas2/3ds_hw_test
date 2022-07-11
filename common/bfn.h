#pragma once

#include <stdarg.h>

/*
 Declarations of preexisting BootROM functions; addresses are in bfn.S
 All of these functions should follow the standard AAPCS convention
*/

#ifdef ARM9

// mcr instruction to wait for interrupt
void bfnWFI(void);

void bfnEnableMPU(void);
void bfnDisableMPU(void);

// sets CR0 to its reset state (MPU & caches disabled, high vectors, TCMs enabled)
// invalidates both instruction and data caches (without previously writing back!!)
void bfnResetControlRegisters(void);

#else

void bfnInstructionSynchronizationBarrier(void);

void bfnDataMemoryBarrier(void);

void bfnInvalidateBranchTargetCache(void);
void bfnInvalidateBranchTargetCacheRange(void* start, u32 len);

// seems to be perfectly compatible with regular library vsnprintf
int bfnVsnprintf(char* s, int n, const char* format, va_list arg);

#endif

// delays execution time by cycles
void bfnWaitCycles(u32 cycles);

u32* bfnMemset32(u32 val, u32* dest, u32 count);
u32* bfnMemcpy32(const u32* src, u32* dest, u32 count);

void bfnMemset(void* dest, u8 val, u32 count);
void bfnMemcpy(const void* src, void* dest, u32 count);

// disables interrupts and returns the old irq state
u32 bfnEnterCriticalSection(void);
// restores the old irq state
void bfnLeaveCriticalSection(u32 irqState);

// enables the data cache and returns the old dcache bit
bool bfnEnableDCache(void);
// disables the data cache and returns the old dcache bit
bool bfnDisableDCache(void);
// sets the data cache bit and returns the old one
bool bfnSetDCache(bool enable);

// invalidates all data cache entries
void bfnInvalidateDCache(void);
// writes back all data cache entries
void bfnWritebackDCache(void);
// writes back and invalidates all data cache entries
void bfnWritebackInvalidateDCache(void);

void bfnInvalidateDCacheRange(void* start, u32 len);
void bfnWritebackDCacheRange(void* start, u32 len);
void bfnWritebackInvalidateDCacheRange(void* start, u32 len);

void bfnDataSynchronizationBarrier(void);

bool bfnEnableICache(void);
bool bfnDisableICache(void);
bool bfnSetICache(bool enable);

// also invalidates the branch target cache on ARM11
void bfnInvalidateICache(void);

// WARNING: DOES NOT INVALIDATE THE BRANCH TARGET CACHE ON ARM11
// NEEDS TO INVALIDATE IT AND FLUSH THE PREFETCH BUFFER
void bfnInvalidateICacheRange(void* start, u32 len);
