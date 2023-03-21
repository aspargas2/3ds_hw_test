#include "log.h"
#include "common.h"

#include <pxi.h>
#include <console.h>

u64 pxiGetTimerTicks() {
	PXI_Send((0 << 16) | PXICMD_GET_TIMER_TICKS);

	return PXI_Recv64();
}
