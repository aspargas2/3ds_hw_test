#include "timer.h"

#include <arm.h>

void resetAllTimers() {
	for (int i = 0; i < N_TIMERS; i++)
		resetTimer(i);
}

void startFullTimerCountup(TimerPrescaler prescaler) {
	resetAllTimers();

	TIMER_CNT(0) = TIMER_ACTIVE | TIMER_ENABLE_IRQ | prescaler;
	TIMER_CNT(1) = TIMER_CNT(2) = TIMER_CNT(3) = TIMER_ACTIVE | TIMER_COUNT_UP;
}

u64 getFullTimerTicks() {
    u64 ticks = 0;
	u32 irqState = ARM_EnterCritical();

	for (int i = N_TIMERS - 1; i >= 0; i--) {
		ticks <<= 16;
		ticks |= (u64) TIMER_VAL(i);
	}

	ARM_LeaveCritical(irqState);
    return ticks;
}