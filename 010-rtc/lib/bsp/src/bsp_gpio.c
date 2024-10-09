#include "bsp_gpio.h"

/**
 * @brief GPIO初始化
 * @param  base 指向GPIO基址的指针
 * @param  pin 初始化的pin
 * @param  config 设置内容
 * @retval 无
 * @note
 */
void Gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
    if (config->direction == kGPIO_DigitalInput)
    {
        base->GDIR &= ~(1 << pin);
    }
    else
    {
        base->GDIR |= 1 << pin;
        Gpio_pinWrite(base, pin, config->outputLogic);
    }
    Gpio_configInterrruptType(base, pin, config->interruptMode);
}

/**
 * @brief  读取GPIO的引脚电平
 * @param  base 指向GPIO基址的指针
 * @param  pin  读取的引脚
 * @retval 读取引脚的电平值
 * @note
 */
int Gpio_pinRead(GPIO_Type *base, int pin)
{
    return (((base->DR) >> pin) & 0x1);
}

/**
 * @brief  更改GPIO引脚输出电平
 * @param  base 指向GPIO基址的指针
 * @param  pin  输出pin脚
 * @param  value    输出电平
 * @retval 无
 * @note
 */
void Gpio_pinWrite(GPIO_Type* base, int pin, int value)
{
    if (value == 0)
    {
        base->DR &= ~(1U << pin);
    }
    else
    {
        base->DR |= (1U << pin);
    }
}

/**
 * @brief  GPIO中断使能
 * @param  base 待初始化的GPIO组
 * @param  pin 待初始化的GPIO在组内编号
 * @retval 无
 * @note
 */
void Gpio_enableInterrupt(GPIO_Type* base, uint16_t pin)
{
    base->IMR |= 1 << pin;
}

/**
 * @brief  GPIO中断禁止
 * @param  base 待初始化的GPIO组
 * @param  pin 待初始化的GPIO在组内编号
 * @retval 无
 * @note
 */
void Gpio_disableInterrupt(GPIO_Type* base, uint16_t pin)
{
    base->IMR &= ~(1 << pin);
}

/**
 * @brief  GPIO中断标志位清除
 * @param  base 带初始化的GPIO组
 * @param  pin 待初始化的GPIO在组内编号
 * @retval 无
 * @note
 */
void Gpio_clearInterruptFlahs(GPIO_Type* base, uint16_t pin)
{
    base->ISR |= 1 << pin;
}

/**
 * @brief  GPIO中断标志位清除
 * @param  base 带初始化的GPIO组
 * @param  pin 待初始化的GPIO在组内编号
 * @param  pin_int_mode 中断触发类型
 * @retval 无
 * @note 默认中断类型为边沿触发
 */
void Gpio_configInterrruptType(GPIO_Type* base, uint16_t pin, gpio_interrupt_mode_t pin_int_mode)
{
    volatile uint32_t *icr; // icr寄存器，icr通过pin的值来判定
    uint32_t icrShift;      // 最终使用的pin的值

    icrShift = pin; // icrShift值初始化

    base->EDGE_SEL &= ~(1 << pin); // 清除边沿触发标志位，该位如果为1,上升、下降沿触发失效

    if (pin < 16) // pin对应低16位时
    {
        icr = &(base->ICR1);
    } //&为取址符，icr为IRC1地址

    else // pin对应高16为时
    {
        icr = &(base->ICR2); // icr为ICR2地址
        icrShift -= 16;      // 超过16,减16就是对应bit位，前面初始化的icrShift被修改
    }

    /*
    根据触发信号类型修改ICR1/ICR2的值
    */
    switch (pin_int_mode)
    {
    case kGPIO_IntLowLevel:
        *icr &= ~(3 << (2 * icrShift));
        break;
    case kGPIO_IntHighLevel:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= ~(1 << (2 * icrShift));
        break;
    case kGPIO_IntRisingEdge:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= (2 << (2 * icrShift));
        break;
    case kGPIO_IntFallingEdge:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= (3 << (2 * icrShift));
        break;
    case kGPIO_IntRisingOrFallingEdge:
        base->EDGE_SEL |= (1 << pin);
        break;
    default:
        break;
    }
}
