#include "console.h"
#include "font.h"

#include <vram.h>
#include <arm.h>

#include <string.h>

static u32 consoleXPos = CON_START_X;
static u32 consoleYPos = CON_START_Y;

void fillRect(u16* screen, int x, int y, int width, int height, u16 color) {
	for (int xx = 1; xx <= width; xx++)
		for (int yy = 1; yy <= height; yy++)
			screen[((x + xx) * SCREEN_HEIGHT) - y - yy] = color;
}

static void shiftRect(u16* screen, int x, int y, int width, int height, int shift, u16 bgcolor) {
	for (int xx = 1; xx <= width; xx++)
		for (int yy = 1; yy <= height - shift; yy++)
			screen[((x + xx) * SCREEN_HEIGHT) - y - yy] = screen[((x + xx) * SCREEN_HEIGHT) - y - yy - shift];

	fillRect(screen, x, y + height - shift, width, shift, bgcolor);
}

void drawCharacter(u16* screen, char character, int x, int y, u16 color) {
	// char - 0x20 because non-essential parts of the font are trimmed off
	u32* charData = (u32*) (font + character - 0x20);

	for (int xx = 0; xx < CHR_WIDTH; xx++) {
		int xDisplacement = (x + xx) * SCREEN_HEIGHT;
		for (int yy = 0; yy < CHR_HEIGHT; yy++) {
			int yDisplacement = SCREEN_HEIGHT - (y + yy) - 1;
			u16 *screenPos = screen + xDisplacement + yDisplacement;
			int bit = (yy * CHR_WIDTH) + xx;
			if (charData[bit / 32] & (1 << (bit % 32)))
				*screenPos = color;
		}
	}
}

void consoleClear() {
	consoleXPos = CON_START_X;
	consoleYPos = CON_START_Y;

	fillRect(MAIN_SCREEN, CON_START_X, CON_START_Y, CON_WIDTH, CON_HEIGHT, CON_COLOR_BG);
}

u32 consoleNPrintColor(const char* str, u16 color, u32 n) {
	u32 i = 0;
	u32 irqState = ARM_EnterCritical();

	for (; str[i] != '\0' && i < n; i++) {
		if (str[i] == '\n' || consoleXPos >= CON_END_X) {
			consoleXPos = CON_START_X;
			consoleYPos += CON_STEP_Y;
			if (consoleYPos > (CON_END_Y - CON_STEP_Y)) {
				consoleYPos = (CON_END_Y - CON_STEP_Y);
				shiftRect(MAIN_SCREEN, CON_START_X, CON_START_Y, CON_WIDTH, CON_HEIGHT, CON_STEP_Y, CON_COLOR_BG);
			}
		}

		if (str[i] != '\n') {
			drawCharacter(MAIN_SCREEN, str[i], consoleXPos, consoleYPos, color);
			consoleXPos += CON_STEP_X;
		}
	}

	ARM_LeaveCritical(irqState);
	return i;
}
