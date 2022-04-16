#pragma once

#include "common.h"
#include "screen.h"
#include "font.h"

#define CON_COLOR_BG   COLOR_BLACK
#define CON_COLOR_FONT COLOR_WHITE

#ifdef ARM9
#define CON_START_Y 0
#define CON_START_X 0
#define CON_END_Y  (SCREEN_HEIGHT)
#define CON_END_X  (MAIN_SCREEN_WIDTH - 2)
#else
#define CON_START_Y (5 + CHR_HEIGHT)
#define CON_START_X 4
#define CON_END_Y  (SCREEN_HEIGHT - 5)
#define CON_END_X  (MAIN_SCREEN_WIDTH - 6)
#endif

#define CON_HEIGHT (CON_END_Y - CON_START_Y)
#define CON_WIDTH (CON_END_X - CON_START_X)
#define CON_STEP_Y CHR_HEIGHT
#define CON_STEP_X CHR_WIDTH
#define CON_N_CHARS_Y (CON_HEIGHT / CON_STEP_Y)
#define CON_N_CHARS_X (CON_WIDTH / CON_STEP_X)

void fillRect(u16* screen, int x, int y, int width, int height, u16 color);
void drawCharacter(u16* screen, char character, int x, int y, u16 color);

void consoleClear();
u32 consoleNPrintColor(const char* str, u16 color, u32 n);

static inline u32 consolePrintColor(const char* str, u16 color) {
	return consoleNPrintColor(str, color, -1);
}

static inline u32 consoleNPrint(const char* str, u32 n) {
	return consoleNPrintColor(str, CON_COLOR_FONT, n);
}

static inline u32 consolePrint(const char* str) {
	return consolePrintColor(str, CON_COLOR_FONT);
}
