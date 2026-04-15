#ifndef ICM_SPI_H
#define ICM_SPI_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "spi.h"

#define WHO_AM_I         0x00
#define PWR_MGMT_1       0x06
#define ACCEL_CONFIG     0x20
#define ACCEL_DATA_START 0x2D
#define REG_BANK_SEL     0x7F

#define USER_BANK_0 (0x0U << 4)
#define USER_BANK_2 (0x2U << 4)

#define ACCEL_FS_2G          0b00
#define ACCEL_FCHOICE_ENABLE 0b1
#define ACCEL_CONFIG_VALUE   ((ACCEL_FS_2G << 1) | ACCEL_FCHOICE_ENABLE)
#define ACCEL_2G_SENSITIVITY 16384.0f

#define PWR_MGMT_1_VALUE 0x01

// To receive data, we must first transmit the address + the read bit (MSB), and then call the receive function
static inline void ICM_Write(uint8_t addr, uint8_t value);
static inline void ICM_Read(uint8_t addr, uint8_t* buffer, uint32_t n);

static inline void ICM_Init() {
    uint8_t data[] = { PWR_MGMT_1_VALUE, USER_BANK_2, ACCEL_CONFIG_VALUE, USER_BANK_0 };
    SPI1_Init();
    printf("Enabled SPI1\r\n");
    ICM_Write(PWR_MGMT_1, PWR_MGMT_1_VALUE);
    printf("Set Power Management\r\n");
    ICM_Write(REG_BANK_SEL, USER_BANK_2);
    printf("Changed Banks\r\n");
    ICM_Write(ACCEL_CONFIG, ACCEL_CONFIG_VALUE);
    printf("Set Accel Config\r\n");
    ICM_Write(REG_BANK_SEL, USER_BANK_0);
    printf("Changed Banks\r\n");
}

static inline void ICM_Write(uint8_t addr, uint8_t value) {
    uint8_t data[2];

    addr &= ~0x80;
    data[0] = addr;
    data[1] = value;
    printf("addr: 0x%.2X\r\n", addr);

    SPI1_CS_Enable();
    printf("Enabled CS\r\n");
    SPI1_Transmit(data, 2);
    printf("Transmitted Data\r\n");
    SPI1_CS_Disable();
}

static inline void ICM_Read(uint8_t addr, uint8_t* buffer, uint32_t n) {
    addr |= 0x80;
    SPI1_Transmit(&addr, 1);
    SPI1_Receive(buffer, n);
}

#endif