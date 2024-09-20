#ifndef BSP_INT_H
#define BSP_INT_H

#include "imx6ull.h"

/*声明终端处理函数*/
typedef void(*system_irq_handler_t)(unsigned int gicciar,void *param);

/*创建中断函数结构体*/
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHander; //中断处理函数
    void *userParam;                //中断处理函数的参数
}sys_irq_handle_t;

void int_init(void);
void system_irqtable_init(void);
void default_irqhandler(unsigned int gcciar,void *userParam);
void system_irqhandler(unsigned int gicciar);
void system_register_irqHandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);

#endif // BSP_INT_H
