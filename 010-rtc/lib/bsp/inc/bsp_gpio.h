#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "imx6ull.h"
typedef enum
{
    kGPIO_NoIntmode = 0U,                   //无触发
    kGPIO_IntLowLevel = 1U,                 //低电平触发
    kGPIO_IntHighLevel = 2U,                //高电平触发
    kGPIO_IntRisingEdge = 3U,               //上升沿触发
    kGPIO_IntFallingEdge = 4U,              //下降沿触发
    kGPIO_IntRisingOrFallingEdge = 5U,      //边沿触发
}gpio_interrupt_mode_t;

/*枚举类型描述GPIO方向*/
typedef enum
{
    kGPIO_DigitalInput = 0U,
    kGPIO_DigitalOutput = 1U,
}gpio_pin_direction_t;

typedef struct
{
    gpio_pin_direction_t direction;
    uint8_t outputLogic;
    gpio_interrupt_mode_t interruptMode;
}gpio_pin_config_t;

void Gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
int Gpio_pinRead(GPIO_Type *base, int pin);
void Gpio_pinWrite(GPIO_Type *base, int pin, int value);
void Gpio_enableInterrupt(GPIO_Type *base, uint16_t pin);
void Gpio_disableInterrupt(GPIO_Type *base, uint16_t pin);
void Gpio_clearInterruptFlahs(GPIO_Type *base, uint16_t pin);
void Gpio_configInterrruptType(GPIO_Type *base, uint16_t pin, gpio_interrupt_mode_t pin_int_mode);

#endif // BSP_GPIO_H