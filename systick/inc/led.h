#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"

#define PA5 (1U << 5)
#define LED_PIN PA5

#define MODER_PA5_BIT0 (1U << 10)
#define MODER_PA5_BIT1 (1U << 11)

#define BSRR_BS5 (1U << 5)
#define BSRR_BR5 (1U << 21)

#define RCC_GPIOAEN (1U << 0)

static inline void ledInit(void) {
    // Enable RCC for Port A
    RCC->AHB1ENR |= RCC_GPIOAEN;

    // Set MODER of GPIOA to 01 for general-purpose output
    GPIOA->MODER |= MODER_PA5_BIT0;
    GPIOA->MODER &= ~MODER_PA5_BIT1;
}

static inline void ledSet(void) {
    GPIOA->BSRR |= BSRR_BS5;
}

static inline void ledReset(void) {
    GPIOA->BSRR |= BSRR_BR5;
}

static inline void ledToggle(void) {
    if (GPIOA->ODR & LED_PIN)
        ledReset();
    else
        ledSet();
}

#endif