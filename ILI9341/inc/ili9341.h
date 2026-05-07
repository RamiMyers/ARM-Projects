#ifndef ILI9341_H
#define ILI9341_H

#include "stm32f4xx.h"
#include "spi.h"
#include "systick.h"

static inline void ILI_Init();
static inline void ILI_GPIO_Setup();
static inline void ILI_CS_Enable();
static inline void ILI_CS_Disable();
static inline void ILI_SendCmd(uint8_t cmd);
static inline void ILI_Write8(uint8_t data);
static inline void ILI_Write16(uint16_t data);

static inline void ILI_Init() {
    ILI_GPIO_Setup();

    // Pull RES (PC7) Low
    SYSTICK_MsecDelay(100);
    GPIOC->ODR &= ~GPIO_ODR_OD7;

    // Pull RES (PC7) High
    SYSTICK_MsecDelay(100);
    GPIOC->ODR |= GPIO_ODR_OD7;

    // Initialize SPI1
    SYSTICK_MsecDelay(100);
    SPI1_Init();

    // Software Reset
    SYSTICK_MsecDelay(100);
    ILI_SendCmd(0x01);

    ILI_SendCmd(0xEF);
    ILI_Write8(0x03);
    ILI_Write8(0x80);
    ILI_Write8(0x02);
    ILI_SendCmd(0xCF);
    ILI_Write8(0x00);
    ILI_Write8(0xC1);
    ILI_Write8(0x30);
    ILI_SendCmd(0xED);
    ILI_Write8(0x64);
    ILI_Write8(0x03);
    ILI_Write8(0x12);
    ILI_Write8(0x81);
    ILI_SendCmd(0xE8);
    ILI_Write8(0x85);
    ILI_Write8(0x00);
    ILI_Write8(0x78);
    ILI_SendCmd(0xCB);
    ILI_Write8(0x39);
    ILI_Write8(0x2C);
    ILI_Write8(0x00);
    ILI_Write8(0x34);
    ILI_Write8(0x02);
    ILI_SendCmd(0xF7);
    ILI_Write8(0x20);
    ILI_SendCmd(0xEA);
    ILI_Write8(0x00);
    ILI_Write8(0x00);

    // PWCTR1
    ILI_SendCmd(0xC0);
    ILI_Write8(0x23);
    // PWCTR2
    ILI_SendCmd(0xC1);
    ILI_Write8(0x10);
    // VMCTR1
    ILI_SendCmd(0xC5);
    ILI_Write8(0x3E);
    ILI_Write8(0x28);
    // VMCTR2
    ILI_SendCmd(0xC7);
    ILI_Write8(0x86);
    // MADCTL
    ILI_SendCmd(0x36);
    ILI_Write8(0x48);
    // VSCRSADD
    ILI_SendCmd(0x37);
    ILI_Write8(0x00);
    // PIXFMT
    ILI_SendCmd(0x3A);
    ILI_Write8(0x55);
    // FRMCTR1
    ILI_SendCmd(0xB1);
    ILI_Write8(0x00);
    ILI_Write8(0x18);
    // DFUNCTR
    ILI_SendCmd(0xB6);
    ILI_Write8(0x08);
    ILI_Write8(0x82);
    ILI_Write8(0x27);
    ILI_SendCmd(0xF2);
    ILI_Write8(0x00);
    // GAMMASET
    ILI_SendCmd(0x26);
    ILI_Write8(0x01);
    // (Actual gamma settings)
    ILI_SendCmd(0xE0);
    ILI_Write8(0x0F);
    ILI_Write8(0x31);
    ILI_Write8(0x2B);
    ILI_Write8(0x0C);
    ILI_Write8(0x0E);
    ILI_Write8(0x08);
    ILI_Write8(0x4E);
    ILI_Write8(0xF1);
    ILI_Write8(0x37);
    ILI_Write8(0x07);
    ILI_Write8(0x10);
    ILI_Write8(0x03);
    ILI_Write8(0x0E);
    ILI_Write8(0x09);
    ILI_Write8(0x00);
    ILI_SendCmd(0xE1);
    ILI_Write8(0x00);
    ILI_Write8(0x0E);
    ILI_Write8(0x14);
    ILI_Write8(0x03);
    ILI_Write8(0x11);
    ILI_Write8(0x07);
    ILI_Write8(0x31);
    ILI_Write8(0xC1);
    ILI_Write8(0x48);
    ILI_Write8(0x08);
    ILI_Write8(0x0F);
    ILI_Write8(0x0C);
    ILI_Write8(0x31);
    ILI_Write8(0x36);
    ILI_Write8(0x0F);

    // Exit sleep mode.
    ILI_SendCmd(0x11);
    SYSTICK_MsecDelay(100);
    // Display on.
    ILI_SendCmd(0x29);
    SYSTICK_MsecDelay(10);
    // 'Normal' display mode.
    ILI_SendCmd(0x13);
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

static inline void ILI_CS_Enable() {
    // Pull CS (PB6) Low
    GPIOB->ODR &= ~GPIO_ODR_OD6;
}

static inline void ILI_CS_Disable() {
    // Pull CS (PB6) High
    GPIOB->ODR |= GPIO_ODR_OD6;
}

static inline void ILI_SendCmd(uint8_t cmd) {
    // Enable CS
    ILI_CS_Enable();

    // Pull D/C (PA6) pin low for command mode
    GPIOA->ODR &= ~GPIO_ODR_OD6;

    // Send Command
    SPI1_Write8(cmd);

    // Pull D/C (PA6) pin high
    GPIOA->ODR |= GPIO_ODR_OD6;

    // Disable CS
    ILI_CS_Disable();
}

static inline void ILI_Write8(uint8_t data) {
    // Enable CS
    ILI_CS_Enable();

    // Pull D/C (PA6) pin high
    GPIOA->ODR |= GPIO_ODR_OD6;

    // Write Data
    SPI1_Write8(data);

    // Disable CS
    ILI_CS_Disable();
}

static inline void ILI_Write16(uint16_t data) {
    // Enable CS
    ILI_CS_Enable();

    // Pull D/C (PA6) pin high
    GPIOA->ODR |= GPIO_ODR_OD6;

    // Write Data
    SPI1_Write16(data);

    // Disable CS
    ILI_CS_Disable();
}

#endif