#include "bsp_keyfilter.h"
#include "bsp_gpio.h"
#include "bsp_key.h"
#include "bsp_epit.h"
#include "bsp_beep.h"
#include "bsp_int.h"

static void gpio1_16_31_irqhandler(uint32_t gccira, void * param);
static void epit_irqhandler(uint32_t gcciar, void * param);

/**
 * @brief  初始化按键pin脚
 * @retval 无
 * @note 注册按键中断，开启下降沿中断，配置定时器，注册定时器中断，开启定时器中断
 */
void Key_initFilter()
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);
    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    Gpio_init(GPIO1,  18, &key_config);

    system_register_irqHandler(GPIO1_Combined_16_31_IRQn, gpio1_16_31_irqhandler, EPIT1);
    Gpio_enableInterrupt(GPIO1, 18);
    GIC_enableIrq(GPIO1_Combined_16_31_IRQn);
    
    Epit_init(EPIT1, 0, 0, 0);
    system_register_irqHandler(EPIT1_IRQn, epit_irqhandler, EPIT1);
    Epit_enableIrq(EPIT1);
    GIC_enableIrq(EPIT1_IRQn);
}

/**
 * @brief  引脚中断函数
 * @param  gccira 无实际作用
 * @param  param EPIT基址
 * @retval 无
 * @note 重启10ms定时器
 */
static void gpio1_16_31_irqhandler(uint32_t gccira, void * param)
{
    Epit_stopCounter(param);
    Epit_setPeriod(param, 66000000/100);
    Epit_startCounter(param);
    Gpio_clearInterruptFlahs(GPIO1, 18);
}

/**
 * @brief  定时器中断
 * @param  gcciar 无实际作用
 * @param  param EPIT基址
 * @retval 无
 * @note 使用按键扫描
 */
static void epit_irqhandler(uint32_t gcciar, void * param)
{
    static uint8_t state = OFF;
    EPIT_Type * epit = (EPIT_Type *)param;

    if ((Epit_getInterruptStatus(epit) & (1<<0)) == 1)
    {
        Epit_stopCounter(param);
        static key_type_t key;
        key.cont = 0;
        key.triger = 0;
        Key_read(&key);
        if (key.triger & 0x1)
        {
            state = !state;
            Beep_switch(state);
        }
        
    }
    Epit_clearInterruptStatus(epit);
}