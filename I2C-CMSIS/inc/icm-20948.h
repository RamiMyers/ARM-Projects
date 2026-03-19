#ifndef ICM_20948
#define ICM_20948

#include "i2c.h"

#define SLAVE_ADDRESS 0x68

void read_WHO_AM_I(char* data) {
    I2C1_ByteRead(SLAVE_ADDRESS, 0x00, data);
}

#endif