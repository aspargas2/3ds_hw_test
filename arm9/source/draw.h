// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common.h"

#define BYTES_PER_PIXEL 3
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH_TOP 400
#define SCREEN_WIDTH_BOT 320

#define RGB(r,g,b) (r<<24|b<<16|g<<8|r)

#define COLOR_BLACK         RGB(0x00, 0x00, 0x00)
#define COLOR_WHITE         RGB(0xFF, 0xFF, 0xFF)
#define COLOR_RED           RGB(0xFF, 0x00, 0x00)
#define COLOR_GREEN         RGB(0x00, 0xFF, 0x00)
#define COLOR_BLUE          RGB(0x00, 0x00, 0xFF)
#define COLOR_CYAN          RGB(0x00, 0xFF, 0xFF)
#define COLOR_MAGENTA       RGB(0xFF, 0x00, 0xFF)
#define COLOR_YELLOW        RGB(0xFF, 0xFF, 0x00)
#define COLOR_GREY          RGB(0x77, 0x77, 0x77)
#define COLOR_TRANSPARENT   RGB(0xFF, 0x00, 0xEF) // otherwise known as 'super fuchsia'

#define STD_COLOR_BG   COLOR_BLACK
#define STD_COLOR_FONT COLOR_WHITE

#define DBG_COLOR_BG   COLOR_BLACK
#define DBG_COLOR_FONT COLOR_WHITE

#define CHR_HEIGHT 8
#define CHR_WIDTH  8

#define DBG_STEP_Y (CHR_HEIGHT + 2)
#define DBG_STEP_X CHR_WIDTH
#define DBG_END_Y  SCREEN_HEIGHT
#define DBG_END_X  SCREEN_WIDTH_TOP

#define DBG_N_CHARS_Y (DBG_END_Y / DBG_STEP_Y)
#define DBG_N_CHARS_X ((DBG_END_X / DBG_STEP_X) + 1)

void InitScreenFbs(int argc, char *argv[]);
void ClearScreen(unsigned char *screen, int width, int color);
void ClearScreenFull(bool clear_top, bool clear_bottom);

void DrawCharacter(unsigned char *screen, int character, int x, int y, int color, int bgcolor);
//void DrawString(unsigned char *screen, const char *str, int x, int y, int color, int bgcolor);

void DebugClear();
void DebugColor(u32 color, const char *message);
void Debug(const char *message);

void ShowProgress(u64 current, u64 total);
