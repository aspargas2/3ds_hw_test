#include "debug.h"
#include "pxi/log.h"

#include <arm.h>
#include <bfn.h>
#include <console.h>

#include <stdarg.h>

#define PREFIX_LEN (sizeof(ARM11_DEBUG_PREFIX) - 1)

static char strBuf[sizeof(ARM11_DEBUG_PREFIX) + DEBUG_MAX_LEN] = ARM11_DEBUG_PREFIX;

void logDebugPrintf(bool logOnly, const char* format, ...) {
	va_list args;
	va_start(args, format);
	int n = bfnVsnprintf(strBuf + PREFIX_LEN, DEBUG_MAX_LEN, format, args);
	va_end(args);

	if (n == 0)
		return;

	if (!logOnly) {
		if ((u32) n != consoleNPrint(strBuf + PREFIX_LEN, n))
			ARM_BKPT();
	}

	pxiLogWrite(strBuf, PREFIX_LEN + n);
}