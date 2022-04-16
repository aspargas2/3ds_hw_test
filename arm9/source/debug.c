#include "debug.h"
#include "console.h"

#include <arm.h>

#ifdef ARM9
#include <log.h>
#else
#include <ui/log.h>
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

static void debugNPrint(bool logOnly, const char* str, u32 n) {
	// Console code returns number of chars printed, so min(n, strlen(str))
	if (n == 0)
		return;
	n = logOnly ? strnlen(str, n) :
			consoleNPrint(str, n);
	if (n == 0)
		return;
	logWrite(str, n);
}

void debugPrint(const char* str) {
	if (*str == '\0')
		return;
	u32 irqState = ARM_EnterCritical();
	logWrite(ARM9_DEBUG_PREFIX, sizeof(ARM9_DEBUG_PREFIX) - 1);
	debugNPrint(false, str, -1);
	ARM_LeaveCritical(irqState);
}

void logDebugPrintf(bool logOnly, const char* format, ...) {
	static char strbuf[30];
	u32 i = 0;

	u32 irqState = ARM_EnterCritical();
	va_list args;
	va_start(args, format);

	logWrite(ARM9_DEBUG_PREFIX, sizeof(ARM9_DEBUG_PREFIX) - 1);

	for (;;i++) {
		char c = format[i];

		if (c == '\0') {
			debugNPrint(logOnly, format, i);
			break;
		}

		if (c == '%') {
			debugNPrint(logOnly, format, i);
			format += i;
			i = 0;
			int lcount = 0;

			codeswitch:
			switch (format[1]) {
				case '%':
					format++;
					continue;
				case 's':
					debugNPrint(logOnly, va_arg(args, char*), -1);
					format += 2;
					continue;
				case 'd':
					// TODO: make this work
					/*if (lcount > 1)
						illtoa(va_arg(args, s64), strbuf, 10);
					else*/
						itoa(va_arg(args, s32), strbuf, 10);
					debugNPrint(logOnly, strbuf, 30);
					format += 2;
					continue;
				case 'u':
					/*if (lcount > 1)
						ulltoa(va_arg(args, u64), strbuf, 10);
					else*/
						utoa(va_arg(args, u32), strbuf, 10);
					debugNPrint(logOnly, strbuf, 30);
					format += 2;
					continue;
				case 'x':
					/*if (lcount > 1)
						ulltoa(va_arg(args, u64), strbuf, 16);
					else*/
						utoa(va_arg(args, u32), strbuf, 16);
					debugNPrint(logOnly, strbuf, 30);
					format += 2;
					continue;
				case 'l':
					lcount++;
					format++;
					goto codeswitch;
				default:
					format -= lcount;
					continue;
			}
		}
	}

	va_end(args);
	ARM_LeaveCritical(irqState);
}