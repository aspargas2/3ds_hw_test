#pragma once

#include "common.h"

// https://www.3dbrew.org/wiki/TIMER_Registers

#define N_TIMERS 4
#define TIMER_FREQ 67027964

#define TIMER_VAL(n) (*(vu16*)(0x10003000 + 4*(n)))
#define TIMER_CNT(n) (*(u16*)(0x10003002 + 4*(n)))
#define TIMER_INTERRUPT(n) (8 + n)

typedef enum { // timer will run at TIMER_FREQ / one of these
	FREQ_1,
	FREQ_64,
	FREQ_256,
	FREQ_1024,
} TimerPrescaler;

#define TIMER_COUNT_UP   0x0004
#define TIMER_ENABLE_IRQ 0x0040
#define TIMER_ACTIVE     0x0080

static inline void resetTimer(int n) {
	TIMER_CNT(n) = 0;
	TIMER_VAL(n) = 0;
}

void resetAllTimers();
void startFullTimerCountup(TimerPrescaler prescaler);
u64 getFullTimerTicks();