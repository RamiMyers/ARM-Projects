#include <stdint.h>
#include "timer.h"
#include "led.h"

int main(void)
{
    ledInit();
    tim2_1Hz_Init();

    // Loop Forever
	for(;;) {
        ledToggle();        
        while (!(TIM2->SR & SR_UIF));
        TIM2->SR &= ~SR_UIF;
    }
}
