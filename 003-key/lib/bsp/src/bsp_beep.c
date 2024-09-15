// SNVS_TAMPER1复用为GPIO5_IO1

#include "bsp_beep.h"

/**
 * @brief 蜂鸣器初始化
 * @retval 无
 * @note 在GPIO5的IO1初始化为输出模式，默认输出为高电平，关闭蜂鸣器
 */
void beep_init(void)
{ // GPIO复用初始化，复用为GPIO5_IO01
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
    // 电气属性设置
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10B0);
    // 设置为输出
    GPIO5->GDIR |= (1 << 1);
    // 输出高电平，关闭蜂鸣器
    GPIO5->DR |= (1 << 1);
}

/**
 * @brief 切换蜂鸣器状态
 * @param status 设置蜂鸣器的状态，==1为开启，!=1为关闭
 * @retval 无
 * @note 
 */
void beep_switch(int status)
{
    if (status == ON)
    { // IO设置为0
        GPIO5->DR &= ~(1 << 1);
    }
    else
    { // IO设置为1
        GPIO5->DR |= (1 << 1);
    }
}