// SNVS_TAMPER1复用为GPIO5_IO1

#include "bsp_beep.h"

void beep_init(void)
{   //GPIO复用初始化，复用为GPIO5_IO01
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0); 
    //电气属性设置
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0x10B0);
    //设置为输出
    GPIO5->GDIR |=(1<<1);
    //输出高电平，关闭蜂鸣器
    GPIO5->DR   |=(1<<1);
}
// 蜂鸣器控制

void beep_switch(int status)
{
    if(status == ON)
    {   //IO设置为0
        GPIO5->DR &=~(1<<1);
    }
    else
    {   //IO设置为1
        GPIO5->DR |=(1<<1);
    }
}