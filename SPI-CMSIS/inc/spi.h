#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"

/*
SPI1 Pins:
    - PA5 (SCK)
    - PA6 (MISO)
    - PA7 (MOSI)
    - PB6 (CS)
*/

static inline void SPI1_Init(void) {
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

    // Enable RCC for SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}

#endif