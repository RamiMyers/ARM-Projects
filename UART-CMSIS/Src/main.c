#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include "uart.h"
#include "led.h"

int main(void)
{
    static const char str[] = "Hello, world!";
    size_t len = strlen(str);
    uartInit();
    tim2_1Hz_Init();
    ledInit();

    // Loop Forever
	for(;;) {
        // printf("%s", str);
        printf("%s\r\n", str);
        ledToggle();
        while (!(TIM2->SR & SR_UIF));
        TIM2->SR &= ~SR_UIF;
    }
}
