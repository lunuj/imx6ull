#include "main.h"

int main(void)
{
    Clk_enable();
    Led_init();
    Uart_init(UART1, 115200);
    Uart_enable(UART1);    

    int_init();     //必须先初始化中断之后再配置中断，在中断初始化中会将所有中断设置为默认中断

    Delay_init(GPT2);
    static unsigned char status2=OFF;
    while(1)
    {
        status2 = !status2;
        Uart_putChar(UART1, 'a');
        Led_switch(LED0, status2);
        Delay_us(GPT2, 500000);
    }
    return 0;
}

void raise(int sig_nr) 
{
    while(1);
}