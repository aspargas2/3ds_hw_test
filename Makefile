TARGET = 3ds_hw_test

ARM9_LOAD      = 0x08000100
ARM9_ENTRY     = $(ARM9_LOAD)
ARM9_ALT_LOAD  = 0x01FF8000
ARM9_ALT_ENTRY = $(ARM9_ALT_LOAD)
ARM9_BIN       = arm9/$(TARGET)_arm9.bin

#ARM11_LOAD  = 
#ARM11_ENTRY = $(ARM11_LOAD)
#ARM11_BIN   = arm11/$(TARGET)_arm11.bin

.PHONY: all bins clean .FORCE
.FORCE:

all: $(TARGET).firm $(TARGET)_direct.firm

#$(TARGET).firm: $(ARM9_BIN) $(ARM11_BIN)
#	firmtool build $@ -n $(ARM9_ENTRY) $(ARM11_ENTRY) -D $(ARM9_BIN) $(ARM11_BIN) -A $(ARM9_LOAD) $(ARM11_LOAD) -C NDMA XDMA -i
$(TARGET).firm: $(ARM9_BIN)
	firmtool build $@ -n $(ARM9_ENTRY) -e 0 -D $(ARM9_BIN) -A $(ARM9_LOAD) -C NDMA -i
$(TARGET)_direct.firm: $(ARM9_BIN)
	firmtool build $@ -n $(ARM9_ALT_ENTRY) -e 0 -D $(ARM9_BIN) -A $(ARM9_ALT_LOAD) -C memcpy -i

$(ARM9_BIN): .FORCE
	@$(MAKE) -C arm9

#$(ARM11_BIN): .FORCE
#	@$(MAKE) -C arm11

clean:
	@$(MAKE) -C arm9 clean
	#@$(MAKE) -C arm11 clean
	@rm -f $(TARGET).firm