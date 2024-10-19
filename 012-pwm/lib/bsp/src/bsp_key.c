#include "bsp_delay.h"
#include "bsp_key.h"
#include "bsp_gpio.h"

/**
 * @brief 按键初始化
 * @retval 无
 * @note 初始化按键为GPIO1的18，输入模式
 */ 
void Key_init(void){
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);
    gpio_pin_config_t key_config;
    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    Gpio_init(GPIO1, 18, &key_config);
}

/**
 * @brief 获取按键值
 * @retval 
 * @note 获取按键值，若为读取时按键还未按下，会进行10ms按键消抖，若按键已经按下，则返回 KEY0_VALUE，若按键未啊你行啊
 */
int Key_getValue(void){
    int ret = 0;
    int _get_data = Gpio_pinRead(GPIO1, 18);
    static uint8_t release = 1;
    if(_get_data == 0){
        if(release != 0){
            Delay_ms(GPT2, 10);
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

/**
 * @brief  读取按键状态
 * @param  key_read key_type_t类型的指针，triger判断是否触发，cont判断是否按下
 * @retval 无
 * @note 将按键状态读取到传递的参数中去
 */
void Key_read(key_type_t * key_read){
    uint8_t read = Gpio_pinRead(GPIO1, 18) ^ 0xff;
    key_read->triger = read & (read ^ key_read->cont);
    key_read->cont = read;
}