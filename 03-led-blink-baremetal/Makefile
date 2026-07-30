# 1. Target Binary Name & Build Directory
TARGET    = 03baremetal
BUILD_DIR = build

# 2. Toolchain Definitions
CC      = arm-none-eabi-gcc
CP      = arm-none-eabi-objcopy
SZ      = arm-none-eabi-size

# 3. Architecture Flags (Cortex-M0)
MCU_FLAGS = -mcpu=cortex-m0 -mthumb

# 4. Source Files
C_SOURCES = \
	main.c \

ASM_SOURCES = \
	startup.s

# 5. Compiler & Linker Flags
CFLAGS  = $(MCU_FLAGS) -I. -O0 -Wall -g -fdata-sections -ffunction-sections
LDFLAGS = $(MCU_FLAGS) -T linkerscript.ld -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections -nostdlib

# 6. Object File Generation List
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# 7. Default Build Target
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

# Compile C Files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling: $<"
	@$(CC) -c $(CFLAGS) $< -o $@

# Assemble ASM Files
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@echo "Assembling: $<"
	@$(CC) -c $(CFLAGS) $< -o $@

# Link Objects into ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@echo "Linking: $@"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "Memory Usage:"
	@$(SZ) $@

# Convert ELF to raw Binary
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@echo "Generating Binary: $@"
	@$(CP) -O binary -S $< $@

# Create Build Directory
$(BUILD_DIR):
	@mkdir -p $@

# 8. Flash via OpenOCD
flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f0x.cfg \
	-c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"

# 9. Clean Output Files
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean flash