#include "main.h"


int main(void)
{
    Clk_enable();
    Led_init();
    Beep_init();

    int_init();     //必须先初始化中断之后再配置中断，在中断初始化中会将所有中断设置为默认中断

    Epit_init(EPIT1, 0, 33000000, 16500000);
    system_register_irqHandler(EPIT1_IRQn, epit_irqhandler, EPIT1);
    Epit_enableIrq(EPIT1);
    GIC_enableIrq(EPIT1_IRQn);
    Epit_startCounter(EPIT1);

    uint8_t led_state = OFF;

    while(1)
    {
        led_state = !led_state;
        Led_switch(LED0, led_state);
        Delay_ms(500);
    }
    return 0;
}