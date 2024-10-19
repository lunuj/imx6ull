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

uint16_t sample = 0;

void pwm1_irqhandler(unsigned int gcciar,void *userParam)
{
    uint8_t statu= Pwm_getStatus(PWM1);
    if ((statu >> 0) & 1)
    {
        printf("[INFO]: FE IRQ\r\n");
        Pwm_setSample(PWM1, sample++);
        Pwm_setSample(PWM1, sample++);
        Pwm_setSample(PWM1, sample++);
        Pwm_setSample(PWM1, sample++);
    }
    if ((statu >> 1) & 1)
    {
        printf("[INFO]: ROV IRQ\r\n");
    }
    if ((statu >> 2) & 1)
    {
        printf("[INFO]: CMP IRQ\r\n");
    }
    if((statu >> 3) & 1){
        printf("[INFO]: FWE IRQ\r\n");
    }
    // printf("Sample = %d\r\n", sample);
    sample = sample % 1000;
}

int main(void)
{
    Device_init();

    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_PWM1_OUT,0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_PWM1_OUT,0xB090);

    Pwm_init(PWM1, 1000, 500);

    Pwm_enableInterruptSource(PWM1, PWM_INTERRUPT_SOURCE_FIE);
    system_register_irqHandler(PWM1_IRQn, (system_irq_handler_t)pwm1_irqhandler,NULL);  //中断函数注册
    GIC_enableIrq(PWM1_IRQn);
    
    Pwm_enable(PWM1);

    static unsigned char status=OFF;
    struct rtc_datetime time;
    while(1)
    {
        status = !status;
        Rtc_getDate(&time);
        printf("[MSG]: %d年%d月%d日%d时%d分%d秒\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
        // Pwm_setSample(PWM1, sample);
        // sample = (sample + 200)%1000;
        // printf("Sample = %d\r\n", sample);
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