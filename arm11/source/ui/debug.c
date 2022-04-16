#include "debug.h"
#include "pxi/log.h"

#include <bfn.h>
#include <console.h>

#include <stdarg.h>

static char strBuf[DEBUG_MAX_LEN + sizeof(ARM11_DEBUG_PREFIX)] = ARM11_DEBUG_PREFIX;

void logDebugPrintf(bool logOnly, const char* format, ...) {
	va_list args;
	va_start(args, format);

	// TODO: refactor bootrom function definitions so an ugly funcpointer cast isn't needed here
	int n = ((int(*)(char*, int, const char*, va_list)) BFN_VSNPRINTF)(strBuf + sizeof(ARM11_DEBUG_PREFIX) - 1, 512, format, args);

	va_end(args);

	if (n == 0)
		return;
	n = logOnly ? strnlen(strBuf + sizeof(ARM11_DEBUG_PREFIX) - 1, n) :
			consoleNPrint(strBuf + sizeof(ARM11_DEBUG_PREFIX) - 1, n);
	if (n == 0)
		return;

	pxiLogWrite(strBuf, n + sizeof(ARM11_DEBUG_PREFIX) - 1);
}