#include "bsp_gpio.h"

/**
 * @brief GPIO初始化
 * @param  base 指向GPIO基址的指针
 * @param  pin 初始化的pin
 * @param  config 设置内容
 * @retval 无
 * @note 
 */
void gpio_init(GPIO_Type* base, int pin, gpio_pin_config_t* config)
{
    if (config->direction == kGPIO_DigtalInput)
    {
        base->GDIR &= ~(1 << pin);
    }
    else{
        base->GDIR |= 1<<pin;
        gpio_pinwrite(base, pin, config->outputLogic);
    }
}

/**
 * @brief  读取GPIO的引脚电平
 * @param  base 指向GPIO基址的指针
 * @param  pin  读取的引脚
 * @retval 读取引脚的电平值
 * @note 
 */
int gpio_pinread(GPIO_Type* base, int pin)
{
    return (((base->DR )>>pin) &0x1);
}

/**
 * @brief  更改GPIO引脚输出电平
 * @param  base 指向GPIO基址的指针
 * @param  pin
 * @param  value
 * @retval 
 * @note 
 */
void gpio_pinwrite(GPIO_Type* base, int pin, int value)
{
    if(value == 0){
        base->DR &= ~(1U << pin);
    }
    else{
        base->DR |= (1U << pin);
    }
}
