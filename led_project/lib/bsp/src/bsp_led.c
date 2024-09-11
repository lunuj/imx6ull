#include "bsp_led.h"

/*初始化LED*/
void led_init(void)
{   
    // 复用、电气属性寄存器初始化

    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03,0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03,0x10B0);
    // GPIO1方向寄存器，
    GPIO1->GDIR = 0x8;
}


// 点亮LED
void led_on(void)
{
    GPIO1->DR &= ~(1<<3); //bit3清零
}


// 关闭LED
void led_off(void)
{
    GPIO1->DR |=(1<<3);  //bit3置一
}


void led_switch(int led, int status)
{    
    switch(led)
    {
        case LED0:
            if(status == ON)
                GPIO1->DR &= ~(1<<3);    /* 打开LED0 */
            else if(status == OFF)
                GPIO1->DR |= (1<<3);    /* 关闭LED0 */
            break;
    }
}