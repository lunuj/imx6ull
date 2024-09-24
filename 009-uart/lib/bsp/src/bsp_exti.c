#include "bsp_exti.h"
#include "bsp_gpio.h"
#include "bsp_beep.h"
#include "bsp_int.h"
#include "bsp_delay.h"

void Exti_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);
    gpio_pin_config_t key_config;
    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    Gpio_init(GPIO1, 18, &key_config);

    GIC_enableIrq(GPIO1_Combined_16_31_IRQn);

    system_register_irqHandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)Gpio_io18_irqhandler, NULL);
    Gpio_enableInterrupt(GPIO1, 18);
}

void Gpio_io18_irqhandler(uint16_t gicciar, void * param){
    static unsigned char state = ON;
    Delay_ms(GPT2, 10);
    if(Gpio_pinRead(GPIO1, 18) == 0)
    {
        state = !state;
        Beep_switch(state);
    }
    Gpio_clearInterruptFlahs(GPIO1, 18);
}