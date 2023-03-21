#pragma once

#include <vram.h>

#define BYTES_PER_PIXEL 2
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH_TOP 400
#define SCREEN_WIDTH_BOT 320

_Static_assert(TOP_VRAM == SCREEN_HEIGHT * SCREEN_WIDTH_TOP * BYTES_PER_PIXEL);

#define TOP_SCREEN ((u16*) VRAM_TOP_LA)
#define BOT_SCREEN ((u16*) VRAM_BOT_A)

#ifdef ARM9
#define MAIN_SCREEN BOT_SCREEN
#define MAIN_SCREEN_WIDTH SCREEN_WIDTH_BOT
#else
#define MAIN_SCREEN TOP_SCREEN
#define MAIN_SCREEN_WIDTH SCREEN_WIDTH_TOP
#endif

#define RGB(r,g,b) ((u16)(r>>3<<11|g>>2<<5|b>>3)) // converts from 888 to 565

#define COLOR_BLACK         RGB(0x00, 0x00, 0x00)
#define COLOR_WHITE         RGB(0xFF, 0xFF, 0xFF)
#define COLOR_RED           RGB(0xFF, 0x00, 0x00)
#define COLOR_GREEN         RGB(0x00, 0xFF, 0x00)
#define COLOR_BLUE          RGB(0x00, 0x00, 0xFF)
#define COLOR_LIGHT_BLUE    RGB(0x00, 0xA0, 0xE8)
#define COLOR_CYAN          RGB(0x00, 0xFF, 0xFF)
#define COLOR_MAGENTA       RGB(0xFF, 0x00, 0xFF)
#define COLOR_YELLOW        RGB(0xFF, 0xFF, 0x00)
#define COLOR_GREY          RGB(0x77, 0x77, 0x77)
#define COLOR_LIGHT_GREY    RGB(0xBB, 0xBB, 0xBB)
//#define COLOR_TRANSPARENT   RGB(0xFF, 0x00, 0xEF) // otherwise known as 'super fuchsia'