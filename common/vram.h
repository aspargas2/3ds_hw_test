#pragma once

#define TOP_VRAM    (400*240*2)
#define BOTTOM_VRAM (320*240*2)

#define VRAM_START  (0x18000000)

#define VRAM_TOP_LA (VRAM_START)
#define VRAM_BOT_A  (VRAM_TOP_LA + TOP_VRAM)

#define VRAM_END    (VRAM_BOT_A + BOTTOM_VRAM)
