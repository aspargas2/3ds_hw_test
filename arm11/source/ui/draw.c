#include "draw.h"

u32 drawStringColor(u16* screen, const char* str, u32 x, u32 y, u16 color) {
	u32 count = 0;

	for (; *str != '\0'; x += CHR_WIDTH, str++, count++) {
		drawCharacter(screen, *str, x, y, color);
	}

	return count;
}

/*u32 DrawString(u16* screen, const char* str, u32 x, u32 y) {
	return DrawStringColor(screen, str, x, y, CON_COLOR_FONT);
}*/

u32 drawStringCenteredColor(u16* screen, const char* str, u32 x, u32 y, u16 color) {
	return drawStringColor(screen, str, x - (strlen(str) * CHR_WIDTH / 2), y, color);
}

void drawProgressBar(u16* screen, u32 x, u32 y, u32 width, u32 height, u32 widthFilled, u16 fillColor, u16 unfillColor) {
	fillRect(screen, x, y, widthFilled, height, fillColor);
	fillRect(screen, x + widthFilled, y, width - widthFilled, height, unfillColor);
}