#include "main.h"

void gpt1_irqhander(uint32_t gcciar, void * param){
    if ((Gpt_getInterruptStatus(GPT1) & GPT_IRQ_OUT1) == GPT_IRQ_OUT1)
    {
        static unsigned char status1=OFF;
        status1 = !status1;
        Beep_switch(status1);
    }
    Gpt_clearInterruptStatus(GPT1, GPT_IRQ_ALL);
}

int main(void)
{
    Clk_enable();
    Led_init();
    Beep_init();

    int_init();     //必须先初始化中断之后再配置中断，在中断初始化中会将所有中断设置为默认中断

    Gpt_init(GPT1, 65);
    Gpt_setCmp0(GPT1, 500000);
    system_register_irqHandler(GPT1_IRQn, gpt1_irqhander, NULL);
    Gpt_enableIrq(GPT1, GPT_IRQ_OUT1);
    GIC_enableIrq(GPT1_IRQn);
    Gpt_startCounter(GPT1);

    Delay_init(GPT2);
    static unsigned char status2=OFF;
    while(1)
    {
        status2 = !status2;
        Led_switch(LED0, status2);
        Delay_us(GPT2, 500000);
    }
    return 0;
}