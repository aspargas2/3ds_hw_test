TARGET = 3ds_hw_test

ARM9_LOAD      = 0x08000100
ARM9_ENTRY     = $(ARM9_LOAD)
ARM9_ALT_LOAD  = 0x01FF8000
ARM9_ALT_ENTRY = $(ARM9_ALT_LOAD)
ARM9_BIN       = arm9/$(TARGET)_arm9.bin

ARM11_LOAD  = 0x18100000
ARM11_ENTRY = $(ARM11_LOAD)
ARM11_BIN   = arm11/$(TARGET)_arm11.bin

.PHONY: all clean .FORCE
.FORCE:

all: $(TARGET).firm $(TARGET)_direct.firm

$(TARGET).firm: $(ARM9_BIN) $(ARM11_BIN)
	firmtool build $@ -n $(ARM9_ENTRY) -e $(ARM11_ENTRY) -D $(ARM9_BIN) $(ARM11_BIN) -A $(ARM9_LOAD) $(ARM11_LOAD) -C NDMA XDMA
$(TARGET)_direct.firm: $(ARM9_BIN) $(ARM11_BIN)
	firmtool build $@ -n $(ARM9_ALT_ENTRY) -e $(ARM11_ENTRY) -D $(ARM9_BIN) $(ARM11_BIN) -A $(ARM9_ALT_LOAD) $(ARM11_LOAD) -C memcpy XDMA

$(ARM9_BIN): .FORCE
	@$(MAKE) -C arm9

$(ARM11_BIN): .FORCE
	@$(MAKE) -C arm11

clean:
	@$(MAKE) -C arm9 clean
	@$(MAKE) -C arm11 clean
	@rm -f $(TARGET)*.firm