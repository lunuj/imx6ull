#ifndef BSP_EPIT_H
#define BSP_EPIT_H

#include "imx6ull.h"

void Epit_init(EPIT_Type *epit, uint16_t div, uint32_t period, uint32_t cmp);
void Epit_setPeriod(EPIT_Type *epit, uint32_t period);
void Epit_enableIrq(EPIT_Type *epit);
void Epit_disableIrq(EPIT_Type *epit);
uint32_t Epit_getInterruptStatus(EPIT_Type *epit);
void Epit_clearInterruptStatus(EPIT_Type *epit);
void Epit_startCounter(EPIT_Type *epit);
void Epit_stopCounter(EPIT_Type *epit);

#endif // BSP_EPIT_H