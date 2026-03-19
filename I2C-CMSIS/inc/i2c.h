#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"

#define RCC_GPIOBEN (1U << 1)

#define PB8 (1U << 8)
#define PB9 (1U << 9)

#define PB8_BIT0 (1U << 16)
#define PB8_BIT1 (1U << 17)
#define PB9_BIT0 (1U << 18)
#define PB9_BIT1 (1U << 19)

#define AFRH8_BIT0 (1U << 0)
#define AFRH8_BIT1 (1U << 1)
#define AFRH8_BIT2 (1U << 2)
#define AFRH8_BIT3 (1U << 3)

#define AFRH9_BIT0 (1U << 4)
#define AFRH9_BIT1 (1U << 5)
#define AFRH9_BIT2 (1U << 6)
#define AFRH9_BIT3 (1U << 7)

#define RCC_I2C1EN (1U << 21)

#define I2C1_CLOCK_FREQ 0x10 /* 16 MHz */
#define I2C1_CCR        80   /* CCR = Peripheral Clock Freq. / (2 * SCL Freq.) = 16,000,000 / (2 * 100,000) = 80 */
#define I2C1_TRISE      0x11 /* TRISE = Peripheral Clock Freq. MHz + 1 = 17 */

#define I2C1_PE    (1U << 0)
#define I2C1_SWRST (1U << 15)
#define I2C1_BUSY  (1U << 1)
#define I2C1_START (1U << 8)    
#define I2C1_STOP  (1U << 9)    
#define I2C1_SB    (1U << 0)
#define I2C1_ADDR  (1U << 1)
#define I2C1_TXE   (1U << 7)
#define I2C1_RXNE  (1U << 6)
#define I2C1_ACK   (1U << 10)

void I2C1_Init(void) {
    // Enable RCC for GPIO Port of I2C1 Pins
    RCC->AHB1ENR |= RCC_GPIOBEN;

    // Set I2C1 Pins to Alternate Function Mode
    GPIOB->MODER &= ~PB8_BIT0;
    GPIOB->MODER |=  PB8_BIT1;

    GPIOB->MODER &= ~PB9_BIT0;
    GPIOB->MODER |=  PB9_BIT1;

    // Set I2C1 Pins to Open-Drain
    GPIOB->OTYPER |= PB8;
    GPIOB->OTYPER |= PB9;

    // Enable Pull-Up on I2C1 Pins
    GPIOB->PUPDR |=  PB8_BIT0;
    GPIOB->PUPDR &= ~PB8_BIT1;

    GPIOB->PUPDR |=  PB9_BIT0;
    GPIOB->PUPDR &= ~PB9_BIT1;

    // Set Alternate Function Settings for I2C1 Pins (AF4 corresponds to SCL & SDA for pins PB8 & PB9, respectively)
    GPIOB->AFR[1] &= ~AFRH8_BIT0;
    GPIOB->AFR[1] &= ~AFRH8_BIT1;
    GPIOB->AFR[1] |=  AFRH8_BIT2;
    GPIOB->AFR[1] &= ~AFRH8_BIT3;

    GPIOB->AFR[1] &= ~AFRH9_BIT0;
    GPIOB->AFR[1] &= ~AFRH9_BIT1;
    GPIOB->AFR[1] |=  AFRH9_BIT2;
    GPIOB->AFR[1] &= ~AFRH9_BIT3;

    // Enable RCC on I2C1
    RCC->APB1ENR |= RCC_I2C1EN;

    // Enter Reset Mode
    I2C1->CR1 |= I2C1_SWRST;

    // Exit Reset Mode
    I2C1->CR1 &= ~I2C1_SWRST;

    // Set I2C1 Clock Frequency to 16 MHz
    I2C1->CR2 = I2C1_CLOCK_FREQ;

    // Set I2C1 to Standard Mode - 100 kHz
    I2C1->CCR = I2C1_CCR;

    // Set Rise Time to 17
    I2C1->TRISE = I2C1_TRISE;

    // Enable I2C1
    I2C1->CR1 |= I2C1_PE;
}

void I2C1_ByteRead(uint8_t saddr, uint8_t maddr, char* data) {
    volatile uint32_t tmp;

    printf("Waiting for Bus\r\n");
    // Wait Until the Bus is Not Empty
    while (I2C1->SR2 & I2C1_BUSY);

    // Generate Start Bit
    I2C1->CR1 |= I2C1_START;

    printf("Waiting for Start Flag\r\n");
    // Wait Until Start Flag is Set
    while (!(I2C1->CR1 & I2C1_SB));

    // Set Slave Address + Write Bit (0)
    I2C1->DR = saddr;

    printf("Slave Address: 0x%X\r\n", saddr);
    printf("Waiting for Address Flag\r\n");
    // Wait Until Address Flag is Set
    while (!(I2C1->SR1 & I2C1_ADDR));

    // Clear Address Flag (reading the address flag automatically clears it)
    tmp = I2C1->SR1;

    // Send Memory Address
    I2C1->DR = maddr;

    printf("Waiting for Transmitter to be Empty\r\n");
    // Wait Until Transmitter Empty
    while (!(I2C1->SR1 & I2C1_TXE));

    // Generate Another Start Bit
    I2C1->CR1 |= I2C1_START;

    printf("Waiting for Restart Flag\r\n");
    // Wait Until Restart Flag is Set
    while (!(I2C1->CR1 & I2C1_SB));

    // Set Slave Address + Read Bit (1)
    I2C1->DR = saddr | 1;

    printf("Waiting for Address Flag (again) \r\n");
    // Wait Until Address Flag is Set
    while (!(I2C1->SR1 & I2C1_ADDR));

    // Disable ACK Bit to Prepare for Stop Bit
    I2C1->CR1 &= ~I2C1_ACK;

    // Clear Address Flag
    tmp = I2C1->SR1;

    // Generate Stop Bit
    I2C1->CR1 |= I2C1_STOP;

    printf("Waiting for Receiver to not be Empty\r\n");
    // Wait Until Receiver is Not Empty
    while (!(I2C1->SR1 & I2C1_RXNE));

    // Read the Data Register
    *data++ = I2C1->DR;

    printf("Read Data Register\r\n");
}

#endif