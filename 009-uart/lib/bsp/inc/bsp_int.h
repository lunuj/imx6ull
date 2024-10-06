#ifndef BSP_INT_H
#define BSP_INT_H

#include "imx6ull.h"

/*声明终端处理函数*/
typedef void(*system_irq_handler_t)(unsigned int gicciar,void *param);

/*创建中断函数结构体*/
typedef struct
{
    system_irq_handler_t irqHander; //中断处理函数
    void *userParam;                //中断处理函数的参数
}sys_irq_handle_t;


void int_init(void);
void system_irqtable_init(void);
void default_irqhandler(unsigned int gcciar,void *userParam);
void system_irqhandler(unsigned int gicciar);
void system_register_irqHandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);

FORCEDINLINE __STATIC_INLINE void __set_VBAR(uint32_t vbar)
{
    __MCR(15, 0, vbar, 12, 0, 0);
}


FORCEDINLINE __STATIC_INLINE uint32_t __get_CBAR(void)
{
    return __MRC(15, 4, 15, 0, 0);
}


/**
 * @brief  初始化GIC寄存器
 * @retval 无
 * @note 只使用group0
 */
FORCEDINLINE __STATIC_INLINE void GIC_init(void)
{
    uint32_t i;
    uint32_t irqRegs;
    GIC_Type *gic = (GIC_Type *)(__get_CBAR() & 0xFFFF0000UL);

    irqRegs = (gic->D_TYPER & 0x1FUL) + 1;

    /* On POR, all SPI is in group 0, level-sensitive and using 1-N model */
    // 关闭SPI、PPI、SGI中断
    for (i = 0; i < irqRegs; i++)
        gic->D_ICENABLER[i] = 0xFFFFFFFFUL;

    /* Make all interrupts have higher priority */
    gic->C_PMR = (0xFFUL << (8 - __GIC_PRIO_BITS)) & 0xFFUL;

    /* No subpriority, all priority level allows preemption */
    gic->C_BPR = 7 - __GIC_PRIO_BITS;

    /* Enable group0 distribution */
    gic->D_CTLR = 1UL;

    /* Enable group0 signaling */
    gic->C_CTLR = 1UL;
}

FORCEDINLINE __STATIC_INLINE void GIC_enableIrq(IRQn_Type IRQn)
{
    GIC_Type *gic = (GIC_Type *)(__get_CBAR() & 0xFFFF0000UL);
    gic->D_ISENABLER[((uint32_t)(int32_t)IRQn) >> 5] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

FORCEDINLINE __STATIC_INLINE void GIC_disableIRQ(IRQn_Type IRQn)
{
    GIC_Type *gic = (GIC_Type *)(__get_CBAR() & 0xFFFF0000UL);
    gic->D_ICENABLER[((uint32_t)(int32_t)IRQn) >> 5] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

#endif // BSP_INT_H
