#ifndef BSP_LED_H
#define BSP_LED_H

#include "imx6ull.h"

#define LED0    0

void Led_init(void);
void Led_on(void);
void Led_off(void);
void Led_switch(int led, int status);


#endif // BSP_LED_H

