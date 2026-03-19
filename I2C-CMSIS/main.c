#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "icm-20948.h"

// TODO: Implement ICM-20948 driver to read WHO_AM_I register

int main(void)
{
    char* data = malloc(1);

    I2C1_Init();
    uartInit();
    tim2_1Hz_Init();

    // Loop Forever
	for(;;) {
        printf("Starting Loop\r\n");
        while (!(TIM2->SR & TIM_UIF));
        TIM2->SR &= ~TIM_UIF;
        printf("Before Read\r\n");
        read_WHO_AM_I(data);
        printf("0x%X\r\n", data);
        printf("After Read\r\n");
    }
}