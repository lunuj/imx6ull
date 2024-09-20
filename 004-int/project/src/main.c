#include "main.h"

int main(void)
{
    clk_enable();
    led_init();
    beep_init();
    key_init();

    int i = 0;
    int key_value;
    uint8_t led_state = OFF;
    uint8_t beep_state = OFF;

    while(1)
    {
        key_value = key_getvalue();
        if (key_value != KEY_NONE)
        {
            switch (key_value)
            {
            case KEY0_VALUE:
                beep_state = ON;
                beep_switch(beep_state);
                break;
            
            default:
                beep_state = OFF;
                beep_switch(beep_state);
                break;
            }
        }else{
            beep_state = OFF;
            beep_switch(beep_state);
        }
        i++;
        if(i==50){
            i = 0;
            led_state = !led_state;
            led_switch(LED0, led_state);
        }
        delay(10);
    }
    return 0;
}