#ifndef BSP_EPIT_H
#define BSP_EPIT_H

#include "imx6ull.h"

void Epit_init(EPIT_Type *epit, uint16_t div, uint32_t period, uint32_t cmp);
void Epit_enableIrq(EPIT_Type *epit);
void Epit_disableIrq(EPIT_Type *epit);
void Epit_startCounter(EPIT_Type *epit);
void Epit_stopCounter(EPIT_Type *epit);
void epit_irqhandler(uint32_t gcciar, void *param);

#endif // BSP_EPIT_H