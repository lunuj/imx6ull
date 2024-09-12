#include "main.h"

int main(void)
{
    clk_enable();
    led_init();
    beep_init();

    while(1)
    {
        led_switch(LED0,ON);
        beep_switch(ON);
        delay(200);
        led_switch(LED0,OFF);
        beep_switch(OFF);
        delay(200);

    }
    return 0;
}