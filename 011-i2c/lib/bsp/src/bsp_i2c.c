#include "bsp_i2c.h"

void I2c_init(I2C_Type * i2c_base)
{
    i2c_base->I2CR &= ~(1<<7);
    i2c_base->IFDR = 0x15;
    i2c_base->I2CR |= 1<<7;
}


uint8_t I2C_isBusBusy(I2C_Type * i2c_base)
{
    return (i2c_base->I2SR & (1<<5));
}

void I2C_sendStart(I2C_Type * i2c_base, uint8_t address){
    //设置为主机发送
    i2c_base->I2CR |= (1<<5) | (1<<4);
    i2c_base->I2DR = address;
}

uint8_t I2C_sendStop(I2C_Type * i2c_base){
    uint16_t timeout = 0xFFFF;
    i2c_base->I2CR &= ~(7<<3);
    while(I2C_isBusBusy(i2c_base)){
        timeout--;
        if (timeout==0)
        {
            return 0;
        }
    }
    return 1;
}

void I2C_sendRestart(I2C_Type * i2c_base, uint8_t address)
{
    i2c_base->I2CR |= (1<<4) | (1<<2);
    i2c_base->I2DR = address;
}

uint8_t I2C_getStatus(I2C_Type * i2c_base)
{
    uint32_t state = i2c_base->I2SR;
    if (state & (1<<4))
    {
        i2c_base->I2SR &= ~(1<<4);
        i2c_base->I2CR &= ~(1<<7);
        i2c_base->I2CR |= 1<<7;
        return 4;
    }
    if (state & (1<<0))
    {
        return 0;
    }
    return 1;
}

void I2C_clearInterruptStatus(I2C_Type * i2c_base)
{
    i2c_base->I2SR &= ~(1<<1);
}

uint8_t I2C_getInterruptStatus(I2C_Type * i2c_base)
{
    return (i2c_base->I2SR & (1<<1));
}


uint8_t I2C_write(I2C_Type * i2c_base, uint8_t * buf, uint8_t len)
{
    uint8_t i = 0;
    while(!(i2c_base->I2SR & (1<<7)));
    i2c_base->I2CR |= (1<<4);

    for (i = 0; i < len; i++)
    {
        i2c_base->I2DR = *(buf+i);
        while(!(i2c_base->I2SR & (1<<1)));
        I2C_clearInterruptStatus(i2c_base);

        if (I2C_getStatus(i2c_base) == 0)
        {
            break;
        }
    }
    return i;
}

uint8_t I2C_read(I2C_Type * i2c_base, uint8_t * buf, uint8_t len)
{
    uint8_t i = 0;
    while(!(i2c_base->I2SR & (1<<7)));
    i2c_base->I2CR &= ~((1<<4)|(1<<3));

    if(len == 1){
        i2c_base->I2CR |= (1<<3);
        i2c_base->I2DR;
    }

    for (i = 0; i < len; i++)
    {
        *(buf+i) = i2c_base->I2DR;
        while(!(i2c_base->I2SR & (1<<1)));
        I2C_clearInterruptStatus(i2c_base);

        if (i==1)
        {
            i2c_base->I2CR |= (1<<3);
        }
        

        if (I2C_getStatus(i2c_base) || i == len)
        {
            break;
        }
    }
    return i;
}