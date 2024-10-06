#include "bsp_uart.h"

/**
 * @brief  UART初始化
 * @param  uart UART基址
 * @param  baud 设置比特率
 * @retval 无
 * @note 默认UART时钟未80MHz
 */
void Uart_init(UART_Type * uart, uint32_t baud)
{
    uart->UCR1 &= ~(1<<0);
    uart->UCR2 &= ~(1<<0);                     //SRET=0
    while((uart->UCR2 & 0x01) == 0 );          //复位完毕，SRET=1
    /*配置UART1*/
    uart->UCR1 = 0;
    // UART1->UCR1 &= ~(1<<14);

    /*配置UCR2*/
    uart->UCR2 = 0;                                 //清除UCR0
    uart->UCR2 |= (1<<1) |(1<<2) |(1<<5)|(1<<14);   //从左起:RXEN=1 TXEN=1 WS=1 IRTS=1
                                                     //接收、发送使能、数据长度为8bit 忽略RTS引脚
    /*配置UCR3*/
    uart->UCR3 |= (1<<2);                           //RXDMUXSEL=1

    uart->UFCR &= ~(7<<7);
    uart->UFCR |= 5<<7;
    //波特率设置115200、
    // uart->UBIR = 71;
    // uart->UBMR = 3124;
    Uart_setBaudrate(uart, baud, 80000000);
}

void Uart_enable(UART_Type * uart)
{
    uart->UCR1 |= 1<<0;
}

void Uart_disable(UART_Type * uart)
{
    uart->UCR1 &= ~(1<<0);
}

void Uart_softReset(UART_Type * uart)
{
    uart->UCR2 &= ~(1<<0);                     //SRET=0
    while((uart->UCR2 & 0x01) == 0 );          //复位完毕，SRET=1
}

void Uart_putChar(UART_Type * uart, uint8_t c)
{
    while(((uart->USR2 >>3) & 0x01) == 0);     //等待前一个发送流程完毕
    uart->UTXD = (c & 0xFF);
}

uint8_t Uart_getChar(UART_Type * uart){
    while(((uart->USR2)&0x01) == 0);          //等待前一个接收流程完毕
    return uart->URXD;
}

void Uart_putString(UART_Type * uart, uint8_t * ptr, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        Uart_putChar(uart, *(ptr+i));
    }
}


/**
 * @brief                   设置比特率(官方代码)
 * @param base              UART结构特
 * @param baudrate          要设置的比特率
 * @param srcclock_hz       时钟源频率
 */
void Uart_setBaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u;        //分子
    uint32_t denominator = 0U;        //分母
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0)
    {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK))
    {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator)
        {
            numerator = 1;
        }
        if (0 == denominator)
        {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider)
    {
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0)
    {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3)
    {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1); //要先写UBIR寄存器，然后在写UBMR寄存器，3592页 
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }
}