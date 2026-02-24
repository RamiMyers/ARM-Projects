#include <stdint.h>
#include "stm32f4xx.h"

#define GPIOA_PA5    (1U << 5)
#define LED_PIN      GPIOA_PA5 

#define MODER_PA5_BIT0 (1U << 10)
#define MODER_PA5_BIT1 (1U << 11)

#define RCC_GPIOAEN (1U << 0)

int main(void)
{
    // Enable RCC for Port A
    RCC->AHB1ENR |= RCC_GPIOAEN;
    // Set MODER of GPIOA to 01 for general-purpose output
    GPIOA->MODER |= MODER_PA5_BIT0;
    GPIOA->MODER &= ~MODER_PA5_BIT1;

    // Loop Forever
	for(;;) {
        GPIOA->ODR ^= LED_PIN;
        for (volatile uint32_t i = 0; i < 0xEFFFF; i++) { }
    }
}
