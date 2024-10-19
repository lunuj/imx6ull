#include "bsp_pwm.h"

void Pwm_init(PWM_Type * pwm_base, uint16_t period, uint16_t sample)
{
    pwm_base->PWMCR = 0;
    Pwm_setFifoLevel(pwm_base, PWM_FIFO_INIT_LEVEL_2);
    Pwm_setClockSource(pwm_base, PWM_CLOCK_SOURCE_IPG);
    Pwm_setClockPrescaler(pwm_base, 65);
    Pwm_setPeriod(pwm_base, period);
    Pwm_setSample(pwm_base, sample);
}

/**
 * @brief  设置PWM的FIFO中断等级
 * @param  pwm_base PWM基址
 * @param  lv FIFO触发中断等级
 * @retval 无
 * @note 
 */
void Pwm_setFifoLevel(PWM_Type * pwm_base, PWM_FIFO_INT_LEVEL lv)
{
    pwm_base->PWMCR |= (lv << 26); 
}

/**
 * @brief  设置PWM时钟分频
 * @param  pwm_base 无
 * @param  prescale 分频值
 * @retval 无
 * @note 分频取值范围在0X000-0XFFF，具体值为prescale+1，最高为4096
 */
void Pwm_setClockPrescaler(PWM_Type * pwm_base, uint16_t prescale)
{
    pwm_base->PWMCR |= ((prescale & 0xfff) << 4);
}

/**
 * @brief  设置PWM时钟源
 * @param  pwm_base PWM基址
 * @param  clksrc 时钟源选择
 * @retval 无
 * @note 
 */
void Pwm_setClockSource(PWM_Type * pwm_base, PWM_CLOCK_SOURCE clksrc)
{
    pwm_base->PWMCR |= (clksrc << 16);
}

/**
 * @brief  复位PWM
 * @param  pwm_base PWM基址
 * @retval 无
 * @note 配置SWR为1，复位后自动清零
 */
void Pwm_reset(PWM_Type * pwm_base)
{
    pwm_base->PWMCR |= (1<<3);
    while(!(pwm_base->PWMCR & (1<<3)));
}

/**
 * @brief  Sample重复
 * @param  pwm_base PWM基址
 * @param  rep 重复次数
 * @retval 无
 * @note 重读次数为2^rep
 */
void Pwm_setRepeat(PWM_Type * pwm_base, uint8_t rep)
{
    pwm_base->PWMCR |= ((rep &3) << 1);
}

/**
 * @brief  PWM使能
 * @param  pwm_base PWM基址
 * @retval 无
 * @note 
 */
void Pwm_enable(PWM_Type * pwm_base)
{
    pwm_base->PWMCR |= (1<<0);
}

/**
 * @brief  PWM失能
 * @param  pwm_base PWM基址
 * @retval 无
 * @note 
 */
void Pwm_disable(PWM_Type * pwm_base)
{
    pwm_base->PWMCR &= ~(1<<0);
}


/**
 * @brief  获取FIFO状态
 * @param  pwm_base PWM基址
 * @retval 返回FIFO内元素数量
 * @note 3位有效，仅0-4有意义，表明FIFO内元素数量
 */
uint8_t Pwm_getFifoStatu(PWM_Type * pwm_base)
{
    return (pwm_base->PWMSR & 7);
}

/**
 * @brief  获取PWM状态
 * @param  pwm_base PWM基址
 * @retval PWM状态
 * @note 返回值4位有效位，从高到低，0X08表示FWR，FIFO写错误，0X04表示CMP，产生比较事件，0X02表示ROV，发生回滚事件，0X01表示FE，FIFO空状态，都是写1清零，此函数已经做了清零处理
 */
uint8_t Pwm_getStatus(PWM_Type * pwm_base)
{
    uint8_t ret = ((pwm_base->PWMSR >> 3) & 0XF);
    pwm_base->PWMSR |= (0xF << 3);
    return ret;
}

/**
 * @brief  PWM中断使能
 * @param  pwm_base PWM基址
 * @param  irq PWM中断源
 * @retval 无
 * @note 中断源3位有效位，有三种，从高到低，0X04为CIE比较中断，0X02为RIE回滚中断，0X01为FIE是FIFO中断
 */
void Pwm_enableInterruptSource(PWM_Type * pwm_base, PWM_INTERRUPT_SOURCE irq)
{
    pwm_base->PWMIR |= (irq & 0X7);
}

/**
 * @brief  PWM中断失能
 * @param  pwm_base PWM基址
 * @param  irq PWM中断源
 * @retval 无
 * @note 中断源3位有效位，有三种，从高到低，0X04为CIE比较中断，0X02为RIE回滚中断，0X01为FIE是FIFO中断
 */
void Pwm_disableInterruptSource(PWM_Type * pwm_base, PWM_INTERRUPT_SOURCE irq)
{
    pwm_base->PWMIR &= ~(irq & 0X7);
}


/**
 * @brief  设置PWM周期值
 * @param  pwm_base PWM基址
 * @param  period PWM周期值
 * @retval 无
 * @note PWMO(Hz)=PCLK(Hz)/(period+2)，即实际设置的周期值为period+1
 */
void Pwm_setPeriod(PWM_Type * pwm_base, uint16_t period)
{
    pwm_base->PWMPR = period & 0XFFFF;
}

/**
 * @brief  设置PWM比较值
 * @param  pwm_base PWM基址
 * @param  sample 比较值
 * @retval 无
 * @note PWM每一个周期都必须设定一个sample值，sample值会进入4级FIFO
 */
void Pwm_setSample(PWM_Type * pwm_base, uint16_t sample)
{
    pwm_base->PWMSAR = sample & 0XFFFF;
}

/**
 * @brief  获取PWM计数值
 * @param  pwm_base PWM基址
 * @retval PWM计数值
 * @note 
 */
uint16_t Pwm_getCounter(PWM_Type * pwm_base)
{
    return (pwm_base->PWMCNR & 0XFFFF);
}