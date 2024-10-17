#include "bsp_i2c.h"

/**
 * @brief  I2C模块初始化
 * @param  i2c_base I2C基址
 * @retval 无
 * @note 初始化波特率为100KHz
 */
void I2c_init(I2C_Type * i2c_base)
{
    i2c_base->I2CR &= ~(1<<7);
    i2c_base->IFDR = 0x15;
    i2c_base->I2CR |= 1<<7;
}

/**
 * @brief  判断I2C总线是否繁忙
 * @param  i2c_base I2C基址
 * @retval 1为总线繁忙 0为总线空闲
 * @note 
 */
uint8_t I2c_isBusBusy(I2C_Type * i2c_base)
{
    return ((i2c_base->I2SR >> 5) & 0x01);
}

uint8_t I2c_isTransmitOver(I2C_Type * i2c_base)
{
    return ((i2c_base->I2SR >> 7) & 0x01);
}

/**
 * @brief  发送I2C起始tiaojian
 * @param  i2c_base I2C基址
 * @param  address 从机地址
 * @retval I2C_STATUS_BUSY 表明总线繁忙 I2C_STATUS_OK 表明发送完成
 * @note 注意从机地址需包含读写位，处理方法为左移一位并或上读写位，读为1，写为0
 */
uint8_t I2c_sendStart(I2C_Type * i2c_base, uint8_t address){
    if(I2c_isBusBusy(i2c_base)){        //IBB，I2C忙标志位
        return I2C_STATUS_BUSY;         //IBB=1时，I2C忙，返回1
    } 
    i2c_base->I2CR |= (1<<5) | (1<<4);  //设置为主机发送
    i2c_base->I2DR = address;
    return I2C_STATUS_OK;
}

/**
 * @brief  发送停止条件
 * @param  i2c_base I2C基址
 * @retval I2C_STATUS_TIMEOUT：发送超时 I2C_STATUS_OK：发送完成
 * @note 
 */
uint8_t I2c_sendStop(I2C_Type * i2c_base){
    uint16_t timeout = 0xFFFF;
    i2c_base->I2CR &= ~(7<<3);          //MSTA=0 Slave模式、MTX=0 接收模式 TXAK=0 发送NoACK 
    while(I2c_isBusBusy(i2c_base)){
        timeout--;
        if (timeout==0)
        {
            return I2C_STATUS_TIMEOUT;
        }
    }
    return I2C_STATUS_OK;
}

/**
 * @brief  重新发送起始条件
 * @param  i2c_base I2C基址
 * @param  address 从机地址
 * @retval I2C_STATUS_IDLE：非主机模式 I2C_STATUS_BUSY：总线繁忙 I2C_STATUS_OK：发送完成
 * @note 从机地址需包含读写位，处理方法为左移一位并或上读写位，读为1，写为0
 */
uint8_t I2c_sendRestart(I2C_Type * i2c_base, uint8_t address)
{
    if ((i2c_base->I2CR & (1<<5)) == 0)
    {
        if (I2c_isBusBusy(i2c_base))
        {
            return I2C_STATUS_BUSY;
        }else{
            return I2C_STATUS_IDLE;
        }
    }
    i2c_base->I2CR |= (1<<4) | (1<<2);
    i2c_base->I2DR = address;
    return I2C_STATUS_OK;
}

/**
 * @brief  获取I2C状态
 * @param  i2c_base I2C基址
 * @retval I2C_STATUS_ARBITRATIONLOST：仲裁丢失 I2C_STATUS_NAK：无应答 I2C_STATUS_OK：发送完成
 * @note 
 */
uint8_t I2c_getStatus(I2C_Type * i2c_base)
{
    uint32_t state = i2c_base->I2SR;
    //先检查是否为仲裁丢失错误
    if(state & (1<<4))              //state值为I2SR，bit[4]：IAL仲裁丢失
    {
        i2c_base->I2SR &= ~(1<<4);      //清除IAL异常
        i2c_base->I2CR &= ~(1<<7);      //禁止I2C
        i2c_base->I2CR |= (1<<7);       //使能I2C
        return I2C_STATUS_ARBITRATIONLOST;  //抛出异常：仲裁丢失
    }
    else if(state & (1<<0))         //I2SR[0]RXAK 1时收到NoACK信号
    {
        return I2C_STATUS_NAK;      //抛出异常：无应答
    }
    return I2C_STATUS_OK;           //返回正常
}

/**
 * @brief  获取I2C中断状态
 * @param  i2c_base I2C基址
 * @retval 1：触发中断 0：未触发中断
 * @note 使用中断状态判断发送情况，需手动写0清除
 */
uint8_t I2c_getInterruptStatus(I2C_Type * i2c_base)
{
    return (i2c_base->I2SR & (1<<1));
}


/**
 * @brief  清除I2C中断状态
 * @param  i2c_base I2C基址
 * @retval 无
 * @note 
 */
void I2c_clearInterruptStatus(I2C_Type * i2c_base)
{
    i2c_base->I2SR &= ~(1<<1);
}

/**
 * @brief  I2C发送数据
 * @param  i2c_base I2C基址
 * @param  buf 指向I2C发送缓存的指针
 * @param  len 缓冲区大小
 * @retval 成功发送的数据个数
 * @note 此函数中使用中断状态判断是否发送完成
 */
uint8_t I2c_write(I2C_Type * i2c_base, uint8_t * buf, uint8_t len)
{
    uint8_t tx_len = 0;
    while(!(i2c_base->I2SR & (1<<7)));
    I2c_clearInterruptStatus(i2c_base);
    i2c_base->I2CR |= (1<<4);

    for (uint8_t i = 0; i < len; i++)
    {
        i2c_base->I2DR = *(buf+i);
        while(!I2c_getInterruptStatus(i2c_base));
        I2c_clearInterruptStatus(i2c_base);

        if (I2c_getStatus(i2c_base))
        {
            break;
        }else{
            tx_len++;
        }
    }
    I2c_clearInterruptStatus(i2c_base);
    return tx_len;
}

/**
 * @brief  I2C读取数据
 * @param  i2c_base I2C基址
 * @param  buf 指向读取缓冲区的指针
 * @param  len 缓冲区长度
 * @retval 读取的数据个数
 * @note 若传输数据过程中只有一个数据需要假读，以发送NACK，并在读取最后一个BIT前产生停止条件
 */
uint8_t I2c_read(I2C_Type * i2c_base, uint8_t * buf, uint8_t len)
{
    uint8_t rx_len = 0;
    while(!(i2c_base->I2SR & (1<<7)));
    I2c_clearInterruptStatus(i2c_base);

    i2c_base->I2CR &= ~((1<<4)|(1<<3));     //[4]MTX=0:接收数据 [3]TXAK=0 8位数据后加1位ACK响应

    if(len == 1){
        i2c_base->I2CR |= (1<<3);
    }
    rx_len = i2c_base->I2DR;
    rx_len = 0;

    for (uint8_t i = 0; i < len; i++)
    {
        while(!I2c_getInterruptStatus(i2c_base));
        I2c_clearInterruptStatus(i2c_base);

        if (i==1){
            i2c_base->I2CR |= (1<<3);
        }

        if (i==0){
            I2c_sendStop(i2c_base);
        }

        *(buf+i) = i2c_base->I2DR;
        rx_len++;
    }
    return rx_len;
}