#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"
#include "led.h"
#include "button.h"

int main(void)
{
    char opt;

    ledInit();
    buttonInit();
    uartInit();

    // Loop Forever
	for(;;) {
        printf("Enter an Option:\r\n");
        printf("1. Toggle On-Board LED\r\n");
        printf("2. Print User-Button State\r\n");
        opt = uartRead();
        switch (opt) {
            case '1':
                ledToggle();
                break;
            case '2':
                printf("%s\r\n", buttonGet() ? "PRESSED" : "RELEASED");
                break;
        }
    }
}