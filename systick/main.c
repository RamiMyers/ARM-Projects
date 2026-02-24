#include <stdint.h>
#include "led.h"
#include "systick.h"

int main(void)
{
    ledInit();

    // Loop Forever
	for(;;) {
        ledToggle();        
        systickMsecDelay(1000);
    }
}
