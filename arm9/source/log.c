#include "log.h"
#include "draw.h"

#include "fatfs/ff.h"

static FATFS fs;
static FIL logfile;
static bool logready;

bool InitLog()
{
	if (f_mount(&fs, "0:", 1) != FR_OK)
		return false;

	if (f_open(&logfile, LOG_FILE, FA_WRITE | FA_OPEN_APPEND) != FR_OK) {
		f_unmount("0:");
		return false;
	}

	logready = true;
	return true;
}

void DeinitLog()
{
	f_close(&logfile);
	f_unmount("0:");
	logready = false;
}

bool LogWrite(const char* text)
{
	UINT bw;
	const UINT btw = strlen(text);
	return logready && (f_write(&logfile, text, btw, &bw) == FR_OK) && (bw == btw);
}

