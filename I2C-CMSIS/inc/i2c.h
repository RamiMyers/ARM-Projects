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

static inline void I2C1_Init(void) {
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
    I2C1->CR1 |= I2C_CR1_SWRST;

    // Exit Reset Mode
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    // Set I2C1 Clock Frequency to 16 MHz
    I2C1->CR2 = I2C1_CLOCK_FREQ;

    // Set I2C1 to Standard Mode - 100 kHz
    I2C1->CCR = I2C1_CCR;

    // Set Rise Time to 17
    I2C1->TRISE = I2C1_TRISE;

    // Enable I2C1
    I2C1->CR1 |= I2C_CR1_PE;
}

/* IMPLEMENTATION NOTES:
    - Controller receiving does not have to wait for TxE after SB generation 
*/
static inline void I2C1_Read(uint8_t saddr, uint8_t maddr, uint8_t n, uint8_t* buffer) { 
    volatile uint32_t tmp; 

    // Wait Until the Bus is Not Busy 
    while (I2C1->SR2 & I2C_SR2_BUSY); 

    // Generate Start Bit 
    I2C1->CR1 |= I2C_CR1_START; 

    // Wait Until Start Flag is Set 
    while (!(I2C1->SR1 & I2C_SR1_SB)); 

    // Clear Start Bit
    tmp = I2C1->SR1;

    // Set Slave Address + Write Bit (0) 
    I2C1->DR = saddr << 1; 

    // Wait Until Address Flag is Set 
    while (!(I2C1->SR1 & I2C_SR1_ADDR)); 

    // Clear Address Flag 
    tmp = I2C1->SR1; 
    tmp = I2C1->SR2; 

    // Wait Until Transmitter Empty 
    while (!(I2C1->SR1 & I2C_SR1_TXE)); 
    
    // Send Memory Address 
    I2C1->DR = maddr; 

    // Generate Restart
    I2C1->CR1 |= I2C_CR1_START; 
    
    // Wait Until Restart Flag is Set 
    while (!(I2C1->SR1 & I2C_SR1_SB)); 

    // Clear Start Bit
    tmp = I2C1->SR1;

    // Set Slave Address + Read Bit (1) 
    I2C1->DR = (saddr << 1) | 1; 
    
    // Wait Until Address Flag is Set 
    while (!(I2C1->SR1 & I2C_SR1_ADDR)); 
    
    // Clear Address Flag 
    tmp = I2C1->SR1; 
    tmp = I2C1->SR2; 
    
    // Send ACK Bit
    I2C1->CR1 |= I2C_CR1_ACK; 
    
    // Read Data Until n is 0 
    while (n > 0) { 
        if (n == 1) { 
            I2C1->CR1 &= ~I2C_CR1_ACK; 
            I2C1->CR1 |= I2C_CR1_STOP; 
        }        
        while (!(I2C1->SR1 & I2C_SR1_RXNE)); 
        *buffer++ = I2C1->DR; 
        n--; 
    } 
}

static inline void I2C1_ByteWrite(uint8_t saddr, uint8_t maddr, uint8_t data) {
    volatile uint8_t tmp;

    // Wait Until the Bus is Not Busy
    while (I2C1->SR2 & I2C_SR2_BUSY);

    // Generate Start Bit
    I2C1->CR1 |= I2C_CR1_START;

    // Wait Until Start Flag is Set
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // Set Slave Address + Write Bit (0)
    I2C1->DR = saddr << 1;

    // Wait Until Address Flag is Set
    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    // Clear Address Flag
    tmp = I2C1->SR1;
    tmp = I2C1->SR2;

    // Wait Until Transmitter Empty
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    // Send Memory Address
    I2C1->DR = maddr;

    // Wait Until Transmitter Empty
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    // Send Data
    I2C1->DR = data;

    // Wait for Byte Transfer Finished Flag
    while (!(I2C1->SR1 & I2C_SR1_BTF));

    // Generate Stop Bit
    I2C1->CR1 |= I2C_CR1_STOP;
}

#endif