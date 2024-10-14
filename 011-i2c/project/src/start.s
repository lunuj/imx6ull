.global _start

_start:
    @添加中断向量表 
    ldr pc,=Reset_Handler           //复位中断函数
    ldr pc,=Undefined_Handler       //未定义指令中断函数
    ldr pc,=SVC_Handler             //SVC
    ldr pc,=PreAbort_Handler        //预取终止
    ldr pc,=DataAbort_Handler       //数据终止
    ldr pc,=NotUsed_Handler         //未使用
    ldr pc,=IRQ_Handler             //IRQ中断
    ldr pc,=FIQ_Handler             //FIQ中断

/*复位中断服务函数 */
Reset_Handler:

    CPSID i                 @禁止全局中断

/*操作CP15协处理器，关闭ICache DCache,MMU*/

    MRC p15,0,r0,c1,c0,0    @读取SCTLR寄存器的数据到R0寄存器中

    BIC r0,r0,#(1<<12)       @清零bit12,关闭I Cache
    BIC r0,r0,#(1<<11)       @清零bit11，关闭分支预测
    BIC r0,r0,#(1<<2)        @清零bit2,关闭D Cache
    BIC r0,r0,#(1<<1)        @清零bit1,关闭对齐控制
    BIC r0,r0,#(1<<0)        @清零bit0,关闭MMU

    MCR p15,0,r0,c1,c0,0    @将R0里的数据写入到SCTLR寄存器中

/*设置中断向量偏移(此步骤可以在C语言里实现)修改P15协处理器VBVR寄存器 */
#if 0
    LDR r0,=0x87800000
    DSB
    ISB   @同步指令，清洗流水线
    MCR p15,0,r0,c12,c0,0   @设置VBVR寄存器=0x87800000
    DSB
    ISB
#endif

/*设置各个模式下sp指针 */

    /*进入IRQ模式 */
    MRS r0,CPSR
    BIC r0,r0,#0x1f
    ORR r0,r0,#0x12
    MSR CPSR,r0
    LDR sp,=0x80600000

    /*进入SYS模式 */
    MRS r0,CPSR
    BIC r0,r0,#0x1f
    ORR r0,r0,#0x1f
    MSR CPSR,r0
    LDR sp,=0x80400000

    /*进入SVC模式 */
    MRS r0,CPSR
    BIC r0,r0,#0x1f
    ORR r0,r0,#0x13
    MSR CPSR,r0
    LDR sp,=0x80200000

    CPSIE i             @使能全局中断
    b main

/*未定义指令中断服务函数 */
Undefined_Handler:
    ldr r0,=Undefined_Handler
    bx r0

/*SVC中断服务函数 */
SVC_Handler:
    ldr r0,=SVC_Handler
    bx r0

/*预取终值中断服务函数 */
PreAbort_Handler:
    ldr r0,=PreAbort_Handler
    bx r0

/*数据终值中断服务函数 */
DataAbort_Handler:
    ldr r0,=DataAbort_Handler
    bx r0

/*未使用中断服务函数 */
NotUsed_Handler:
    ldr r0,=NotUsed_Handler
    bx r0

/*IRQ中断服务函数 */
IRQ_Handler:

    PUSH {lr}                    /* 保存lr寄存器内容 */
    PUSH {r0-r3, r12}            /* 保存r0-r3，r12寄存器 */

    MRS r0, spsr                /* 读取spsr寄存器 */
    PUSH {r0}                    /* 保存spsr寄存器 */

    MRC p15, 4, r1, c15, c0, 0 /* 从CP15的C0寄存器CBAR内的值到R1寄存器中，获取GIC基地址
                                * 参考文档ARM Cortex-A(armV7)编程手册V4.0.pdf P49
                                * Cortex-A7 Technical ReferenceManua.pdf P68 P138*/
    
    ADD r1, r1, #0x2000         /* GIC基地址加0X2000，也就是GIC的CPU接口端基地址
                                 *至此R1保存内容为GIC里CPUInterface基地址 */
 
    LDR r0, [r1, #0xC]            /* GIC的CPU接口端基地址加0X0C就是GICC_IAR寄存器，从bit[9:0]获取中断ID
                                 *至此，R0保存为GICC_IAR,对应中断ID */

    push {r0, r1}                /* 保存r0,r1 */

    CPS #0x13                    /* 进入SVC模式，允许其他中断再次进去  注意R0和R1内为有用数据，后面使用时应注意*/
    
    PUSH {lr}                    /* 保存SVC模式的lr寄存器 */

    LDR r2, =system_irqhandler    /* 加载C语言中断处理函数到r2寄存器中 该函数需要传参
                                 * 跳转至定义的C语言中断处理函数，中断ID作为参数保存在R0中 */

    BLX r2                         /* r2是指向函数的地址，所以用BX，加BXL是保存跳转处地址至LR寄存器，共返回时使用*/ 

    POP {lr}                    /* 执行完C语言中断服务函数，lr出栈 */
    CPS #0x12                    /* 进入IRQ模式 */

    POP {r0, r1}                
    STR r0, [r1, #0X10]            /* 中断执行完成，写EOIR ,此步骤进行后，R0和R1的值就不重要了，可以被覆盖*/

    POP {r0}                        
    MSR spsr_cxsf, r0            /* 恢复spsr */

    POP {r0-r3, r12}            /* r0-r3,r12出栈 */
    POP {lr}                    /* lr出栈 */
    
    SUBS pc, lr, #4                /* 将lr-4赋给pc */

/*FIQ中断服务函数 */
FIQ_Handler:
    ldr r0,=FIQ_Handler
    bx r0
