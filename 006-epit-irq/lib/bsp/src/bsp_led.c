#include "bsp_led.h"
#include "bsp_gpio.h"

/*初始化LED*/
void Led_init(void)
{   
    // 复用、电气属性寄存器初始化

    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03,0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03,0x10B0);
    // GPIO1方向寄存器，
    gpio_pin_config_t gpio_config;

    gpio_config.direction = kGPIO_DigitalOutput;
    gpio_config.outputLogic = 0U;

    Gpio_init(GPIO1,3,&gpio_config);
}


// 点亮LED
void Led_on(void)
{
    Gpio_pinWrite(GPIO1,3,0U);
}


// 关闭LED
void Led_off(void)
{
    Gpio_pinWrite(GPIO1,3,1U);
}


void Led_switch(int led, int status)
{    
    switch(led)
    {
        case LED0:
            if(status == ON)
                Led_on();
            else if(status == OFF)
                Led_off();
            break;
    }
}