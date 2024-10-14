#include "bsp_int.h"

static unsigned int irqNesting; // 记录中断嵌套计数器

/*定义中断处理函数表*/
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS]; // NUMBER_OF_INT_VECTORS为中断ID个数

/**
 * @brief  默认中断处理函数
 * @param  gcciar 中断ID
 * @param  userParam 中断服务处理函数参数
 * @retval 无
 * @note 无
 */
void default_irqhandler(unsigned int gcciar, void *userParam)
{
    while (1)
    {
    }
}

/**
 * @brief  初始化中断向量表
 * @retval 无
 * @note 无
 */
void system_irqtable_init(void)
{
    unsigned int i = 0;
    irqNesting = 0;
    for (i = 0; i < NUMBER_OF_INT_VECTORS; i++)
    {
        irqTable[i].irqHander = default_irqhandler; // 初始化为默认函数
        irqTable[i].userParam = NULL;               // 参数为指针，指向空
    }
}

/**
 * @brief  中断初始化函数
 * @retval 无
 * @note 包含GIC初始化，初始化中断向量表，中断向量偏移
 */
void int_init(void)
{
    GIC_init();             // GIC初始化
    system_irqtable_init(); // 初始化中断函数表
    __set_VBAR(0x87800000); // 中断向量表偏执
}

/**
 * @brief  注册中断处理函数
 * @param  irq  中断id
 * @param  handler  中断处理服务函数
 * @param  userParam    中断服务处理函数参数
 * @retval 无
 * @note
 */
void system_register_irqHandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam)
{
    irqTable[irq].irqHander = handler;   // 函数
    irqTable[irq].userParam = userParam; // 参数
}

/**
 * @brief  C语言中断服务，IRQ中断调用函数
 * @param  gicciar 中断ID
 * @retval 无
 * @note 无
 */
void system_irqhandler(unsigned int gicciar)
{
    uint32_t intNum = gicciar & 0x3FF; // bit[9:0]共10位，转换过来就是0x3FF与运算就可获取中断ID

    /* 检查中断ID是否为异常*/
    if (intNum >= NUMBER_OF_INT_VECTORS)
    {
        return; // 中断ID异常，直接返回
    }

    irqNesting++;                                                   // 有新中断，计数器+1
    irqTable[intNum].irqHander(intNum, irqTable[intNum].userParam); // 执行具体中断处理
    irqNesting--;                                                   // 中断处理完成，嵌套计数器-1
}