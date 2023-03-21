#include "common.h"

.section .bootrom

#ifdef ARM9 // .bootrom linked at 0xFFFF0000

.org 0x0198; asm_func bfnWaitCycles
.org 0x01A4; asm_func bfnWFI
.org 0x03A4; asm_func bfnMemset32
.org 0x03F0; asm_func bfnMemcpy32
.org 0x056C; asm_func bfnMemset
.org 0x05E0; asm_func bfnMemcpy
.org 0x06EC; asm_func bfnEnterCriticalSection
.org 0x0700; asm_func bfnLeaveCriticalSection
.org 0x0798; asm_func bfnEnableDCache
.org 0x07B0; asm_func bfnDisableDCache
.org 0x07C8; asm_func bfnSetDCache
.org 0x07F0; asm_func bfnInvalidateDCache
.org 0x07FC; asm_func bfnWritebackDCache
.org 0x0830; asm_func bfnWritebackInvalidateDCache
.org 0x0868; asm_func bfnInvalidateDCacheRange
.org 0x0884; asm_func bfnWritebackDCacheRange
.org 0x08A8; asm_func bfnWritebackInvalidateDCacheRange
.org 0x096C; asm_func bfnDataSynchronizationBarrier
.org 0x0A5C; asm_func bfnEnableICache
.org 0x0A74; asm_func bfnDisableICache
.org 0x0A8C; asm_func bfnSetICache
.org 0x0AB4; asm_func bfnInvalidateICache
.org 0x0AC0; asm_func bfnInvalidateICacheRange
.org 0x0C38; asm_func bfnEnableMPU
.org 0x0C48; asm_func bfnDisableMPU
.org 0x0C58; asm_func bfnResetControlRegisters

#else // ARM11; .bootrom linked at 0x10000

.org 0x1288; asm_func bfnEnableDCache
.org 0x12A0; asm_func bfnDisableDCache
.org 0x12B8; asm_func bfnSetDCache
.org 0x12E0; asm_func bfnInvalidateDCache
.org 0x12EC; asm_func bfnWritebackDCache
.org 0x1320; asm_func bfnWritebackInvalidateDCache
.org 0x1358; asm_func bfnInvalidateDCacheRange
.org 0x1374; asm_func bfnWritebackDCacheRange
.org 0x1398; asm_func bfnWritebackInvalidateDCacheRange
.org 0x13C0; asm_func bfnDataSynchronizationBarrier
.org 0x13E8; asm_func bfnDataMemoryBarrier
.org 0x13F4; asm_func bfnEnableICache
.org 0x140C; asm_func bfnDisableICache
.org 0x1424; asm_func bfnSetICache
.org 0x144C; asm_func bfnInvalidateICache
.org 0x1458; asm_func bfnInvalidateICacheRange
.org 0x1490; asm_func bfnInstructionSynchronizationBarrier
.org 0x14E8; asm_func bfnInvalidateBranchTargetCache
.org 0x14F4; asm_func bfnInvalidateBranchTargetCacheRange
.org 0x16E4; asm_func bfnMemset32
.org 0x1730; asm_func bfnMemcpy32
.org 0x18AC; asm_func bfnMemset
.org 0x1920; asm_func bfnMemcpy
.org 0x1A38; asm_func bfnWaitCycles
.org 0x1AC4; asm_func bfnEnterCriticalSection
.org 0x1AD8; asm_func bfnLeaveCriticalSection
.org 0x28F8; .thumb_func; asm_func bfnVsnprintf

#endif