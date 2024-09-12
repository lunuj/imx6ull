#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_clk.h"

int main(void)
{
    clk_enable();
    led_init();
    
    while(1)
    {
        led_switch(LED0,ON);
        delay(200);
        led_switch(LED0,OFF);
        delay(200);

    }
    return 0;
}