#ifndef UART_H
#define UART_H

#include "stm32f4xx.h"

#define SYSTEM_CLOCK_FREQ 16000000UL
#define BAUD_RATE         115200UL

#define UART_RE   (1U << 2)
#define UART_TE   (1U << 3)
#define UART_UE   (1U << 13)
#define UART_RXNE (1U << 5)
#define UART_TXE  (1U << 7)

#define RCC_GPIOA (1U << 0)
#define RCC_USART2 (1U << 17)

#define MODER_PA2_BIT0 (1U << 4)
#define MODER_PA2_BIT1 (1U << 5)
#define AFRL_PA2_BIT0  (1U << 8)
#define AFRL_PA2_BIT1  (1U << 9)
#define AFRL_PA2_BIT2  (1U << 10)
#define AFRL_PA2_BIT3  (1U << 11)

#define MODER_PA3_BIT0 (1U << 6)
#define MODER_PA3_BIT1 (1U << 7)
#define AFRL_PA3_BIT0  (1U << 12)
#define AFRL_PA3_BIT1  (1U << 13)
#define AFRL_PA3_BIT2  (1U << 14)
#define AFRL_PA3_BIT3  (1U << 15)

static inline void uartInit(void) {
    // Enable RCC for GPIOA
    RCC->AHB1ENR |= RCC_GPIOA;

    // Configure PA2 alternate function: USART2 TX
    GPIOA->MODER &= ~MODER_PA2_BIT0;
    GPIOA->MODER |= MODER_PA2_BIT1;
    GPIOA->AFR[0] |= AFRL_PA2_BIT0;
    GPIOA->AFR[0] |= AFRL_PA2_BIT1;
    GPIOA->AFR[0] |= AFRL_PA2_BIT2;
    GPIOA->AFR[0] &= ~AFRL_PA2_BIT3;

    // Configure PA3 alternate function: USART2 RX
    GPIOA->MODER &= ~MODER_PA3_BIT0;
    GPIOA->MODER |= MODER_PA3_BIT1;
    GPIOA->AFR[0] |= AFRL_PA3_BIT0;
    GPIOA->AFR[0] |= AFRL_PA3_BIT1;
    GPIOA->AFR[0] |= AFRL_PA3_BIT2;
    GPIOA->AFR[0] &= ~AFRL_PA3_BIT3;

    // Enable RCC for USART2
    RCC->APB1ENR |= RCC_USART2;

    // Calculate baud rate divisor
    /* round(y/x) = (y + (x/2)) / x */
    USART2->BRR = (SYSTEM_CLOCK_FREQ + (BAUD_RATE / 2U)) / BAUD_RATE;

    // Enable TX & RX 
    USART2->CR1 |= UART_RE;
    USART2->CR1 |= UART_TE;

    // Enalbe UART
    USART2->CR1 |= UART_UE;
}

static inline void uartWrite(uint8_t ch) {
    // Wait for SR to be empty
    while (!(USART2->SR & UART_TXE));
    USART2->DR = ch & 0xFF;
}

static inline uint8_t uartRead(void) {
    // Wait for data to arrive
    while (!(USART2->SR & UART_RXNE));
    return USART2->DR & 0xFF;
}

int __io_putchar(int ch) {
    uartWrite((uint8_t)ch);
    return ch;
}

#endif