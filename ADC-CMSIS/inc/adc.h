#ifndef ADC_H
#define ADC_H

#include "stm32f4xx.h"

#define RCC_GPIOAEN (1U << 0)
#define RCC_ADC1EN  (1U << 8)

#define MODER_PA0_BIT0 (1U << 0)
#define MODER_PA0_BIT1 (1U << 1)

#define SQR3_SQ1_PA0 0x0
#define SQR1_CONVERSION_LEN 0x0

#define ADC_ADON    (1U << 0)
#define ADC_CONT    (1U << 1)
#define ADC_SWSTART (1U << 30)
#define ADC_EOC     (1U << 1)


void adc1_Init(void) {
    // Configure PA0 for Analaog Mode
    RCC->AHB1ENR |= RCC_GPIOAEN;

    GPIOA->MODER |= MODER_PA0_BIT0;
    GPIOA->MODER |= MODER_PA0_BIT1;

    // Enable RCC for ADC1
    RCC->APB2ENR |= RCC_ADC1EN;

    // Configure PA0 channel
    ADC1->SQR3 |= SQR3_SQ1_PA0;
    // Set conversion sequence length to 1
    ADC1->SQR1 = SQR1_CONVERSION_LEN;
    // Enable ADC
    ADC1->CR2 |= ADC_ADON;
}

void adcStartConversion(void) {
    // Configure ADC for discontinuous mode
    ADC1->CR2 &= ~ADC_CONT;
    // Start conversion
    ADC1->CR2 |= ADC_SWSTART;
}

uint32_t adcRead(void) {
    // Wait for end of conversion
    while (!(ADC1->SR & ADC_EOC));
    // Return value
    return (ADC1->DR);
}

#endif