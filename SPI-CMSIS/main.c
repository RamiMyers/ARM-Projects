#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "systick.h"
#include "spi.h"

int main(void) {

    SPI1_Init();
    USART2_Init();

    // Loop Forever
    printf("Starting Loop\r\n");
	for(;;) {
        SYSTICK_MsecDelay(100);
    }
}