#pragma once

#include "common.h"

#define LOG_FILE "hwtest.log"

bool logReady();
bool initLog();
void deinitLog();

bool logWrite(const void* data, unsigned int btw);
bool logWriteStr(const char* str);