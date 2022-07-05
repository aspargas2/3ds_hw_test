TARGET   = 3ds_hw_test_arm9

export COMMIT := $(shell git rev-parse --short HEAD)

SOURCE   = source
BUILD    = build
SOURCES  = $(SOURCE) $(SOURCE)/* ../common
INCLUDES = $(SOURCES)

SFILES  = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.S))
CFILES  = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.c))

OBJS    = $(subst ../common/,build/cmn.,$(subst source,build,$(SFILES:.S=.o) $(CFILES:.c=.o)))
DEPS    = $(OBJS:.o=.d)
INCLUDE = $(foreach dir, $(INCLUDES), -I$(CURDIR)/$(dir))

ARCH    = -DARM9 -mcpu=arm946e-s -march=armv5te -mthumb -mlittle-endian -mthumb-interwork
ASFLAGS = $(ARCH) $(COMMON_ARCH) $(INCLUDE)
CFLAGS  = -DCOMMIT="\"$(COMMIT)\"" -Wall -Wextra -Os -flto $(ARCH) $(INCLUDE) -fno-stack-protector

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

.PHONY: all clean

all: $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(CC) -T link.ld -flto -Wl,--nmagic,--use-blx $^ -o $@ -nostartfiles -nostdlib
	#$(CC) -T link.ld -Wl,--nmagic $^ -o $@ -nostartfiles -nostdlib

$(BUILD)/%.o: $(SOURCE)/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/%.o: $(SOURCE)/%.S
	@mkdir -p "$(@D)"
	$(CC) $(ASFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/cmn.%.o: ../common/%.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/cmn.%.o: ../common/%.S
	@mkdir -p "$(@D)"
	$(CC) $(ASFLAGS) -MMD -MP -c $< -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -S -O binary $< $@

clean:
	@rm -rf $(OBJS) $(TARGET).elf $(TARGET).bin $(BUILD)

-include $(DEPS)
