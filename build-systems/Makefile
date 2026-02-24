CC = arm-none-eabi-gcc
CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11
LDFLAGS = -nostdlib -T stm32f446_ls.ld
TARGET = build-systems.elf

$(TARGET): main.o stm32f446_startup.o
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) $< -o $@

stm32f446_startup.o: stm32f446_startup.c
	$(CC) $(CFLAGS) $< -o $@

load:
	openocd -f board/st_nucleo_f4.cfg

clean:
	rm -f *.o *.elf