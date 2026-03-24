#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "icm-20948.h"
#include "systick.h"

// TODO: Verify acceleration scaling with ChatGPT
// TODO: Experiment with accel. offset registers

int main(void)
{
    uint8_t data;
    uint8_t accelX_High, accelX_Low;
    int16_t rawAccelX;
    float accelX;

    ICM_Init();
    USART2_Init();

    // Loop Forever
    printf("Starting Loop\r\n");
	for(;;) {
        SYSTICK_MsecDelay(100);
        data = ICM_ReadRegister(WHO_AM_I);
        accelX_High = ICM_ReadRegister(ACCEL_XOUT_H);
        accelX_Low = ICM_ReadRegister(ACCEL_XOUT_L);
        rawAccelX = (int16_t)(accelX_High << 8) | accelX_Low;
        accelX = rawAccelX / 16384.0f;
        printf("0x%X\r\n", data);
        printf("X Acceleration: %f g\r\n", accelX);
    }
}