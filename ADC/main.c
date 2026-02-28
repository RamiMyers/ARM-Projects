#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"

// TODO: Copy project into ADC-CMSIS, using vendor CMSIS package
// TODO: Change register flags to the convention: <periph>_<flag> as opposed to <register>_<flag> to prevent potential naming conflicts

int main(void)
{
    uint32_t value;

    uartInit();
    adcInit();
    tim2_1Hz_Init();

    // Loop Forever
	for(;;) {
        while (!(TIM2->SR & SR_UIF));
        TIM2->SR &= ~SR_UIF;

        adcStartConversion();
        value = adcRead();
        printf("Sensor Value: %u\r\n", value);
    }
}