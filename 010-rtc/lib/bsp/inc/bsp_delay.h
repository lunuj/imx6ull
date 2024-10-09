#ifndef BSP_DELAY_H
#define BSP_DELAY_H

#include "imx6ull.h"

void Delay_init(GPT_Type *gpt);
void Delay_us(GPT_Type *gpt, uint32_t us);
void Delay_ms(GPT_Type *gpt, uint32_t ms);
void Delay_s(GPT_Type *gpt, uint32_t s);

#endif // BSP_DELAY_H

