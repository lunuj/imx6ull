#ifndef BSP_I2C_H
#define BSP_I2C_H

#include "imx6ull.h"
void I2c_init(I2C_Type *i2c_base);

uint8_t I2C_isBusBusy(I2C_Type *i2c_base);

void I2C_sendStart(I2C_Type *i2c_base, uint8_t address);

uint8_t I2C_sendStop(I2C_Type *i2c_base);

void I2C_sendRestart(I2C_Type *i2c_base, uint8_t address);

uint8_t I2C_getStatus(I2C_Type *i2c_base);

void I2C_clearInterruptStatus(I2C_Type *i2c_base);

uint8_t I2C_getInterruptStatus(I2C_Type *i2c_base);

uint8_t I2C_write(I2C_Type *i2c_base, uint8_t *buf, uint8_t len);

uint8_t I2C_read(I2C_Type *i2c_base, uint8_t *buf, uint8_t len);

#endif // BSP_I2C_H

