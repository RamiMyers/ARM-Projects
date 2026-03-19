#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"

int main(void)
{
    uint32_t value;

    uartInit();
    adc1_Init();
    tim2_1Hz_Init();

    // Loop Forever
	for(;;) {
        while (!(TIM2->SR & TIM_UIF));
        TIM2->SR &= ~TIM_UIF;

        adcStartConversion();
        value = adcRead();
        printf("Sensor Value: %u\r\n", value);
    }
}