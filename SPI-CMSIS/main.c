#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "icm-20948.h"
#include "systick.h"

int main(void) {
    float accel[3];

    ICM_Init();
    USART2_Init();

    // Loop Forever
    printf("Starting Loop\r\n");
	for(;;) {
        SYSTICK_MsecDelay(100);
        ICM_ReadAccel(accel);
        printf("X Acceleration: %f g\r\n", accel[0]);
        printf("Y Acceleration: %f g\r\n", accel[1]);
        printf("Z Acceleration: %f g\r\n", accel[2]);
    }
}