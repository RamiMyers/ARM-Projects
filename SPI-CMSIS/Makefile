# Flags
CC := arm-none-eabi-gcc
CPUFLAGS := -mcpu=cortex-m4 -mthumb
INCFLAGS := -Iinc \
            -Iinc/CMSIS/Include \
            -Iinc/CMSIS/Device/ST/STM32F4xx/Include
DEFS := -DSTM32F446xx 
CFLAGS := $(CPUFLAGS) $(INCFLAGS) $(DEFS) -c -std=gnu11 
LDFLAGS := $(CPUFLAGS) -T stm32f446xe_flash.ld

# Project Structure
BUILD := ./build
SRCS := main.c syscall.c sysmem.c startup_stm32f446xx.s
OBJS := $(addprefix $(BUILD)/,$(SRCS:.c=.o))
OBJS := $(OBJS:.s=.o)

# Target
TARGET := UART.elf

$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD)/%.o: %.s
	$(CC) $(CFLAGS) $< -o $@

flash: $(BUILD)/$(TARGET)
	openocd -f board/st_nucleo_f4.cfg -c "program $< verify reset exit"

clean:
	rm -f $(BUILD)/*