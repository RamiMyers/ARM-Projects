#include <stdint.h>
#include "led.h"
#include "button.h"

int main(void)
{
    buttonInit();
    ledInit();

    // Loop Forever
	for(;;) {
        if (buttonGet())
            ledToggle();
        for (volatile uint32_t i = 0; i < 0x1FFFF; i++);
    }
}
