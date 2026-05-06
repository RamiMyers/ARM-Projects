#ifndef ILI9341_H
#define ILI9341_H

#include "stm32f4xx.h"
#include "spi.h"

static inline void ILI_Init();
static inline void ILI_GPIO_Setup();
static inline void ILI_Write(uint8_t cmd);

static inline void ILI_Init() {
    ILI_GPIO_Setup();
    SPI1_Init();
}

static inline void ILI_GPIO_Setup() {
    /*
    ILI Pins:
    - PA5: SCK
    - PA6: D/C (GPO)
    - PA7: MOSI
    - PB6: CS (GPO)
    - PC7: RES (GPO)
    */

   // Enable Ports A, B, and C
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;

   /* PA5: SCK */
   // Configure mode to AF (10)
   GPIOA->MODER &= ~GPIO_MODER_MODE5_0;
   GPIOA->MODER |=  GPIO_MODER_MODE5_1;

   // Configure alternate function to AF5 (0101)
   GPIOA->AFR[0] |=  GPIO_AFRL_AFRL5_0;
   GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL5_1;
   GPIOA->AFR[0] |=  GPIO_AFRL_AFRL5_2;
   GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL5_3;

   /* PA6: D/C (GPO) */
   // Configure mode to GPO (01)
   GPIOA->MODER |=  GPIO_MODER_MODE6_0;
   GPIOA->MODER &= ~GPIO_MODER_MODE6_1;

   /* PA7: MOSI */
   // Configure mode to AF (10)
   GPIOA->MODER &= ~GPIO_MODER_MODE7_0;
   GPIOA->MODER |=  GPIO_MODER_MODE7_1;

   // Configure alternate function to AF5 (0101)
   GPIOA->AFR[0] |=  GPIO_AFRL_AFRL7_0;
   GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL7_1;
   GPIOA->AFR[0] |=  GPIO_AFRL_AFRL7_2;
   GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL7_3;

   /* PB6: CS (GPO) */
   // Configure mode to GPO (01)
   GPIOB->MODER |=  GPIO_MODER_MODE6_0;
   GPIOB->MODER &= ~GPIO_MODER_MODE6_1;

   /* PC7: RES (GPO) */
   // Configure mode to GPO (01)
   GPIOC->MODER |=  GPIO_MODER_MODE7_0;
   GPIOC->MODER &= ~GPIO_MODER_MODE7_1;
}

static inline void ILI_Write(uint8_t cmd) {
    // Wait for BSY flag to reset
    while (SPI1->SR & SPI_SR_BSY);
    // Pull D/C (PA6) pin low for command mode
    GPIOA->ODR &= ~GPIO_ODR_OD6;
    // Send Command
    SPI1_WriteByte(cmd);
    // Wait for BSY flag to reset
    while (SPI1->SR & SPI_SR_BSY);
    // Pull D/C (PA6) pin high
    GPIOA->ODR |= GPIO_ODR_OD6;
}

#endif