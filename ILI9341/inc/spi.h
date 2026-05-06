#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"

static inline void SPI1_Init() {
    // Enable RCC for SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Set CPOL & CPHA to 0
    SPI1->CR1 &= ~SPI_CR1_CPOL;
    SPI1->CR1 &= ~SPI_CR1_CPHA;

    // Configure for Full-Duplex
    SPI1->CR1 &= ~SPI_CR1_RXONLY;

    // Configure for MSB First
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    // Enable Software Slave Management
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;

    // Configure MCU as Master
    SPI1->CR1 |= SPI_CR1_MSTR;

    // Configure for 8-Bit Data Frame Communication
    SPI1->CR1 &= ~SPI_CR1_DFF;

    // Enable SPI1
    SPI1->CR1 |= SPI_CR1_SPE;
}

static inline void SPI1_WriteByte(uint8_t data) {
    // Wait for TXE
    while (!(SPI1->SR & SPI_SR_TXE));
    // Send Data
    SPI1->DR = data;
}

#endif