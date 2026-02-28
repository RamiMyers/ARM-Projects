#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "timer.h"
#include "uart.h"

int main(void)
{
    char str[] = "Hello, world!\r\n";
    uartInit();
    tim2_1Hz_Init();

    size_t n = strlen(str);
    uartPrint("UART is alive!\r\n");
    uartPrint("str Len: ");
    uartWrite((uint8_t)('0' + (n / 10)));
    uartWrite((uint8_t)('0' + (n % 10)));
    uartPrint("\r\n");

    // Loop Forever
	for(;;) {
        while (!(TIM2->SR & SR_UIF));
        TIM2->SR &= ~SR_UIF;

        printf("Formatted string: %s", str);
        for (int i = 0; i < strlen(str); i++)
            uartWrite(str[i]);
    }
}