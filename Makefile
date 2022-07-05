TARGET = 3ds_hw_test

ARM9_LOAD      = 0x08000100
ARM9_ENTRY     = $(ARM9_LOAD)
ARM9_ALT_LOAD  = 0x01FF8000
ARM9_ALT_ENTRY = $(ARM9_ALT_LOAD)
ARM9_BIN       = arm9/$(TARGET)_arm9.bin

ARM11_LOAD  = 0x18100000
ARM11_ENTRY = $(ARM11_LOAD)
ARM11_BIN   = arm11/$(TARGET)_arm11.bin

export CC      = arm-none-eabi-gcc
export OBJCOPY = arm-none-eabi-objcopy

ifeq (, $(shell which $(CC) 2> /dev/null))
    export USE_DKA = true
endif
ifeq (, $(shell which $(OBJCOPY) 2> /dev/null))
    export USE_DKA = true
endif

ifeq ($(USE_DKA),true)
    ifeq ($(strip $(DEVKITARM)),)
        $(error "Please provide $(CC) and $(OBJCOPY) in PATH, or set DEVKITARM in your environment.")
    endif
    $(info Building with DKA)
    include $(DEVKITARM)/base_tools
endif

.PHONY: all clean .FORCE
.FORCE:

all: $(TARGET).firm $(TARGET)_direct.firm

$(TARGET).firm: $(ARM9_BIN) $(ARM11_BIN)
	firmtool build $@ -n $(ARM9_ENTRY) -e $(ARM11_ENTRY) -D $(ARM9_BIN) $(ARM11_BIN) -A $(ARM9_LOAD) $(ARM11_LOAD) -C NDMA XDMA -b
$(TARGET)_direct.firm: $(ARM9_BIN) $(ARM11_BIN)
	firmtool build $@ -n $(ARM9_ALT_ENTRY) -e $(ARM11_ENTRY) -D $(ARM9_BIN) $(ARM11_BIN) -A $(ARM9_ALT_LOAD) $(ARM11_LOAD) -C memcpy XDMA -b

$(ARM9_BIN): .FORCE
	@$(MAKE) -C arm9

$(ARM11_BIN): .FORCE
	@$(MAKE) -C arm11

clean:
	@$(MAKE) -C arm9 clean
	@$(MAKE) -C arm11 clean
	@rm -f $(TARGET)*.firm