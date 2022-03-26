#include "common.h"
#include "draw.h"
#include "hid.h"
#include "log.h"
#include "i2c.h"
#include "memtests.h"

#include <stdlib.h>

static void mcu_poweroff()
{
	i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 0);
	while (1) ;
}

static u32 wait_any_key_pressed()
{
	u32 pad;
	while (~HID_PAD & BUTTON_NONE);
	while (!((pad = ~HID_PAD) & BUTTON_NONE));
	return pad;
}

static void wait_any_key_poweroff()
{
	Debug("Press any key to poweroff.\n");
	wait_any_key_pressed();
	mcu_poweroff();
}

static void LogDebug(const char* message) {
	LogWrite(message);
	Debug(message);
}

static void LogErrors(u32 n_errors) {
	for (u32 i = 0; i < min(MAX_ERRORS, n_errors) * 3; i += 3) {
		char str[9];
		itoa(errors_buffer[i], str, 16);
		LogWrite(str);
		LogWrite(": ");
		itoa(errors_buffer[i + 1], str, 16);
		LogWrite(str);
		LogWrite("->");
		itoa(errors_buffer[i + 2], str, 16);
		LogWrite(str);
		LogWrite("\n");
	}
}

int main(int argc, char *argv[])
{
	InitScreenFbs(argc, argv);
	ClearScreenFull(true, true);
	Debug("Hello from ARM9\n");

	//i2cWriteRegister(I2C_DEV_MCU, 0x29, 6);

	Debug("Initializing SD log... ");

	bool uselog;
	if ((uselog = InitLog())) {
		Debug("success\n");
	} else {
		Debug("failed. SD log will not be written.\n");
	}
	
	LogWrite("\n\n\n");
	LogDebug("=== 3ds_hw_test by aspargas2 - commit " COMMIT " ===\n");
	Debug("Press B to power off, any other button to begin\n");
	if (wait_any_key_pressed() & BUTTON_B) {
		DeinitLog();
		mcu_poweroff();
	}

	u32 n_errors;
	char str[20];

	for (u32 i = 0; i < N_REGIONS; i++) {
		LogDebug("\nTesting ");
		LogDebug(regions[i].name);
		LogDebug(" ...\n");
		for (u32 j = 0; j < N_TESTS; j++) {
			LogDebug("Running ");
			LogDebug(tests[j].name);
			LogDebug("...\n");
			n_errors = tests[j].func((void*) regions[i].start_addr, (void*) regions[i].start_addr + regions[i].size);
			LogDebug("Done. ");
			itoa(n_errors, str, 10);
			LogDebug(str);
			LogDebug(" errors detected.\n");
			if (n_errors && uselog) {
				Debug("Writing errors to SD log... ");
				LogErrors(n_errors);
				Debug("done\n");
			}
		}
	}

	Debug("Deinitializing SD log... ");
	DeinitLog();
	Debug("done\n");

	wait_any_key_poweroff();
}
