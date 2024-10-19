#ifndef BSP_PWM_H
#define BSP_PWM_H

#include "imx6ull.h"

typedef enum PWM_FIFO_INT_LEVEL{
    PWM_FIFO_INIT_LEVEL_1 = 0,
    PWM_FIFO_INIT_LEVEL_2 = 1,
    PWM_FIFO_INIT_LEVEL_3 = 2,
    PWM_FIFO_INIT_LEVEL_4 = 3,
}PWM_FIFO_INT_LEVEL;

typedef enum PWM_CLOCK_SOURCE{
    PWM_CLOCK_SOURCE_OFF = 0,
    PWM_CLOCK_SOURCE_IPG = 1,
    PWM_CLOCK_SOURCE_IPG_HIGHFREQ = 2,
    PWM_CLOCK_SOURCE_IPG_32L = 3,
}PWM_CLOCK_SOURCE;

typedef enum PWM_INTERRUPT_SOURCE{
    PWM_INTERRUPT_SOURCE_FIE = 0X1,
    PWM_INTERRUPT_SOURCE_RIE = 0X2,
    PWM_INTERRUPT_SOURCE_CIE = 0X4,
}PWM_INTERRUPT_SOURCE;

void Pwm_init(PWM_Type *pwm_base, uint16_t period, uint16_t sample);
void Pwm_setFifoLevel(PWM_Type *pwm_base, PWM_FIFO_INT_LEVEL lv);
void Pwm_setClockPrescaler(PWM_Type *pwm_base, uint16_t prescale);
void Pwm_setClockSource(PWM_Type *pwm_base, PWM_CLOCK_SOURCE clksrc);
void Pwm_reset(PWM_Type *pwm_base);
void Pwm_setRepeat(PWM_Type *pwm_base, uint8_t rep);
void Pwm_enable(PWM_Type *pwm_base);
void Pwm_disable(PWM_Type *pwm_base);
uint8_t Pwm_getFifoStatu(PWM_Type *pwm_base);
uint8_t Pwm_getStatus(PWM_Type *pwm_base);
void Pwm_enableInterruptSource(PWM_Type *pwm_base, PWM_INTERRUPT_SOURCE irq);
void Pwm_disableInterruptSource(PWM_Type *pwm_base, PWM_INTERRUPT_SOURCE irq);
void Pwm_setPeriod(PWM_Type *pwm_base, uint16_t period);
void Pwm_setSample(PWM_Type *pwm_base, uint16_t sample);
uint16_t Pwm_getCounter(PWM_Type *pwm_base);

#endif // BSP_PWM_H
