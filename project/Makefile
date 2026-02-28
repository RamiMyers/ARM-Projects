# ----- Toolchain -----
CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size

# ----- Project -----
TARGET   := project
ELF      := $(TARGET).elf
LDSCRIPT := STM32F446RETX_FLASH.ld

# ----- CPU / FPU (from CubeIDE log) -----
CPUFLAGS := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# ----- Defines / Includes (from your compile line) -----
DEFS := 
INCS := -IInc

# ----- Common compile flags -----
CFLAGS := $(CPUFLAGS) -std=gnu11 -O0 -g3 \
          -ffunction-sections -fdata-sections \
          -Wall -MMD -MP \
          -fstack-usage \
          $(DEFS) $(INCS)

# Startup is plain asm in CubeIDE; preprocess if you use #includes/#defines
ASFLAGS := $(CPUFLAGS) -x assembler-with-cpp -MMD -MP $(DEFS) $(INCS)

# ----- Link flags (from CubeIDE log) -----
LDFLAGS := $(CPUFLAGS) \
          -T$(LDSCRIPT) \
          -Wl,-Map=$(TARGET).map \
          -Wl,--gc-sections \
          -static \
          -specs=nosys.specs -specs=nano.specs \
          -Wl,--start-group -lc -lm -Wl,--end-group

# ----- Objects (from objects.list) -----
OBJS := Src/main.o \
        Src/syscalls.o \
        Src/sysmem.o \
        Startup/startup_stm32f446retx.o

DEPS := $(OBJS:.o=.d)

# ----- Default target -----
all: $(TARGET).elf $(TARGET).hex $(TARGET).bin

# ----- Link -----
$(ELF): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(SIZE) $@

# ----- Compile C -----
Src/%.o: Src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# ----- Assemble startup -----
Startup/%.o: Startup/%.s
	$(CC) -c $< -o $@ $(ASFLAGS)

# ----- Objcopy outputs -----
$(TARGET).hex: $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(ELF)
	$(OBJCOPY) -O binary $< $@

# ----- Convenience targets -----
clean:
	rm -f $(OBJS) $(DEPS) $(ELF) $(TARGET).map $(TARGET).hex $(TARGET).bin \
	      Src/*.su Src/*.cyclo

# OpenOCD (adjust cfg for your board)
flash: $(ELF)
	openocd -f board/st_nucleo_f4.cfg -c "program $(ELF) verify reset exit"

# ----- Include auto-deps -----
-include $(DEPS)