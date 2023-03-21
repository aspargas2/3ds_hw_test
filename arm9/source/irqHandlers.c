#include "irqHandlers.h"

#include <debug.h>
#include <pxi.h>
#include <arm.h>

#include "irq.h"
#include "log.h"
#include "timer.h"

#define PXICMD_QUEUE_LEN 64

#define CIRC_INC(i) (((i) == (PXICMD_QUEUE_LEN - 1)) ? (0) : ((i) + 1))
#define CIRC_DEC(i) (((i) == 0) ? (PXICMD_QUEUE_LEN - 1) : ((i) - 1))

static u32 pxicmdQueue[PXICMD_QUEUE_LEN];

static u32 pxicmdQueueFront = 0;
static u32 pxicmdQueueBack = 0;

static void enqueuePxicmd(u32 cmd) {
	if (pxicmdQueueFront == CIRC_INC(pxicmdQueueBack))
		ARM_BKPT();

	pxicmdQueue[pxicmdQueueBack = CIRC_INC(pxicmdQueueBack)] = cmd;
}

u32 dequeuePxicmd() {
	if (pxicmdQueueFront == pxicmdQueueBack)
		return PXICMD_NONE;

	return pxicmdQueue[pxicmdQueueFront = CIRC_INC(pxicmdQueueFront)];
}

void irqHandlerMain(/*u32 lr*/) {
	//debugPrintf("Got IRQ; %x\n", lr);

	if(testInterrupt(TIMER_INTERRUPT(0)))
		clearInterrupt(TIMER_INTERRUPT(0));

	if (testInterrupt(PXI_RX_INTERRUPT)) {
		clearInterrupt(PXI_RX_INTERRUPT);

		while (!(*PXI_CNT & PXI_CNT_RECV_FIFO_EMPTY)) {
			u32 cmd = PXI_Recv();
			u32 argc = cmd >> 16;
			cmd &= 0xFFFF;

			switch (cmd) {
				case PXICMD_LOG_DATA:
					PXI_Send(logWrite((void*) PXI_Recv(), PXI_Recv()));
					break;

				case PXICMD_LOG_STRING:
					PXI_Send(logWriteStr((char*) PXI_Recv()));
					break;

				case PXICMD_GET_TIMER_TICKS:
					PXI_Send64(getFullTimerTicks());
					break;

				// For all other commands, place command ID and args in queue to be dealt with later
				default:
					enqueuePxicmd(cmd);
					for (u32 i = 0; i < argc; i++)
						enqueuePxicmd(PXI_Recv());
					break;
			}

			//debugPrintf("Handled cmd %d\n", cmd);
		}
	}
}