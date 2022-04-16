#pragma once

#include "common.h"

#define MAX_LOG_LEN 1024

bool pxiLogWrite(const void* data, unsigned int btw);
bool pxiLogWriteStr(const char* str);