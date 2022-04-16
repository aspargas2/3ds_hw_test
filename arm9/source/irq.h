#pragma once

#include "common.h"

// https://www.3dbrew.org/wiki/IRQ_Registers

#define IRQ_IE ((vu32*)0x10001000)
#define IRQ_IF ((vu32*)0x10001004)

static inline void disableAllInterrupts() {
	*IRQ_IE = 0;
}

static inline void enableInterrupt(u32 irqn) {
	*IRQ_IE |= BIT(irqn);
}

static inline void disableInterrupt(u32 irqn) {
	*IRQ_IE &= ~BIT(irqn);
}

static inline void clearAllInterrupts() {
	*IRQ_IF = -1;
}

static inline bool testInterrupt(u32 irqn) {
	return *IRQ_IF & BIT(irqn);
}

static inline void clearInterrupt(u32 irqn) {
	*IRQ_IF &= BIT(irqn);
}