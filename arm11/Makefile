TARGET   = 3ds_hw_test_arm11

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

ARCH    = -DARM11 -march=armv6k -mtune=mpcore -mlittle-endian -marm -mtp=soft
ASFLAGS = $(ARCH) $(COMMON_ARCH) $(INCLUDE)
CFLAGS  = -DCOMMIT="\"$(COMMIT)\"" -Wall -Wextra -Os $(ARCH) $(INCLUDE) -fno-stack-protector

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

.PHONY: all clean

all: $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(CC) -T link.ld -Wl,--nmagic,--use-blx $^ -o $@ -nostartfiles -specs=nosys.specs -specs=nano.specs

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
