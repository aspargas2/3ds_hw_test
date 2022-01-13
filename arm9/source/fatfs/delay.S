.arm
.global waitcycles
.type   waitcycles STT_FUNC

@waitcycles ( u32 us )
waitcycles:
	PUSH    {R0-R3,LR}
	waitcycles_loop:
		LDR     R3, [SP]
		SUBS    R2, R3, #1
		STR     R2, [SP]
		CMP     R3, #0
		BNE     waitcycles_loop
	POP     {R0-R3,PC}
