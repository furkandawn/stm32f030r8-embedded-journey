# ============================================================================
# Project Configuration
# ============================================================================

TARGET = 04systeminit
BUILD_DIR = build

# ============================================================================
# Toolchain
# ============================================================================

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# ============================================================================
# Target MCU
# ============================================================================

MCU_FLAGS = -mcpu=cortex-m0 -mthumb

# ============================================================================
# Source Files
# ============================================================================

C_SOURCES = \
	main.c \
	system.c

ASM_SOURCES = \
	startup.s

# ============================================================================
# Include Directories
# ============================================================================

C_INCLUDES = \
	-I. \
	-IDrivers/CMSIS/Include \
	-IDrivers/CMSIS/Device/ST/STM32F0xx/Include

# ============================================================================
# Compiler & Linker Flags
# ============================================================================

CFLAGS = $(MCU_FLAGS) $(C_INCLUDES) \
	-DSTM32F030x8 \
	-O0 \
	-Wall \
	-g \
	-ffunction-sections \
	-fdata-sections

LDFLAGS = $(MCU_FLAGS) \
	-T linkerscript.ld \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections \
	-nostdlib

# ============================================================================
# Object Files
# ============================================================================

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))

vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# ============================================================================
# Build Targets
# ============================================================================

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

# Compile C Sources
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo Compiling: $<
	@$(CC) -c $(CFLAGS) $< -o $@

# Assemble Startup File
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@echo Assembling: $<
	@$(CC) -c $(CFLAGS) $< -o $@

# Link Executable
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	@echo Linking: $@
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo Memory Usage:
	@$(SIZE) $@

# Generate Raw Binary
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@echo Generating Binary: $@
	@$(OBJCOPY) -O binary -S $< $@

# ============================================================================
# Utility Targets
# ============================================================================

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f0x.cfg \
	-c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all flash clean