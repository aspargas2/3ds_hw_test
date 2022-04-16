#include "log.h"

#include "fatfs/ff.h"

#include <arm.h>

static FATFS fs;
static FIL logfile;
static bool logready = false;

bool logReady() {
	return logready;
}

bool initLog() {
	if (f_mount(&fs, "0:", 1) != FR_OK)
		return false;

	if (f_open(&logfile, LOG_FILE, FA_WRITE | FA_OPEN_APPEND) != FR_OK) {
		f_unmount("0:");
		return false;
	}

	logready = true;
	return true;
}

void deinitLog() {
	logready = false;
	f_close(&logfile);
	f_unmount("0:");
}

bool logWrite(const void* data, unsigned int btw) {
	if (!logready)
		return false;

	UINT bw;
	u32 irqState = ARM_EnterCritical();
	bool ret = (f_write(&logfile, data, btw, &bw) == FR_OK) && (bw == btw);
	ARM_LeaveCritical(irqState);

	return ret;
}

bool logWriteStr(const char* str) {
	return logready && logWrite(str, strlen(str));
}

