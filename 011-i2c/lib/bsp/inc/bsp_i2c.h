#ifndef BSP_I2C_H
#define BSP_I2C_H

#include "imx6ull.h"

#define I2C_STATUS_OK                 (0)
#define I2C_STATUS_BUSY               (1)
#define I2C_STATUS_IDLE               (2)
#define I2C_STATUS_NAK                (3)
#define I2C_STATUS_ARBITRATIONLOST    (4)
#define I2C_STATUS_TIMEOUT            (5)
#define I2C_STATUS_ADDRNAK            (6)

void I2c_init(I2C_Type *i2c_base);
uint8_t I2c_isBusBusy(I2C_Type *i2c_base);
uint8_t I2c_isTransmitOver(I2C_Type *i2c_base);
uint8_t I2c_sendStart(I2C_Type *i2c_base, uint8_t address);
uint8_t I2c_sendStop(I2C_Type *i2c_base);
uint8_t I2c_sendRestart(I2C_Type *i2c_base, uint8_t address);
uint8_t I2c_getStatus(I2C_Type *i2c_base);
void I2c_clearInterruptStatus(I2C_Type *i2c_base);
uint8_t I2c_getInterruptStatus(I2C_Type *i2c_base);
uint8_t I2c_write(I2C_Type *i2c_base, uint8_t *buf, uint8_t len);
uint8_t I2c_read(I2C_Type *i2c_base, uint8_t *buf, uint8_t len);

#endif // BSP_I2C_H

