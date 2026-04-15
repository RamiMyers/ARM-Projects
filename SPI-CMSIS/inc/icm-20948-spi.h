#ifndef ICM_SPI_H
#define ICM_SPI_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "spi.h"

#define WHO_AM_I         0x00
#define USER_CTRL        0x03
#define PWR_MGMT_1       0x06
#define ACCEL_CONFIG     0x14
#define ACCEL_DATA_START 0x2D
#define REG_BANK_SEL     0x7F

#define USER_BANK_0 (0x0U << 4)
#define USER_BANK_2 (0x2U << 4)

#define USER_CTRL_I2C_IF_DIS (0x1U << 4)

#define ACCEL_FS_2G          0b00U
#define ACCEL_FCHOICE_ENABLE 0b1U
#define ACCEL_CONFIG_VALUE   ((ACCEL_FS_2G << 1) | ACCEL_FCHOICE_ENABLE)
#define ACCEL_2G_SENSITIVITY 16384.0f

#define PWR_MGMT_1_VALUE 0x01U

static inline void ICM_Write(uint8_t addr, uint8_t value);
static inline void ICM_Read(uint8_t addr, uint8_t* buffer, uint32_t n);

uint8_t accelBytes[6];
int16_t rawAccel[3];

static inline void ICM_Init() {
    SPI1_Init();
    ICM_Write(USER_CTRL, USER_CTRL_I2C_IF_DIS);
    ICM_Write(PWR_MGMT_1, PWR_MGMT_1_VALUE);
    ICM_Write(REG_BANK_SEL, USER_BANK_2);
    ICM_Write(ACCEL_CONFIG, ACCEL_CONFIG_VALUE);
    ICM_Write(REG_BANK_SEL, USER_BANK_0);
}

static inline void ICM_Write(uint8_t addr, uint8_t value) {
    uint8_t data[2];

    addr &= ~0x80;
    data[0] = addr;
    data[1] = value;

    SPI1_CS_Enable();
    SPI1_Transmit(data, 2);
    SPI1_CS_Disable();
}

static inline void ICM_Read(uint8_t addr, uint8_t* buffer, uint32_t n) {
    addr |= 0x80;

    SPI1_CS_Enable();
    SPI1_Transmit(&addr, 1);
    SPI1_Receive(buffer, n);
    SPI1_CS_Disable();
}

static inline void ICM_ReadAccel(float* buffer) {
    ICM_Read(ACCEL_DATA_START, accelBytes, 6);

    rawAccel[0] = (accelBytes[0] << 8) | accelBytes[1];
    rawAccel[1] = (accelBytes[2] << 8) | accelBytes[3];
    rawAccel[2] = (accelBytes[4] << 8) | accelBytes[5];

    buffer[0] = rawAccel[0] / ACCEL_2G_SENSITIVITY;
    buffer[1] = rawAccel[1] / ACCEL_2G_SENSITIVITY;
    buffer[2] = rawAccel[2] / ACCEL_2G_SENSITIVITY;
}

#endif