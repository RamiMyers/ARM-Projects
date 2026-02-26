#ifndef UART_H
#define UART_H

#include "peripherals.h"

#define CR1_TE (1U << 3)

#define AHB1ENR_GPIOA (1U << 0)
#define APB1ENR_UART4 (1U << 19)

#define MODER_PA0_BIT0 (1U << 0)
#define MODER_PA0_BIT1 (1U << 1)
#define AFRL_PA0_BIT0  (1U << 0)
#define AFRL_PA0_BIT1  (1U << 1)
#define AFRL_PA0_BIT2  (1U << 2)
#define AFRL_PA0_BIT3  (1U << 3)

void uart_init(void) {
    // Enable RCC for GPIOA
    RCC->AHB1ENR |= AHB1ENR_GPIOA;

    // Configure PA0 alternate function: UART4
    GPIOA->MODER &= ~MODER_PA0_BIT0;
    GPIOA->MODER |= MODER_PA0_BIT1;
    GPIOA->AFRL &= ~AFRL_PA0_BIT0;
    GPIOA->AFRL &= ~AFRL_PA0_BIT1;
    GPIOA->AFRL &= ~AFRL_PA0_BIT2;
    GPIOA->AFRL |= AFRL_PA0_BIT3;

    // Enable RCC for UART4
    RCC->APB1ENR |= APB1ENR_UART4;

    // TODO: Configure baud rate
    UART4->CR1 |= CR1_TE;
    // TODO: Enable UART
}

#endif