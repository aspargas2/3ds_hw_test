#include "common.h"
#include "hid.h"
#include "log.h"
#include "i2c.h"
#include "timer.h"
#include "irq.h"
#include "irqHandlers.h"

#include <memtests.h>
#include <power.h>
#include <pxi.h>
#include <debug.h>
#include <console.h>
#include <arm.h>

#include <stdlib.h>

#define TITLE_STRING ("===== 3ds_hw_test by aspargas2 - commit " COMMIT " =====\n")

static u32 wait_any_key_pressed()
{
	u32 pad;
	while (~HID_PAD & BUTTON_NONE);
	while (!((pad = ~HID_PAD) & BUTTON_NONE));
	return pad;
}

static void wait_any_key_poweroff()
{
	consolePrint("Press any key to poweroff.\n");
	wait_any_key_pressed();
	mcuPoweroff();
}

static bool bootBarrierWithTimeout() {
	bool ret;

	PXI_SetRemote(PXI_BOOT_BARRIER);
	startFullTimerCountup(FREQ_1024);

	while (1) {
		if (PXI_GetRemote() == PXI_BOOT_BARRIER) {
			ret = true;
			break;
		} else if (TIMER_VAL(1) > 3) {
			ret = false;
			break;
		}
	}

	return ret;
}

int main(/*int argc, char *argv[]*/) {
	bool arm11dead, mainLoop = true;

	PXI_Reset();
	resetAllTimers();
	disableAllInterrupts();
	clearAllInterrupts();

	// If bootroms are unlocked, lock them to unlock FCRAM
	bool locked = false;
	u8* sysprot = (void*) 0x10000000; // CFG9_SYSPROT9
	if (*sysprot == 0) {
		locked = true;
		*sysprot = 1;
		sysprot++; // CFG9_SYSPROT11
		*sysprot = 1;
	}

	// Enable Extended ARM9 Memory
	if (!IS_O3DS)
		(*(u8*)0x10000200) = 1;

	if ((arm11dead = !bootBarrierWithTimeout()))
		// Flash power LED red, as the screens are probably off
		I2C_writeReg(I2C_DEV_CTR_MCU, 0x29, 6);

	// Gives an interrupt around once a second, ensuring we can WFI without getting stuck forever
	enableInterrupt(TIMER_INTERRUPT(0));

	consolePrintColor(TITLE_STRING, COLOR_LIGHT_BLUE);
	consolePrint("Hello from ARM9\nInitializing SD log... ");

	if (initLog()) {
		consolePrint("success\n");
	} else {
		consolePrint("failed. SD log will not be written.\n");
	}

	logWriteStr("\n\n\n");
	logWriteStr(TITLE_STRING);

	if (locked)
		debugPrint("Note: launched with bootroms unlocked\n");

	if (!arm11dead) {
		enableInterrupt(PXI_RX_INTERRUPT);
		ARM_EnableInterrupts();

		//debugPrint("Entering main loop\n");

		while (mainLoop) {
			u32 pxiCmd = dequeuePxicmd();

			switch(pxiCmd) {
				case PXICMD_NONE:
					ARM_WFI();
					break;

				case PXICMD_RUN_MEMTEST: {
					u64* totalErrorsAddr = (u64*) dequeuePxicmd();
					*totalErrorsAddr = testMemory(dequeuePxicmd(), dequeuePxicmd(), false);
					//ARM_BKPT();
					break;
				}

				case PXICMD_LEGACY_BOOT:
					mainLoop = false;
					debugPrint("Deinitializing SD log... ");
					deinitLog();
					consolePrint("done\n");
					break;

				default:
					debugPrintf("Got unknown PXI cmd from queue: %lx\n", pxiCmd);
					break;
			}

			//debugPrintf("looped %x %x %x\n", *IRQ_IE, *IRQ_IF, *PXI_CNT);
		}

		ARM_DisableInterrupts();
		PXI_Barrier(PXI_FIRMLAUNCH_BARRIER);
	} else {
		debugPrint("\nARM11 failed to boot, continuing independently\n\n");

		consolePrint("Press B to power off, anything else to test memory\n");
		if (wait_any_key_pressed() & BUTTON_B) {
			deinitLog();
			mcuPoweroff();
		}

		I2C_writeReg(I2C_DEV_CTR_MCU, 0x29, 1);
		testMemory(ALL_MEMTEST_REGIONS, ALL_MEMTESTS, true);
		I2C_writeReg(I2C_DEV_CTR_MCU, 0x29, 6);

		debugPrint("Deinitializing SD log... ");
		deinitLog();
		consolePrint("done\n");

		wait_any_key_poweroff();
	}

	while (1);
}
