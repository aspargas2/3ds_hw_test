#include "exception.h"

#include <i2c.h>
#include <debug.h>
#include <console.h>

#include "log.h"

void doException(u32 type, u32* regs) {
	I2C_writeReg(I2C_DEV_CTR_MCU, 0x29, 4);

	consoleClear();
	debugPrintf("\nARM9 ded %ld\n\n", type);

	for (int i = 0; i < 16; i += 2) {
		debugPrintf("%d %lx   %d %lx\n", i, regs[i], i + 1, regs[i + 1]);
	}

	debugPrintf("CPSR %lx\n\nStack:\n", regs[16]);

	u32* sp = (u32*) regs[13];

	for (int i = 0; i < 16; i += 4) {
		debugPrintf("%lx %lx %lx %lx\n", sp[i], sp[i+1], sp[i+2], sp[i+3]);
	}

	deinitLog();

	while (1);
}