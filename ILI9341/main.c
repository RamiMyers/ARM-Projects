#include <stdio.h>
#include "systick.h"
#include "uart.h"
#include "ili9341.h"

int main(void) {
    int iter = 0;
    int on = 0;

    USART2_Init();
    ILI_Init();

    ILI_SendCmd(0x2A);
    ILI_Write16(0x0000);
    ILI_Write16((uint16_t)239);

    ILI_SendCmd(0x2B);
    ILI_Write16(0x0000);
    ILI_Write16((uint16_t)319);

    ILI_SendCmd(0x2C);

    while (1) {
        for (iter = 0; iter < (240*320); ++iter) {
            if (on)
                ILI_Write16(0xF800);
            else
                ILI_Write16(0x001F);
        }
        on = !on;
    }
    return 0;
}