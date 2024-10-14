#include "main.h"

void Device_init(){
    Imx6ull_clkInit();
    Clk_enable();

    Led_init();

    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX,0);//复用为UART1_TX 
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX,0x10b0);

    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX,0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX,0x10b0);
    
    Uart_init(UART1, 115200);
    Uart_enable(UART1);    

    int_init();     //必须先初始化中断之后再配置中断，在中断初始化中会将所有中断设置为默认中断

    Delay_init(GPT2);
    Rtc_init();
    Rtc_enable();

}

int main(void)
{
    Device_init();
    Ap3216c_init();

    static unsigned char status=OFF;
    struct rtc_datetime time;
    uint8_t test = 0,rx = 0;
    while(1)
    {
        status = !status;
        Rtc_getDate(&time);

        // uint16_t data[3];
        // Ap3216c_readData(data);
        // printf("ir = %d als = %d ps = %d\r\n", data[0], data[1], data[2]);
        Ap3216c_writeChar(0x00, test);
        Ap3216c_readChar(0x00, &rx);
        printf("rx = %d\r\ntest = %d\r\n", rx, test++);
        printf("time %d年%d月%d日%d时%d分%d秒\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
        Led_switch(LED0, status);
        Delay_us(GPT2, 500000);
    }
    return 0;
}

/**
 * @brief  异常处理函数
 * @param  sig_nr 暂时未知
 * @retval 无
 * @note 负责处理除数为0的异常情况
 */
void raise(int sig_nr) 
{
    while(1);
}