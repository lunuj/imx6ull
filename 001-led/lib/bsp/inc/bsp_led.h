#ifndef BSP_LED_H
#define BSP_LED_H

#include "imx6ull.h"

#define LED0    0

void led_init(void);
void led_on(void);
void led_off(void);
void led_switch(int led, int status);
    

#endif // BSP_LED_H