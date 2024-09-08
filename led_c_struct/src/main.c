#include "main.h"
/*使能外设时钟*/
void clk_enabled(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
}

/*初始化LED*/
void led_init(void)
{   
    // 复用、电气属性寄存器初始化
    SW_MUX_GPIO1_IO03 = 0x5;
    SW_PAD_GPIO1_IO03 = 0x10B0;
    // GPIO1方向寄存器，
    GPIO1_GDIR = 0x8;
}
void delay_short(volatile unsigned int n)
{
    while(n--);
}

/*延时，主频在396Hz下一次循环大概是1ms*/

void delay(volatile unsigned int n){
    while(n--){
        delay_short(0x7ff);
    }
}

// 点亮LED
void led_on(void)
{
    GPIO1_DR &= ~(1<<3);//bit4清零
}

// 关闭LED
void led_off(void)
{
    GPIO1_DR |=(1<<3);  //bit4置一
}

//主函数
int main(void)
{
    /*初始化LED*/
    clk_enabled();
    /*设置LED闪烁*/
    led_init();
    while(1)
    {
        led_on();
        delay(500);
        led_off();
        delay(500);
    }
    return 0;
}