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
    uint8_t data = 0;
    uint16_t reasult[3];
    while(1)
    {
        status = !status;
        Rtc_getDate(&time);
        printf("[MSG]: %d年%d月%d日%d时%d分%d秒\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);

        Ap3216c_readChar(0x00, &data);
        printf("[INFO]: [REG 0X00] = %d\r\n", data);

        Ap3216c_readData(reasult);
        printf("[INFO]: 红外线强度(IR) = %d\r\n[INFO]: 光强度(ALS) = %d\r\n[INFO]: 接近距离(PS) = %d\r\n",reasult[0],reasult[1],reasult[2]);

        Led_switch(LED0, status);
        Delay_us(GPT2, 1000000);
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