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

    // /*芯片初始化*/
    // uint8_t value = 0xFF;
    Ap3216c_writeChar((AP3216C_SYSTEMCONG << 1) | 0, 0x4);      //软复位
    // Delay_ms(GPT1, 50);
    Ap3216c_writeChar((AP3216C_SYSTEMCONG << 1) | 0, 0x3);      //使能ir、ps、als
    // Ap3216c_readChar((AP3216C_SYSTEMCONG << 1) | 1, &value);
    // printf("AP3216C 0X00 = %d\r\n",value);
}

uint8_t Ap3216c_writeChar(uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = {reg, data};
    uint8_t ret = 0;

    while(!I2c_isTransmitOver(AP3216C_I2C));                    /* 等待传输完成 */
    I2c_clearInterruptStatus(AP3216C_I2C);
    I2c_sendStart(AP3216C_I2C, (AP3216C_ADDRESS << 1) | 0);
    
    while(!I2c_getInterruptStatus(AP3216C_I2C));
    ret = I2c_getStatus(AP3216C_I2C);                           /* 检查是否出现传输错误 */
    if(ret != I2C_STATUS_OK)
    {
        I2c_sendStop(AP3216C_I2C);
        return ret;
    }

    I2c_write(AP3216C_I2C, buf, 2);
    I2c_sendStop(AP3216C_I2C);

    return I2C_STATUS_OK;
}

/**
 * @brief  AP3216读取一个字节
 * @param  reg 读取的寄存器地址
 * @param  data 读取缓冲区
 * @retval 返回通信状态 I2C_STATUS_OK：读取成功
 * @note 具体流程为：1.发送写起始条件 2.发送要读取的寄存器地址，不产生停止位 3.重新产生读起始条件 4.读取一个字节并产生停止条件
 */
uint8_t Ap3216c_readChar(uint8_t reg, uint8_t * data)
{
    uint8_t ret = 0;

    while(!I2c_isTransmitOver(AP3216C_I2C));                    /* 等待传输完成 */
    I2c_clearInterruptStatus(AP3216C_I2C);
    I2c_sendStart(AP3216C_I2C, (AP3216C_ADDRESS << 1) | 0);
    
    while(!I2c_getInterruptStatus(AP3216C_I2C));
    ret = I2c_getStatus(AP3216C_I2C);                           /* 检查是否出现传输错误 */
    if(ret != I2C_STATUS_OK)
    {
        I2c_sendStop(AP3216C_I2C);
        return ret;
    }

    I2c_write(AP3216C_I2C, &reg, 1);
    I2c_clearInterruptStatus(AP3216C_I2C);                      /* 清除中断挂起位 */
    I2c_sendRestart(AP3216C_I2C, (AP3216C_ADDRESS<<1) | 1);     /* 发送重复开始信号和从机地址 */

    while(!I2c_getInterruptStatus(AP3216C_I2C));                /* 等待传输完成 */
    ret = I2c_getStatus(AP3216C_I2C);                           /* 检查是否有错误发生 */
    if(ret != I2C_STATUS_OK)
    {
        I2c_sendStop(AP3216C_I2C);
        return ret;  
    }                         

    I2c_read(AP3216C_I2C, data, 1);
    I2c_sendStop(AP3216C_I2C);
    return I2C_STATUS_OK;
}

/**
 * @brief  AP3216C获取数据分析
 * @param  reasult 分析结果
 * @param  data 原始数据
 * @retval 无
 * @note 分析结果存放数据分别为IR，ALS，PS
 */
void Ap3216c_dataAnaylsis(uint16_t reasult[3], uint8_t data[6])
{
    /*ir数据10bit buf[1]高8位，buf[0][bit1:0]为低2位*/
    reasult[0] = ((unsigned short)data[1] << 2) | (data[0] & 0x03); // 与0x03,只保留低bit[1:0]
    /*als数据10bit buf[3][bit5:0]高8位，buf[2][bit3:0]为低8位*/
    reasult[1] = (unsigned short)data[3] << 8 | data[2];
    /*ps数据10bit buf[5][bit5:0]高6位，buf[4][bit3:0]为低4位*/
    reasult[2] = (((unsigned short)data[5] & 0x3F) <<4) | (data[4] & 0x0F); //与0x3F保留bit[5:0],与0x0F保留bit[4:0]

    if (data[0] &= 0x80)        // buf[0][bit7]==1时，IR、PS数据无效
    {
        reasult[0] = 0;
        reasult[2] = 0;
    }
}

/**
 * @brief  读取并分析AP3216C采集结果
 * @param  reasult 分析结果
 * @retval 无
 * @note 分析结果存放数据分别为IR，ALS，PS
 */
void Ap3216c_readData(uint16_t reasult[3])
{
    uint8_t data[6];

    for(int i=0;i<6;i++){
        Ap3216c_readChar(AP3216C_IRDATALOW +i, data+i);
    }

    Ap3216c_dataAnaylsis(reasult, data);
}