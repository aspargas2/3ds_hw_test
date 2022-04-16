#pragma once

#include <console.h>

u32 drawStringColor(u16* screen, const char* str, u32 x, u32 y, u16 color);
//u32 DrawString(u16* screen, const char* str, u32 x, u32 y);

u32 drawStringCenteredColor(u16* screen, const char* str, u32 x, u32 y, u16 color);

void drawProgressBar(u16* screen, u32 x, u32 y, u32 width, u32 height, u32 widthFilled, u16 fillColor, u16 unfillColor);