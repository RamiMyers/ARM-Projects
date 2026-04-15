#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "systick.h"
#include "icm-20948-spi.h"

// TODO: Verify SPI1 config with ChatGPT

int main(void) {
    uint8_t rawAccel[6];
    uint16_t rawAccelX;

    USART2_Init();
    ICM_Init();
    printf("Init Peripherals\r\n");

    // Loop Forever
    printf("Starting Loop\r\n");
	for(;;) {
        SYSTICK_MsecDelay(100);
        printf("Delayed\r\n");
        // ICM_Read(ACCEL_DATA_START, rawAccel, 6);
        // rawAccelX = rawAccel[0] | (rawAccel[1] << 0x8U);
        // printf("Raw X Acceleration: %d\r\n", rawAccelX);
    }
}