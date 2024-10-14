#include "ic_ap3216c.h"
#include "bsp_i2c.h"
#include "bsp_printf.h"
#include "bsp_delay.h"

void Ap3216c_init()
{
    /*IO初始化*/
    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL,1);
    IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL,0x70B0);

    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA,1);
    IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA,0x70B0);

    /*I2C初始化*/
    I2c_init(I2C1);

    /*芯片初始化*/
    uint8_t value=0;
    Ap3216c_writeChar(AP3216C_SYSTEMCONG, 0x4);      //软复位
    Delay_ms(GPT1, 50);
    Ap3216c_writeChar(AP3216C_SYSTEMCONG,0x3);      //使能ir、ps、als
    Ap3216c_readChar(AP3216C_SYSTEMCONG, &value);
    printf("ap3216c systemconfig reg = %d\r\n",value);
}

void Ap3216c_writeChar(uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = {reg, data};
    while(I2C_isBusBusy(AP3216C_I2C));
    I2C_sendStart(AP3216C_I2C, AP3216C_ADDRESS);
    I2C_write(AP3216C_I2C, buf, 2);
    I2C_sendStop(AP3216C_I2C);
}

void Ap3216c_readChar(uint8_t reg, uint8_t * data)
{
    while(I2C_isBusBusy(AP3216C_I2C));
    I2C_sendStart(AP3216C_I2C, AP3216C_ADDRESS);
    I2C_write(AP3216C_I2C, &reg, 1);
    I2C_sendRestart(AP3216C_I2C, AP3216C_ADDRESS);
    I2C_read(AP3216C_I2C, data, 1);
    I2C_sendStop(AP3216C_I2C);
}

void Ap3216c_readData(uint16_t reasult[3])
{
    uint8_t data[6];
    while(I2C_isBusBusy(AP3216C_I2C));
    I2C_sendStart(AP3216C_I2C, AP3216C_ADDRESS);
    uint8_t reg = AP3216C_IRDATALOW;
    I2C_write(AP3216C_I2C, &reg, 1);
    I2C_sendRestart(AP3216C_I2C, AP3216C_ADDRESS);
    I2C_read(AP3216C_I2C, data, 6);
    I2C_sendStop(AP3216C_I2C);
    Ap3216c_dataAnaylsis(reasult, data);
}

void Ap3216c_dataAnaylsis(uint16_t reasult[3], uint8_t data[6])
{
    if (data[0] &= 0x80)
    { // buf[0][bit7]==1时，IR、PS数据无效
        reasult[0] = 0;
        reasult[1] = 0;
    }
    else
    {
        /*ir数据10bit buf[1]高8位，buf[0][bit1:0]为低2位*/
        reasult[0] = ((unsigned short)data[1] << 2) | (data[0] & 0x03); // 与0x03,只保留低bit[1:0]
        /*ps数据10bit buf[5][bit5:0]高6位，buf[4][bit3:0]为低4位*/
        reasult[1] = (((unsigned short)data[5] & 0x3F) <<4) | (data[4] & 0x0F); //与0x3F保留bit[5:0],与0x0F保留bit[4:0]
    }
    reasult[2] = (unsigned short)data[3] << 8 | data[2];
}