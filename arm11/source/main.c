#include <common.h>
#include <arm.h>
#include <pxi.h>
#include <i2c.h>
#include <hid.h>
#include <power.h>

#include <stdlib.h>

#include "arm/gic.h"
#include "arm/timer.h"

#include "hw/gpulcd.h"
#include "hw/mcu.h"

#include "system/sys.h"
#include "system/event.h"

#include "ui/ui.h"
#include "ui/debug.h"

#include "pxi/timer.h"

#include <memtests.h>

static u32 wait_any_key_pressed()
{
	u32 pad;
	while (~HID_PAD & BUTTON_NONE) ARM_WFI();
	while (!((pad = ~HID_PAD) & BUTTON_NONE)) ARM_WFI();
	return pad;
}

/*static void vblankUpdate(void)
{
	if (!getEventIRQ()->test(VBLANK_INTERRUPT, true))
		return;

	// handle shell events
	static const u32 mcuEvShell = MCUEV_HID_SHELL_OPEN | MCUEV_HID_SHELL_CLOSE;
	u32 shell = getEventMCU()->test(mcuEvShell, mcuEvShell);
	if (shell & MCUEV_HID_SHELL_CLOSE) {
		GFX_powerOffBacklights(GFX_BLIGHT_BOTH);
	} else if (shell & MCUEV_HID_SHELL_OPEN) {
		GFX_powerOnBacklights(GFX_BLIGHT_BOTH);
	}

	//sharedMem.hidState.full = HID_GetState();
}*/

void __attribute__((noreturn)) MainLoop(void)
{
	//bool mainLoop = true;

	u8 fixed_bright_lvl = 0x80;
	GFX_setBrightness(fixed_bright_lvl, fixed_bright_lvl);

	// initialize state stuff
	getEventIRQ()->reset();
	getEventMCU()->reset();

	// configure interrupts
	gicSetInterruptConfig(PXI_RX_INTERRUPT, BIT(0), GIC_PRIO0, NULL);
	gicSetInterruptConfig(VBLANK_INTERRUPT, BIT(0), GIC_PRIO0, NULL);
	gicSetInterruptConfig(MCU_INTERRUPT, BIT(0), GIC_PRIO0, NULL);

	// enable interrupts
	gicEnableInterrupt(MCU_INTERRUPT);

	// perform gpu init after initializing mcu but before
	// enabling the pxi system and the vblank handler
	GFX_init(GFX_RGB565);

	gicEnableInterrupt(PXI_RX_INTERRUPT);
	gicEnableInterrupt(VBLANK_INTERRUPT);

	initUI();

	// ARM9 won't try anything funny until this point
	PXI_Barrier(PXI_BOOT_BARRIER);
	debugPrint("Hello from ARM11\n");

	consolePrint("Press B to power off, any other button to begin\n");
	if (wait_any_key_pressed() & BUTTON_B) {
		PXI_Send(PXICMD_LEGACY_BOOT);
		PXI_Barrier(PXI_FIRMLAUNCH_BARRIER);
		mcuPoweroff();
	}

	testMemory(ALL_MEMTEST_REGIONS, ALL_MEMTESTS);

	consolePrint("Press any button to power off.\n");
	wait_any_key_pressed();
	PXI_Send(PXICMD_LEGACY_BOOT);
	PXI_Barrier(PXI_FIRMLAUNCH_BARRIER);
	mcuPoweroff();

	// perform deinit in reverse order
	gicDisableInterrupt(VBLANK_INTERRUPT);
	gicDisableInterrupt(PXI_RX_INTERRUPT);

	// unconditionally reinitialize the screens
	// in RGB24 framebuffer mode
	GFX_init(GFX_BGR8);

	gicDisableInterrupt(MCU_INTERRUPT);

	// Wait for the ARM9 to do its firmlaunch setup
	//PXI_Barrier(PXI_FIRMLAUNCH_BARRIER);

	SYS_CoreZeroShutdown();
	SYS_CoreShutdown();
}
