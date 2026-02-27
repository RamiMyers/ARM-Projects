CC = arm-none-eabi-gcc
CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -D STM32F446xx -I inc -I inc/CMSIS/Include -I inc/CMSIS/Device/ST/STM32F4xx/Include
LDFLAGS = -T stm32f446xe_flash.ld -specs=nosys.specs
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

load:
	openocd -f board/st_nucleo_f4.cfg

clean:
	rm -f *.o *.elf *.map