#pragma once

#include "common.h"
#include "bfn.h"

#include <string.h>

#define memset(dest, c, num) ((void) bfnMemset(dest, c, num))
#define memcpy(dest, src, num) ((void) bfnMemcpy(src, dest, num))

int clz(u32 x);

void utoahex(u32 x, char* buf);
void utoadec(u32 x, char* buf);
void itoadec(s32 x, char* buf);