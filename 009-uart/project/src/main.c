#include "main.h"

int main(void)
{
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
    static unsigned char status2=OFF;
    uint32_t i = 0x12345;
    char str[6] = "lunuj\0";
    while(1)
    {
        status2 = !status2;
        printf("hello world %d %s %x\r\n", i, str, i);
        Led_switch(LED0, status2);
        i++;
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