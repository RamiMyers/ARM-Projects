#ifndef UART_H
#define UART_H

#include "peripherals.h"

#define SYSTEM_CLOCK_FREQ 16000000UL
#define BAUD_RATE         115200UL

#define CR1_RE (1U << 2)
#define CR1_TE (1U << 3)
#define CR1_UE (1U << 13)

#define SR_RXNE (1U << 5)
#define SR_TXE  (1U << 7)

#define AHB1ENR_GPIOA (1U << 0)
#define APB1ENR_USART2 (1U << 17)

#define MODER_PA2_BIT0 (1U << 4)
#define MODER_PA2_BIT1 (1U << 5)
#define AFRL_PA2_BIT0  (1U << 8)
#define AFRL_PA2_BIT1  (1U << 9)
#define AFRL_PA2_BIT2  (1U << 10)
#define AFRL_PA2_BIT3  (1U << 11)

// TODO: Configure PA3 for AF7: USART2 RX
#define MODER_PA3_BIT0 (1U << 6)
#define MODER_PA3_BIT1 (1U << 7)
#define AFRL_PA3_BIT0  (1U << 12)
#define AFRL_PA3_BIT1  (1U << 13)
#define AFRL_PA3_BIT2  (1U << 14)
#define AFRL_PA3_BIT3  (1U << 15)

static inline void uartInit(void) {
    // Enable RCC for GPIOA
    RCC->AHB1ENR |= AHB1ENR_GPIOA;

    // Configure PA2 alternate function: USART2 TX
    GPIOA->MODER &= ~MODER_PA2_BIT0;
    GPIOA->MODER |= MODER_PA2_BIT1;
    GPIOA->AFRL |= AFRL_PA2_BIT0;
    GPIOA->AFRL |= AFRL_PA2_BIT1;
    GPIOA->AFRL |= AFRL_PA2_BIT2;
    GPIOA->AFRL &= ~AFRL_PA2_BIT3;

    // Configure PA3 alternate function: USART2 RX
    GPIOA->MODER &= ~MODER_PA3_BIT0;
    GPIOA->MODER |= MODER_PA3_BIT1;
    GPIOA->AFRL |= AFRL_PA3_BIT0;
    GPIOA->AFRL |= AFRL_PA3_BIT1;
    GPIOA->AFRL |= AFRL_PA3_BIT2;
    GPIOA->AFRL &= ~AFRL_PA3_BIT3;

    // Enable RCC for USART2
    RCC->APB1ENR |= APB1ENR_USART2;

    // Calculate baud rate divisor
    /* round(y/x) = (y + (x/2)) / x */
    USART2->BRR = (SYSTEM_CLOCK_FREQ + (BAUD_RATE / 2U)) / BAUD_RATE;

    // Enable TX & RX 
    USART2->CR1 |= CR1_RE;
    USART2->CR1 |= CR1_TE;

    // Enalbe UART
    USART2->CR1 |= CR1_UE;
}

static inline void uartWrite(uint8_t ch) {
    // Wait for SR to be empty
    while (!(USART2->SR & SR_TXE));
    USART2->DR = ch & 0xFF;
}

static inline uint8_t uartRead(void) {
    // Wait for data to arrive
    while (!(USART2->SR & SR_RXNE));
    return USART2->DR & 0xFF;
}

int __io_putchar(int ch) {
    uartWrite((uint8_t)ch);
    return ch;
}

#endif