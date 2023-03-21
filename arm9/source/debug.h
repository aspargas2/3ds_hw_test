#pragma once

#include "common.h"

#define ARM9_DEBUG_PREFIX "[ARM9] "

#define debugPrintf(format, args...) logDebugPrintf(false, format, args)

void debugPrint(const char* str);

// Hand-rolled minimal printf
void PRINTF_ARGS(2) logDebugPrintf(bool logOnly, const char* format, ...);