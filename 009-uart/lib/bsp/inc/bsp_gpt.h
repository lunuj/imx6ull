#ifndef BSP_GPT_H
#define BSP_GPT_H

#include "imx6ull.h"

typedef enum {
    GPT_IRQ_OUT1 = 0x1,
    GPT_IRQ_OUT2 = 0x2,
    GPT_IRQ_OUT3 = 0x4,
    GPT_IRQ_IN1 = 0x8,
    GPT_IRQ_IN2 = 0x10,
    GPT_IRQ_ROVIE = 0x20,
    GPT_IRQ_ALL = 0x3f,
}GPT_IRQ_SOURCE;

void Gpt_init(GPT_Type *gpt, uint8_t div);
void Gpt_reset(GPT_Type *gpt);
void Gpt_setMode(GPT_Type *gpt, uint8_t mode);
void Gpt_setDiv(GPT_Type *gpt, uint16_t div);
void Gpt_setCmp0(GPT_Type *gpt, uint32_t period);
void Gpt_setCmp1(GPT_Type *gpt, uint32_t period);
void Gpt_seCmp2(GPT_Type *gpt, uint32_t period);
void Gpt_enableIrq(GPT_Type *gpt, GPT_IRQ_SOURCE irq);
void Gpt_disableIrq(GPT_Type *gpt, GPT_IRQ_SOURCE irq);
uint8_t Gpt_getInterruptStatus(GPT_Type *gpt);
void Gpt_clearInterruptStatus(GPT_Type *gpt, GPT_IRQ_SOURCE irq);
void Gpt_startCounter(GPT_Type *gpt);
void Gpt_stopCounter(GPT_Type *gpt);
uint32_t Gpt_readCounter(GPT_Type *gpt);

#endif // BSP_GPT_H

