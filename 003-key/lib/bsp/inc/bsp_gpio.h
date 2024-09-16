#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "imx6ull.h"

/*枚举类型描述GPIO方向*/
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput = 0U,
    kGPIO_DigitalOutput = 1U,
}_gpio_pin_direction_t;

typedef struct gpio_pin_config
{
    _gpio_pin_direction_t direction;
    uint8_t outputLogic;
}gpio_pin_config_t;

void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);

#endif // BSP_GPIO_H