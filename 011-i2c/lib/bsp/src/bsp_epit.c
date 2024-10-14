#include "bsp_epit.h"
#include "bsp_int.h"
#include "bsp_beep.h"
#include "bsp_led.h"

/**
 * @brief  EPIT初始化配置
 * @param  epit 指向EPIT基址的指针
 * @param  div 分频系数，最大为4096
 * @param  period 自动重装值，最大为0xFFFF_FFFF
 * @param  cmp 比较器比较值，最大为0xFFFF_FFFF
 * @retval 无
 * @note 默认使用peripheral clock，一般为66MHz，配置为周期计数模式，且计数器初始值为自动重装值
 */
void Epit_init(EPIT_Type * epit, uint16_t div, uint32_t period, uint32_t cmp)
{
    epit->CR = 0;          //寄存器清零
    epit->CR |= 1<<24;      //配置时钟源为peripheral clock
    epit->CR |= (div&0xfff) << 4;   //配置分频系数，最大为4096
    epit->CR |= 1<<3;       //配置为周期计数模式
    epit->CR |= 1<<1;       //配置计数器初值从LD寄存器取出

    epit->LR = period;     //配置自动重装值
    epit->CMPR = cmp;      //配置比较值
}

void Epit_setPeriod(EPIT_Type * epit, uint32_t period)
{
    epit->LR = period;     //配置自动重装值
}

void Epit_enableIrq(EPIT_Type * epit)
{
    epit->CR |= 1<<2;       //开启比较中断，需要注意，当CNR==CMPR时，会触发中断，但此时计数值仍会继续递减计数直到0
}

void Epit_disableIrq(EPIT_Type * epit)
{
    epit->CR &= ~(1<<2);     //关闭比较中断
}

uint32_t Epit_getInterruptStatus(EPIT_Type * epit)
{
    return epit->SR;
}

void Epit_clearInterruptStatus(EPIT_Type * epit)
{
    epit->SR |= ~(1<<0);
}

void Epit_startCounter(EPIT_Type * epit)
{
    epit->CR |= 1<<0; 
}

void Epit_stopCounter(EPIT_Type * epit)
{
    epit->CR &= ~(1<<0);
}

/**
 * @brief  EPIT中断函数
 * @param  gcciar 这里不需要没有实际参数，但是定义函数的时候是按照前面声明函数是定义的，就把直接把样式复制过来了
 * @param  param EPIT中断基址
 * @retval 无
 * @note 使用前需配置中断函数，使能EPIC中断，GIC中断，以及全局中断
 */
void epit_irqhandler(uint32_t gcciar, void * param)
{
    static uint8_t state = OFF;
    EPIT_Type * epit = (EPIT_Type *)param;
    state = !state;
    if (epit->SR & (1<<0))
    {
        Beep_switch(state);
    }
    epit->SR |= ~(1<<0);
}