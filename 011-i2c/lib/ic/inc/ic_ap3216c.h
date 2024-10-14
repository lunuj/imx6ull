#ifndef IC_AP3216C_H
#define IC_AP3216C_H

#include "imx6ull.h"

#define AP3216C_I2C             I2C1
#define AP3216C_ADDRESS         0x1E

//寄存器地址
#define AP3216C_SYSTEMCONG      0x00
#define AP3216C_INTSTATUS       0x01
#define AP3216C_INTCLEAR        0x02
#define AP3216C_IRDATALOW       0x0A
#define AP3216C_IRDATAHIGH      0x0B
#define AP3216C_ALSDATALOW      0x0C
#define AP3216C_ALSDATAHIGH     0x0D
#define AP3216C_PSDATALOW       0x0E
#define AP3216C_PSDATAHIGH      0x0F

void Ap3216c_init();

void Ap3216c_writeChar(uint8_t reg, uint8_t data);

void Ap3216c_readChar(uint8_t reg, uint8_t * data);

void Ap3216c_readData(uint16_t reasult[3]);

void Ap3216c_dataAnaylsis(uint16_t reasult[3], uint8_t data[6]);

#endif // IC_AP3216C_H

