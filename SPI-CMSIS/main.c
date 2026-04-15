#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "systick.h"
#include "icm-20948-spi.h"

int main(void) {
    float accel[3];

    USART2_Init();
    ICM_Init();
    printf("Init Peripherals\r\n");

    // Loop Forever
    printf("Starting Loop\r\n");
	for(;;) {
        SYSTICK_MsecDelay(100);
        ICM_ReadAccel(accel);
        printf("X Acceleration: %f\r\n", accel[0]);
        printf("Y Acceleration: %f\r\n", accel[1]);
        printf("Z Acceleration: %f\r\n", accel[2]);
    }
}