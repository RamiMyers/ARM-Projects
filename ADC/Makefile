# Flags
CC := arm-none-eabi-gcc
CPUFLAGS := -mcpu=cortex-m4 -mthumb
CFLAGS := $(CPUFLAGS) -c -std=gnu11 -Iinc
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