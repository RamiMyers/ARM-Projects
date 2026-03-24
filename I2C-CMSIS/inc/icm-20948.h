#ifndef ICM_20948
#define ICM_20948

#include "i2c.h"

#define SLAVE_ADDR   0x69
#define WHO_AM_I     0x00
#define PWR_MGMT_1   0x06
#define ACCEL_CONFIG 0x20
#define ACCEL_XOUT_H 0x2D
#define ACCEL_XOUT_L 0x2E
#define REG_BANK_SEL 0x7F

#define ACCEL_FS_2G          0b00
#define ACCEL_FCHOICE_ENABLE 0b1
#define ACCEL_CONFIG_VALUE   ((ACCEL_FS_2G << 1) | ACCEL_FCHOICE_ENABLE)

#define PWR_MGMT_1_VALUE 0x01

void ICM_Init(void);
uint8_t ICM_ReadRegister(uint8_t maddr);
void ICM_WriteRegister(uint8_t maddr, uint8_t data);

void ICM_Init(void) {
    I2C1_Init();

    ICM_WriteRegister(PWR_MGMT_1, PWR_MGMT_1_VALUE);
    ICM_WriteRegister(REG_BANK_SEL, (0x2 << 4));
    ICM_WriteRegister(ACCEL_CONFIG, ACCEL_CONFIG_VALUE);
    ICM_WriteRegister(REG_BANK_SEL, (0x0 << 4));
}

uint8_t ICM_ReadRegister(uint8_t maddr) {
    return I2C1_ByteRead(SLAVE_ADDR, maddr);
}

void ICM_WriteRegister(uint8_t maddr, uint8_t data) {
    I2C1_ByteWrite(SLAVE_ADDR, maddr, data);
}

#endif