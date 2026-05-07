#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"

static inline void SPI1_Init() {
    // Enable RCC for SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Disable SPI1
    SPI1->CR1 &= ~SPI_CR1_SPE;

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

static inline void SPI1_Write8(uint8_t data) {
    uint8_t temp;

    // Wait for TXE
    while (!(SPI1->SR & SPI_SR_TXE));
    // Send Data
    *(uint8_t*)&(SPI1->DR) = data;
    // Wait for TXE
    while (!(SPI1->SR & SPI_SR_TXE));
    // Wait for BSY Flag to Reset
    while (SPI1->SR & SPI_SR_BSY);

    // Clear OVR Flag
    temp = SPI1->DR;
    temp = SPI1->SR;
}

static inline void SPI1_Write16(uint16_t data) {
    SPI1_Write8((uint8_t)(data >> 8));
    SPI1_Write8((uint8_t)(data & 0x00FF));
}

// static inline void SPI1_Write16(uint16_t data) {
//     uint8_t temp;

//     // Wait for TXE
//     while (!(SPI1->SR & SPI_SR_TXE));
//     // Send Data
//     *(uint16_t*)&(SPI1->DR) = data;
//     // Wait for TXE
//     while (!(SPI1->SR & SPI_SR_TXE));
//     // Wait for BSY Flag to Reset
//     while (SPI1->SR & SPI_SR_BSY);

//     // Clear OVR Flag
//     temp = SPI1->DR;
//     temp = SPI1->SR;
// }

#endif