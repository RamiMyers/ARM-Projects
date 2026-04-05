#ifndef ICM_20948
#define ICM_20948

#include "i2c.h"

#define SLAVE_ADDR       0x69
#define WHO_AM_I         0x00
#define PWR_MGMT_1       0x06
#define ACCEL_CONFIG     0x20
#define ACCEL_DATA_START 0x2D
#define REG_BANK_SEL     0x7F

#define ACCEL_FS_2G          0b00
#define ACCEL_FCHOICE_ENABLE 0b1
#define ACCEL_CONFIG_VALUE   ((ACCEL_FS_2G << 1) | ACCEL_FCHOICE_ENABLE)
#define ACCEL_2G_SENSITIVITY 16384.0f

#define PWR_MGMT_1_VALUE 0x01

static inline void ICM_Init(void);
static inline void ICM_WriteRegister(uint8_t maddr, uint8_t data);
static inline void ICM_Read(uint8_t maddr, uint8_t n, uint8_t* buffer);
static inline void ICM_ReadAccel(float* buffer);

static uint8_t accelBytes[6];
static int16_t rawAccel[6];

static inline void ICM_Init(void) {
    I2C1_Init();

    ICM_WriteRegister(PWR_MGMT_1, PWR_MGMT_1_VALUE);
    ICM_WriteRegister(REG_BANK_SEL, (0x2U << 4));
    ICM_WriteRegister(ACCEL_CONFIG, ACCEL_CONFIG_VALUE);
    ICM_WriteRegister(REG_BANK_SEL, (0x0U << 4));
}

static inline void ICM_WriteRegister(uint8_t maddr, uint8_t data) {
    I2C1_ByteWrite(SLAVE_ADDR, maddr, data);
}

static inline void ICM_Read(uint8_t maddr, uint8_t n, uint8_t* buffer) {
    I2C1_Read(SLAVE_ADDR, maddr, n, buffer);
}

static inline void ICM_ReadAccel(float* buffer) {
    // Read high & low bytes of each acceleration axis
    ICM_Read(ACCEL_DATA_START, 6, accelBytes);

    // Combine high & low bytes of each axis to get raw values
    rawAccel[0] = (accelBytes[0] << 8) | accelBytes[1];
    rawAccel[1] = (accelBytes[2] << 8) | accelBytes[3];
    rawAccel[2] = (accelBytes[4] << 8) | accelBytes[5];

    // Convert raw values to g and store in final buffer
    buffer[0] = rawAccel[0] / ACCEL_2G_SENSITIVITY;
    buffer[1] = rawAccel[1] / ACCEL_2G_SENSITIVITY;
    buffer[2] = rawAccel[2] / ACCEL_2G_SENSITIVITY;
}

#endif