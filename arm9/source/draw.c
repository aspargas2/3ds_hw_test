// Copyright 2013 Normmatt
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <string.h>

#include "font.h"
#include "draw.h"

static u8 *top_screen0 = NULL;
static u8 *top_screen1 = NULL;
static u8 *bot_screen0 = NULL;
static u8 *bot_screen1 = NULL;

static u32 debugxpos = 0;
static u32 debugypos = 0;

void InitScreenFbs(int argc, char *argv[])
{
	if (argc >= 2) {
		/* newer entrypoints */
		struct {
			u8 *top_left;
			u8 *top_right;
			u8 *bottom;
		} *fb = (void *)argv[1];
		top_screen0 = fb[0].top_left;
		top_screen1 = fb[1].top_left;
		bot_screen0 = fb[0].bottom;
		bot_screen1 = fb[1].bottom;
	} else {
		/* outdated entrypoints */
		top_screen0 = (u8 *)(*(u32 *)0x23FFFE00);
		top_screen1 = (u8 *)(*(u32 *)0x23FFFE00);
		bot_screen0 = (u8 *)(*(u32 *)0x23FFFE08);
		bot_screen1 = (u8 *)(*(u32 *)0x23FFFE08);
	}
}

void ClearScreen(u8 *screen, int width, int color)
{
	if (color == COLOR_TRANSPARENT)
		color = COLOR_BLACK;

	for (int i = 0; i < (width * SCREEN_HEIGHT); i++) {
		*(screen++) = color >> 16;      // B
		*(screen++) = color >> 8;       // G
		*(screen++) = color & 0xFF;     // R
	}
}

void ClearScreenFull(bool clear_top, bool clear_bottom)
{
	if (clear_top) {
		ClearScreen(top_screen0, SCREEN_WIDTH_TOP, STD_COLOR_BG);
		ClearScreen(top_screen1, SCREEN_WIDTH_TOP, STD_COLOR_BG);
	}
	if (clear_bottom) {
		ClearScreen(bot_screen0, SCREEN_WIDTH_BOT, STD_COLOR_BG);
		ClearScreen(bot_screen1, SCREEN_WIDTH_BOT, STD_COLOR_BG);
	}
}

void ShiftScreen(u8* screen, int width, int shift, int bgcolor)
{
	for (int x = 1; x <= width; x++) {
		for (int y = 1; y <= BYTES_PER_PIXEL * (SCREEN_HEIGHT - shift); y++)
			*(screen + (x * SCREEN_HEIGHT * BYTES_PER_PIXEL) - y) = *(screen + (x * SCREEN_HEIGHT * BYTES_PER_PIXEL) - y - (shift * BYTES_PER_PIXEL));
		for (int y = 0; y < BYTES_PER_PIXEL * shift; y++)
			*(screen + ((x - 1) * SCREEN_HEIGHT * BYTES_PER_PIXEL) + y) = bgcolor;
	}
}

void DrawCharacter(u8 *screen, int character, int x, int y, int color, int bgcolor)
{
	// char - 0x20 because non-essential parts of the font are trimmed off
	u64 charData = font[character - 0x20];

	for (int yy = 0; yy < CHR_HEIGHT; yy++) {
		int xDisplacement = (x * BYTES_PER_PIXEL * SCREEN_HEIGHT);
		int yDisplacement = ((SCREEN_HEIGHT - (y + yy) - 1) * BYTES_PER_PIXEL);
		u8 *screenPos = screen + xDisplacement + yDisplacement;

		for (int xx = 0; xx < CHR_WIDTH; xx++) {
			if ((charData >> ((yy * CHR_WIDTH) + xx)) & 1) {
				*(screenPos + 0) = color >> 16;         // B
				*(screenPos + 1) = color >> 8;          // G
				*(screenPos + 2) = color & 0xFF;        // R
			} else if (bgcolor != COLOR_TRANSPARENT) {
				*(screenPos + 0) = bgcolor >> 16;       // B
				*(screenPos + 1) = bgcolor >> 8;        // G
				*(screenPos + 2) = bgcolor & 0xFF;      // R
			}
			screenPos += BYTES_PER_PIXEL * SCREEN_HEIGHT;
		}
	}
}

void DebugDrawString(const char *str, int color, int bgcolor)
{
	for (size_t i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n' || debugxpos >= DBG_END_X) {
			debugxpos = 0;
			debugypos += DBG_STEP_Y;
			if (debugypos >= DBG_END_Y) {
				debugypos = (DBG_END_Y - (DBG_END_Y % DBG_STEP_Y) - DBG_STEP_Y);
				ShiftScreen(bot_screen0, DBG_END_X, DBG_STEP_Y, bgcolor);
				if (bot_screen0 != bot_screen1)
					ShiftScreen(bot_screen1, DBG_END_X, DBG_STEP_Y, bgcolor);
			}
		}
		if (str[i] != '\n') {
			DrawCharacter(bot_screen0, str[i], debugxpos, debugypos, color, bgcolor);
			DrawCharacter(bot_screen1, str[i], debugxpos, debugypos, color, bgcolor);
			debugxpos += DBG_STEP_X;
		}
	}
}

void DebugClear()
{
	debugxpos = 0;
	debugypos = 0;

	ClearScreenFull(false, true);
}

void DebugColor(u32 color, const char* message)
{
	DebugDrawString(message, color, DBG_COLOR_BG);
}

void Debug(const char *message)
{
	DebugColor(DBG_COLOR_FONT, message);
}
