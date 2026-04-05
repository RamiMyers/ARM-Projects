#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "icm-20948.h"
#include "systick.h"

// TODO: Refactor i2c.h to adhere to better macro-naming practices: <peripheral>_<register>_<bit>
// TODO: Experiment with burst read function: read 2 bytes, and then 1, to ensure that everything is working properly.

int main(void) {
    uint8_t data;
    uint8_t accelX_High, accelX_Low;
    int16_t rawAccelX, rawAccelY, rawAccelZ;
    float accelX, accelY, accelZ;
    uint8_t accBytes[6];

    ICM_Init();
    USART2_Init();

    // Loop Forever
    printf("Starting Loop\r\n");
	for(;;) {
        SYSTICK_MsecDelay(100);
        I2C1_BurstRead(SLAVE_ADDR, 0x2D, 6, accBytes);
        rawAccelX = (int16_t)(accBytes[0] << 8) | accBytes[1]; 
        rawAccelY = (int16_t)(accBytes[2] << 8) | accBytes[3]; 
        rawAccelZ = (int16_t)(accBytes[4] << 8) | accBytes[5]; 
        accelX = rawAccelX / 16384.0f;
        accelY = rawAccelY / 16384.0f;
        accelZ = rawAccelZ / 16384.0f;
        printf("X Acceleration: %f g\r\n", accelX);
        printf("Y Acceleration: %f g\r\n", accelY);
        printf("Z Acceleration: %f g\r\n", accelZ);
    }
}