#include <stdint.h>
#include "gpio.h"


int main(void)
{
    ledInit();

    // Loop Forever
	for(;;) {
        ledToggle();
        for (volatile uint32_t i = 0; i < 0x3FFFF; i++) { }
    }
}
