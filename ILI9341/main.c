#include <stdio.h>
#include "systick.h"
#include "uart.h"
#include "ili9341.h"

int main(void) {
    const char str[] = "World!";

    USART2_Init();
    ILI_Init();

    while (1) {
        SYSTICK_MsecDelay(1000);
        printf("Hello, %s\r\n", str);
    }
    return 0;
}