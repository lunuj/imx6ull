#include "main.h"


int main(void)
{
    Clk_enable();
    Led_init();
    Beep_init();

    int_init();     //必须先初始化中断之后再配置中断，在中断初始化中会将所有中断设置为默认中断
    Key_initFilter();

    uint8_t led_state = OFF;

    while(1)
    {
        led_state = !led_state;
        Led_switch(LED0, led_state);
        Delay_ms(500);
    }
    return 0;
}