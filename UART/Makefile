CC = arm-none-eabi-gcc
CPUFLAGS = -mcpu=cortex-m4 -mthumb
CFLAGS = $(CPUFLAGS) -c -std=gnu11 -Iinc
LDFLAGS = $(CPUFLAGS) -T stm32f446xe_flash.ld
TARGET = UART.elf

$(TARGET): main.o syscall.o sysmem.o startup_stm32f446xx.o
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) $< -o $@

syscall.o: syscall.c
	$(CC) $(CFLAGS) $< -o $@

sysmem.o: sysmem.c
	$(CC) $(CFLAGS) $< -o $@

startup_stm32f446xx.o: startup_stm32f446xx.s
	$(CC) $(CFLAGS) $< -o $@

flash:
	openocd -f board/st_nucleo_f4.cfg -c "program $(TARGET) verify reset exit"

clean:
	rm -f *.o *.elf *.map