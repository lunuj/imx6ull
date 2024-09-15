#include "bsp_delay.h"
#include "bsp_key.h"
#include "bsp_gpio.h"

/**
 * @brief 按键初始化
 * @retval 无
 * @note 初始化按键为GPIO1的18，输入模式
 */ 
void key_init(void){
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);
    gpio_pin_config_t key_config;
    key_config.direction = kGPIO_DigtalInput;
    gpio_init(GPIO1, 18, &key_config);
}

/**
 * @brief 获取按键值
 * @retval 
 * @note 获取按键值，若为读取时按键还未按下，会进行10ms按键消抖，若按键已经按下，则返回 KEY0_VALUE，若按键未啊你行啊
 */
int key_getvalue(void){
    int ret = 0;
    int _get_data = gpio_pinread(GPIO1, 18);
    static uint8_t release = 1;
    if(_get_data == 0){
        if(release != 0){
            delay(10);
            release = 0;
        }
        if(_get_data == 0){
            return KEY0_VALUE;
        }

    }
    else if (_get_data == 1)
    {
        ret = KEY_NONE;
        release = 1;
    }
    return ret;
}