#include "exception.h"
#include "ui/debug.h"

#include <pxi.h>
#include <i2c.h>
#include <console.h>

#include <stdlib.h>

void doException(u32 type, u32* regs) {
	I2C_writeReg(I2C_DEV_CTR_MCU, 0x29, 4);

	consoleClear();
	debugPrintf("ARM11 ded %ld\n\n", type);

	for (int i = 0; i < 20; i += 2) {
		debugPrintf("%02d %08lX   %02d %08lX\n", i, regs[i], i+1, regs[i+1]);
	}

	debugPrint("\nStack:\n");
	u32* sp = (u32*) regs[13];

	for (int i = 0; i < 32; i += 4) {
		debugPrintf("%08lX %08lX %08lX %08lX\n", sp[i], sp[i+1], sp[i+2], sp[i+3]);
	}

	PXI_Send(PXICMD_LEGACY_BOOT);

	while (1);
}