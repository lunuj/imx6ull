#include "bsp_gpt.h"

/**
 * @brief  初始化GPT模块
 * @param  gpt GPT基址
 * @param  div 分频系数，应为实际分频-1
 * @retval 无
 * @note GPT为向上计数
 */
void Gpt_init(GPT_Type * gpt, uint8_t div){
    gpt->CR = 0;
    Gpt_reset(gpt);
    gpt->CR = 0;                    //清除GPT
    gpt->CR |= (1<<15);             //复位GPT
    while((gpt->CR >>15) & 0x01);   //复位结束，SWR回0,跳出while循环

    gpt->CR |= (1<<1) | (1<<6);     //设置计数模式为周期计数，配置时钟源为Peripheral Clock (ipg_clk)66MHz
    gpt->PR = div & 0xfff;           //设置分频系数，最大为4096
}

/**
 * @brief  复位GPT模块
 * @param  gpt GPT基址
 * @retval 无
 * @note 此函数会阻塞，直到GPT复位成功，复位时仅EN, ENMOD, STOPEN, WAITEN, and DBGEN bits不会被恢复为默认值，需重新设置比较计数器
 */
void Gpt_reset(GPT_Type * gpt)
{
    gpt->CR |= 1<<15;
    while((gpt->CR >> 15) & 0x1);
}

/**
 * @brief  设置GPT的计数模式
 * @param  gpt GPT基址
 * @param  mode 计数模式，0为自由计数模式，1为周期计数模式
 * @retval 
 * @note 
 */
void Gpt_setMode(GPT_Type * gpt, uint8_t mode)
{
    gpt->CR = (mode & 0x1) << 1;
}


/**
 * @brief  设置GPT分频系数
 * @param  gpt GPT基址
 * @param  div 分频系数，最大为12位为4096
 * @retval 无
 * @note 此处设置分频系数为时钟源，应与24M分频器区分
 */
void Gpt_setDiv(GPT_Type * gpt, uint16_t div)
{
    gpt->PR = div & 0x1f;
}

/**
 * @brief  设置GPT比较计数器0的值
 * @param  gpt GPT基址
 * @param  period 计数器值
 * @retval 无
 * @note GPT有三个比较计数器，在周期计数下仅0号有效
 */
void Gpt_setCmp0(GPT_Type * gpt, uint32_t period)
{
    gpt->OCR[0] = period;
}


/**
 * @brief  设置GPT比较计数器1的值
 * @param  gpt GPT基址
 * @param  period 计数器值
 * @retval 无
 * @note GPT有三个比较计数器，在周期计数下仅0号有效
 */
void Gpt_setCmp1(GPT_Type * gpt, uint32_t period)
{
    gpt->OCR[1] = period;
}

/**
 * @brief  设置GPT比较计数器2的值
 * @param  gpt GPT基址
 * @param  period 计数器值
 * @retval 无
 * @note GPT有三个比较计数器，在周期计数下仅0号有效
 */
void Gpt_seCmp2(GPT_Type * gpt, uint32_t period)
{
    gpt->OCR[2] = period;
}

void Gpt_enableIrq(GPT_Type * gpt, GPT_IRQ_SOURCE irq)
{
    gpt->IR |= irq;
}

void Gpt_disableIrq(GPT_Type * gpt, GPT_IRQ_SOURCE irq)
{
    gpt->IR &= ~irq;
}

uint8_t Gpt_getInterruptStatus(GPT_Type * gpt)
{
    return gpt->SR;
}

void Gpt_clearInterruptStatus(GPT_Type * gpt, GPT_IRQ_SOURCE irq)
{
    gpt->SR |= irq;
}

void Gpt_startCounter(GPT_Type * gpt)
{
    gpt->CR |= 1<<0;
}

void Gpt_stopCounter(GPT_Type * gpt)
{
    gpt->CR &= ~(1<<0);
}

uint32_t Gpt_readCounter(GPT_Type * gpt)
{
    return gpt->CNT;
}