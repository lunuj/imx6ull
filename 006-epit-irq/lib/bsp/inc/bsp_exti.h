#ifndef BSP_EXTI_H
#define BSP_EXTI_H

#include "imx6ull.h"
void Exti_init(void);
void Gpio_io18_irqhandler(uint16_t gicciar, void* param);

#endif // BSP_EXTI_H