#include "log.h"
#include "common.h"

#include <pxi.h>
#include <console.h>

static u8 __attribute__((section(".shared"))) sharedBuffer[MAX_LOG_LEN];

bool pxiLogWrite(const void* data, unsigned int btw) {
	btw = min(btw, MAX_LOG_LEN);
	memcpy(sharedBuffer, data, btw);

	PXI_Send((2 << 16) | PXICMD_LOG_DATA);
	PXI_Send((u32) sharedBuffer);
	PXI_Send(btw);

	//consolePrint("sent pxi log data\n");

	return PXI_Recv();
}

bool pxiLogWriteStr(const char* str) {
	strncpy((char*) sharedBuffer, str, MAX_LOG_LEN - 1);

	PXI_Send((1 << 16) | PXICMD_LOG_DATA);
	PXI_Send((u32) sharedBuffer);

	//consolePrint("sent pxi log str\n");

	return PXI_Recv();
}

