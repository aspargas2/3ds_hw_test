#pragma once

#include "common.h"

#define LOG_FILE "hwtest.log"

bool InitLog();
void DeinitLog();

bool LogWrite(const char* text);
