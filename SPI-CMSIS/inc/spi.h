#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"
#include "stdio.h"

/*
SPI1 Pins:
    - PA5 (SCK)
    - PA6 (MISO)
    - PA7 (MOSI)
    - PB6 (CS)
*/

static inline void SPI1_Init(void);
static inline void SPI1_GPIO_Config(void);
static inline void SPI1_Config(void);
static inline void SPI1_CS_Enable(void);
static inline void SPI1_CS_Disable(void);
static inline void SPI1_Transmit(uint8_t* data, uint32_t n);

static inline void SPI1_Init(void) {
    SPI1_GPIO_Config();
    SPI1_Config();
}

static inline void SPI1_GPIO_Config(void) {
    // Enable RCC for GPIO ports A & B
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

    // Configure SPI1 GPIO pins for Alternate Function Mode (10)
    // PA5
    GPIOA->MODER &= ~GPIO_MODER_MODE5_0;
    GPIOA->MODER |=  GPIO_MODER_MODE5_1;

    // PA6
    GPIOA->MODER &= ~GPIO_MODER_MODE6_0;
    GPIOA->MODER |=  GPIO_MODER_MODE6_1;

    // PA7
    GPIOA->MODER &= ~GPIO_MODER_MODE7_0;
    GPIOA->MODER |=  GPIO_MODER_MODE7_1;

    // Set PB6 as an output pin to serve as CS
    GPIOB->MODER |=  GPIO_MODER_MODE6_0;
    GPIOB->MODER &= ~GPIO_MODER_MODE6_1;

    // Configure GPIO pins for AF5 (0101)
    // PA5 = SCK
    GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL5_0;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5_1;
    GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL5_2;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5_3;

    // PA6 = MISO
    GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL6_0;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL6_1;
    GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL6_2;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL6_3;

    // PA7 = MOSI
    GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL7_0;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL7_1;
    GPIOA->AFR[0] |=  GPIO_AFRL_AFSEL7_2;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL7_3;

}

static inline void SPI1_Config(void) {
    // Enable RCC for SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Set Baud Rate (16 MHz / 4 = 4 MHz; the ICM-20948 operates at a maximum of 7 MHz)
    SPI1->CR1 |= (1U << SPI_CR1_BR_Pos);

    // Set CPOL to 0 (the ICM-20948 samples data on the rising edge)
    SPI1->CR1 &= ~SPI_CR1_CPOL;

    // Set CPHA to 0 (the ICM-20948 shifts data on the falling edge)
    SPI1->CR1 &= ~SPI_CR1_CPHA;

    // Enable Full-Duplex Communication
    SPI1->CR1 &= ~SPI_CR1_RXONLY;

    // Send MSB First (as per ICM-20948 datasheet)
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    // Enable Software NSS Control
    SPI1->CR1 |= ~SPI_CR1_SSM;
    SPI1->CR1 |= ~SPI_CR1_SSI;

    // Configure MCU as Master
    SPI1->CR1 |= SPI_CR1_MSTR;

    // Configure 8-Bit Data Frame Format
    SPI1->CR1 &= ~SPI_CR1_DFF;

    // Enable Peripheral
    SPI1->CR1 |= SPI_CR1_SPE;
}

static inline void SPI1_CS_Enable(void) {
    // Pull NSS Pin Low
    GPIOB->ODR &= ~GPIO_ODR_OD6;
}

static inline void SPI1_CS_Disable(void) {
    // Pull NSS Pin High
    GPIOB->ODR |= GPIO_ODR_OD6;
}

static inline void SPI1_Transmit(uint8_t* data, uint32_t n) {
    uint8_t temp;

    for (uint32_t i = 0; i < n; i++) {
        printf("Iteration: %d\r\n", i);
        while (!(SPI1->SR & SPI_SR_TXE));
        printf("Transmitting Data\r\n");
        SPI1->DR = *data++;
        printf("Transmitted Data\r\n");
    }

    while (!(SPI1->SR & SPI_SR_TXE));
    printf("Transmitter Not Empty\r\n");
    while (SPI1->SR & SPI_SR_BSY);
    printf("SR Not Busy\r\n");

    temp = SPI1->DR;
    temp = SPI1->SR;
    printf("Cleard OVR Flag\r\n");
}

static inline void SPI1_Receive(uint8_t* buffer, uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        while (!(SPI1->SR & SPI_SR_RXNE));
        *buffer++ = SPI1->DR;
    }
}

#endif