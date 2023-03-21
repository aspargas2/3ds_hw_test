#pragma once

#include "common.h"

#define CHR_HEIGHT 10
#define CHR_WIDTH  6

// Generated with an ugly python script; each u64 is a character's font data

static const u64 font[] = {
	/*0x0, 0x7adce1ce1780, 0x7b3b7fb7f780, 0x873ef94000, 0x873e708000, 0x708db671c000, 0x708fbe708000, 0x31e300000,
	0xffffffce1cfffff, 0xc4a148c000, 0xfffff3b5eb73fff, 0x72289e128f00, 0x21c21c8a2700, 0x18e208a28600, 0xdb6924f24f00, 0x22a73672a200,
	0x8639e386080, 0x830e3ce30800, 0x21ca88a9c200, 0x480492492480, 0xa28a3caaaabc, 0x72242891228489c, 0x7df7c0000000, 0x3e21ca88a9c200,
	0x208208a9c200, 0x21ca88208200, 0x843e408000, 0x813e108000, 0x782082000000, 0x1287f852000, 0xfbe71c208000, 0x20871cfbe000,*/
	0x0, 0x200208208200, 0x14514, 0x53e514f94000, 0x872aa1c2aa708, 0xcb2108426980, 0xb12a84292300, 0x8208,
	0x10204104104210, 0x4210410410204, 0x22a72a200, 0x823e208000, 0x10830c000000000, 0x3e000000, 0x618000000000, 0x82104208410820,
	0x7229aaca2700, 0xf8820828c200, 0xf84210822700, 0x722818822700, 0x410f92518400, 0x722820782f80, 0x72289e084600, 0x208210820f80,
	0x72289c8a2700, 0x31083c8a2700, 0x618018600000, 0x10830c00c300000, 0x810204210800, 0xf80f80000, 0x108420408100, 0x200210822700,
	0x702eaaea2700, 0x8a2fa2894200, 0x7a491c924780, 0x7220820a2700, 0x7a4924924780, 0xf8209e082f80, 0x8209e082f80, 0x7228ba0a2700,
	0x8a28be8a2880, 0x708208208700, 0x312490410e00, 0x892286292880, 0xf82082082080, 0x8a28aaab6880, 0x8a28b2aa6880, 0x7228a28a2700,
	0x8209e8a2780, 0xb12aa28a2700, 0x89229e8a2780, 0x72281c0a2700, 0x208208208f80, 0x7228a28a2880, 0x2148a28a2880, 0x8b6aaa8a2880,
	0x8a2508522880, 0x2082148a2880, 0xf82108420f80, 0x18208208208218, 0x820410208104082, 0xc20820820820c, 0x894200, 0x3e000000000000,
	0x10204, 0xf22f20700000, 0x6a68a6682080, 0x7220a2700000, 0xb328b2b20800, 0xf02fa2700000, 0x20821c208c00, 0x720b328b2b00000,
	0x8a28a2782080, 0x708208300200, 0x188208208300200, 0x89238a482080, 0x708208208300, 0xaaaaaa580000, 0x8a28a6680000, 0x7228a2700000,
	0x826a68a6680000, 0x820b328b2b00000, 0x820a6680000, 0x7a0702f00000, 0xc08208708200, 0xb328a2880000, 0x2148a2880000, 0x52aaaaa80000,
	0x894214880000, 0x720f228a2880000, 0xf84210f80000, 0x10208204208210, 0x8208200208208, 0x4208210208204, 0x10a84000000, 0xfa2894200000,
	/*0x1087220820a2700, 0xb328a2880500, 0xf02fa2700210, 0xf22f20700508, 0xf22f20700500, 0xf22f20700408, 0xf22f20718918, 0x1087220a2700000,
	0xf02fa2700508, 0xf02fa2700500, 0xf02fa2700204, 0x708208300500, 0x708208300508, 0x708208300204, 0x8a2fa2894222, 0x8a2fa289c48c,
	0xf8209e082f88, 0xf0afa8780000, 0xe8a29e28af00, 0x7228a2700508, 0x7228a2700500, 0x7228a2700204, 0xb328a2880508, 0xb328a2880204,
	0x720f228a2880500, 0x7228a28a2721, 0x7228a28a2891, 0x872a2aa708000, 0x7a208238248c, 0x208f88f94880, 0x892e92392380, 0x18821c208c00,
	0xf22f20700210, 0x708208300210, 0x7228a2700210, 0xb328a2880210, 0x8a28a66806ac, 0x8a28b2aa689a, 0xf80f22f20700, 0xf807228a2700,
	0x722084200200, 0x20be000000, 0x2083e000000, 0x38422508524184, 0x10f1a508524184, 0x208208200200, 0x912252900000, 0x252912240000,
	0x888888888888888, 0xa95a95a95a95a95, 0xbbbbbbbbbbbbbbb, 0x208208208208208, 0x20820820f208208, 0x2082083c83c8208, 0x514514517514514, 0x51451451f000000,
	0x2082083c83c0000, 0x5145145d05d4514, 0x514514514514514, 0x5145145d07c0000, 0x7d05d4514, 0x1f514514, 0x3c83c8208, 0x20820820f000000,
	0x38208208, 0x3f208208, 0x20820823f000000, 0x208208238208208, 0x3f000000, 0x20820823f208208, 0x208208e08e08208, 0x514514534514514,
	0xf04d14514, 0x514514d04f00000, 0xfc0dd4514, 0x514514dc0fc0000, 0x514514d04d14514, 0xfc0fc0000, 0x514514dc0dd4514, 0xfc0fc8208,
	0x3f514514, 0x208208fc0fc0000, 0x51451453f000000, 0x3c514514, 0xe08e08208, 0x208208e08e00000, 0x51451453c000000, 0x51451453f514514,
	0x208208fc8fc8208, 0xf208208, 0x208208238000000, 0xfffffffffffffff, 0xfffffffc0000000, 0x1c71c71c71c71c7, 0xe38e38e38e38e38, 0x3fffffff,
	0xb12492b00000, 0x26a28924a2918, 0x820828a2f80, 0x514514f80000, 0xfa4210224f80, 0x312492f00000, 0x82b92492480000, 0xc08208f80000,
	0x70872289c21c, 0x7228be8a2700, 0xd948a28a2700, 0x624924604600, 0x72aaaa700000, 0x9caaa720000, 0xf0209c082f00, 0x8a28a28a2700,
	0x3f03f03f000, 0xf80208f88200, 0x700108408100, 0x700408108400, 0x208208208208c00, 0x6208208208208, 0x803e008000, 0x42a110a84000,
	0x31248c, 0x21c200000, 0x8000000, 0x10428a410800, 0x28a286, 0x384206, 0x71c71c71c000, 0x0,*/
};