#pragma once

#include "common.h"

#define ARM11_DEBUG_PREFIX "[ARM11] "
#define DEBUG_MAX_LEN 512

#define debugPrint(str) debugPrintf("%s", str)
#define debugPrintf(format, args...) logDebugPrintf(false, format, args)

// Uses bootrom vsnprintf
void PRINTF_ARGS(2) logDebugPrintf(bool logOnly, const char* format, ...);